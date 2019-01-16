#include "Material.h"
#include <string>
#include <glad/glad.h>
#include "Shader.h"


Material::Material(const std::vector<Texture>& textures, vec3 ambient, vec3 diffuse, vec3 specular, float shininess) 
	: textures(textures), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
{
}

void Material::bind(const Shader& shader) const
{
	// Bind material properties
	shader.bindUniform("material.ambient", ambient);
	shader.bindUniform("material.diffuse", diffuse);
	shader.bindUniform("material.specular", specular);
	shader.bindUniform("material.shininess", shininess);

	// Bind textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
		// Retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}

		shader.bindUniform(("material." + name + number).c_str(), i);
		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
}