#pragma once

#include <vector>
#include <glm/glm.hpp>

using namespace glm;

class Shader;

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Material
{
public:
	Material(const std::vector<Texture>& textures, vec3 ambient = vec3(0.0f), vec3 diffuse = vec3(0.0f), vec3 specular = vec3(0.0f), float shininess = 1.0f);
	// Bind all material properties/textures to the shader
	void bind(const Shader& shader) const;
private:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	std::vector<Texture> textures;
};