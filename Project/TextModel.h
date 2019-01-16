#pragma once

#include <vector>
#include "Model.h"

#include <glm/glm.hpp>

class TextModel : public Model
{
public:
	TextModel(const std::string& fontTexturePath);
	~TextModel();
	// Render text using passed shader. Text need to be set in setTextToRender before it can be rendered
	void render(const Shader& shader) const override;
	// Set text to render with x and y representing start position and size is size of the letters
	void setTextToRender(const std::string& text, int x, int y, int size);
private:
	unsigned int text2DTextureID;              // Texture containing the font
	unsigned int vao;
	unsigned int text2DVertexBufferID;      // Buffer containing the vertices
	unsigned int text2DUVBufferID;          // UVs

	std::vector<glm::vec2> vertices;
};