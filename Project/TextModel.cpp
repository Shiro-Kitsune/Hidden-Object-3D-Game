#include "TextModel.h"

#include <glad/glad.h>

TextModel::TextModel(const std::string & fontTexturePath)
{
	// Initialize texture
	text2DTextureID = loadDDS(fontTexturePath.c_str());

	glGenVertexArrays(1, &vao);
	// Initialize VBO
	glGenBuffers(1, &text2DVertexBufferID);
	glGenBuffers(1, &text2DUVBufferID);
}

TextModel::~TextModel()
{
	glDeleteVertexArrays(1, &vao);
	// Delete buffers
	glDeleteBuffers(1, &text2DVertexBufferID);
	glDeleteBuffers(1, &text2DUVBufferID);

	// Delete texture
	glDeleteTextures(1, &text2DTextureID);
}

void TextModel::render(const Shader & shader) const
{
	if (vertices.size() == 0)
	{
		return;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(vao);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, text2DTextureID);	

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glBindVertexArray(0);

	glDisable(GL_BLEND);
}

void TextModel::setTextToRender(const std::string & text, int x, int y, int size)
{
	// Fill buffers
	vertices.clear();
	std::vector<glm::vec2> UVs;
	for (unsigned int i = 0; i < text.length(); i++) {

		glm::vec2 vertex_up_left = glm::vec2(x + i * size, y + size);
		glm::vec2 vertex_up_right = glm::vec2(x + i * size + size, y + size);
		glm::vec2 vertex_down_right = glm::vec2(x + i * size + size, y);
		glm::vec2 vertex_down_left = glm::vec2(x + i * size, y);

		// A square is transformed into two triangles
		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = (character / 16) / 16.0f;

		glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
		glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));
		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}

	glBindVertexArray(vao);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, text2DVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, text2DUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Unbind VAO so no one can change it
	glBindVertexArray(0);
}
