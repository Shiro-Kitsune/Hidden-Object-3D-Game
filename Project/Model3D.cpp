#include "Model3D.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>

Model3D::Model3D(const std::string& path)
{
	loadModel(path);
}

Model3D::~Model3D()
{
	for (auto mesh : meshes)
	{
		delete mesh;
	}

	// Delete all loaded textures. Must be done here instead of Mesh otherwise may attempt to delete the same texture several times
	for (auto& texture : loadedTextures)
	{	
		glDeleteTextures(1, &texture.id);
	}
}

void Model3D::render(const Shader& shader) const
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->render(shader);
	}
}

void Model3D::loadModel(std::string const &path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scenes = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	// check for errors
	if (!scenes || scenes->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scenes->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scenes->mRootNode, scenes);
}

void Model3D::processNode(aiNode *node, const aiScene *scenes)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scenes->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scenes));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scenes);
	}

}

Mesh* Model3D::processMesh(aiMesh *mesh, const aiScene *scenes)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// Texture coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Process materials
	aiMaterial* material = scenes->mMaterials[mesh->mMaterialIndex];
	Material mat = loadMaterial(material);	
	// Return a mesh object created from the extracted mesh data
	return new Mesh(vertices, indices, mat);
}

Material Model3D::loadMaterial(aiMaterial * mat)
{
	// Extract shininess
	float shininess = 0;
	aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess);
	shininess = (shininess < 1.0f) ? 1.0f : shininess;
	// Extract ambient color
	aiColor3D ambientColor(0.f, 0.f, 0.f);
	mat->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
	// Extract diffuse color
	aiColor3D diffColor(0.f, 0.f, 0.f);
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffColor);
	// Extract specular color
	aiColor3D specColor(0.f, 0.f, 0.f);
	mat->Get(AI_MATKEY_COLOR_SPECULAR, specColor);
	// Extract textures
	std::vector<Texture> textures;
	// Assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// Diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// Specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// Return material object created from extracted data
	return Material(textures, vec3(ambientColor.r, ambientColor.g, ambientColor.b), vec3(diffColor.r, diffColor.g, diffColor.b),
		vec3(specColor.r, specColor.g, specColor.b), shininess);
}

std::vector<Texture> Model3D::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < loadedTextures.size(); j++)
		{
			if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(loadedTextures[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		// If texture hasn't been loaded already, load it
		if (!skip)
		{   
			Texture texture;
			std::string filePath = directory + "/" + str.C_Str();
			texture.id = loadTextureFromFile(filePath.c_str());
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			loadedTextures.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures
		}
	}
	return textures;
}