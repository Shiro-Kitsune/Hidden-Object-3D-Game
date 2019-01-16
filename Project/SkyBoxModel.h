#pragma once

#include <string>
#include <vector>
#include "Model.h"
#include "Material.h"

// A class that represents a skybox model
class SkyBoxModel : public Model
{
public:
	// Create skybox from 6 textures
	SkyBoxModel(const std::vector<std::string>& faces);
	void render(const Shader& shader) const override;
protected:
	unsigned int vao, vbo;
	float vertices[108];
	Texture texture;

	// Initializes all the buffer objects/arrays
	void createSkyBox();
};