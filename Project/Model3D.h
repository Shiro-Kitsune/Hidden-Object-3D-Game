#pragma once

#include <vector>

#include <assimp/scene.h>

#include "Model.h"
#include "Mesh.h"
#include "Material.h"

class Shader;

class Model3D : public Model
{
public:
	Model3D(const std::string& path);
	~Model3D();
	// Draws the model and all its meshes
	void render(const Shader& shader) const override;
protected:
	std::vector<Mesh*> meshes;
	std::string directory;
	std::vector<Texture> loadedTextures;  // Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode *node, const aiScene *scenes);
	Mesh* processMesh(aiMesh *mesh, const aiScene *scenes);
	// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(std::string const &path);
	// Loads mesh material including textures and returns it as material class
	Material loadMaterial(aiMaterial *mat);
	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};