#pragma once

#include <string>
#include <vector>

class Shader;

// Abstract model class that serves as a base class for all specialized model classes
class Model
{
public:
	virtual ~Model() = default;
	// Render model using passed shader
	virtual void render(const Shader& shader) const = 0;
	// Loads texture
	static unsigned int loadTextureFromFile(const char* texturePath);
	// Load texture in DDS format
	static unsigned int loadDDS(const char* path);
	// Creates cubemap texture from 6 separate textures
	static unsigned int loadCubemapTexture(const std::vector<std::string>& faces);
};