#include "Model2D.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

Model2D::Model2D(const std::string & texturePath, int x, int y, int size)
{
	textureID = loadTextureFromFile(texturePath.c_str());
	setupMesh(x, y, size);
}

Model2D::~Model2D()
{
	glDeleteVertexArrays(1, &vao);
	// Delete buffers
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &uvBufferID);

	// Delete texture
	glDeleteTextures(1, &textureID);
}

void Model2D::render(const Shader & shader) const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(vao);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

	glDisable(GL_BLEND);
}

void Model2D::setupMesh(int x, int y, int size)
{
	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	
	glm::vec2 vertex_up_left = glm::vec2(x, y + size);
	glm::vec2 vertex_up_right = glm::vec2(x + size, y + size);
	glm::vec2 vertex_down_right = glm::vec2(x + size, y);
	glm::vec2 vertex_down_left = glm::vec2(x, y);
	
	// A square is transformed into two triangles
	vertices.push_back(vertex_up_left);
	vertices.push_back(vertex_down_left);
	vertices.push_back(vertex_up_right);
	
	vertices.push_back(vertex_down_right);
	vertices.push_back(vertex_up_right);
	vertices.push_back(vertex_down_left);
	
	glm::vec2 uv_up_left = glm::vec2(0.0f, 0.0f);
	glm::vec2 uv_up_right = glm::vec2(1.0f, 0.0f);
	glm::vec2 uv_down_right = glm::vec2(1.0f, 1.0f);
	glm::vec2 uv_down_left = glm::vec2(0.0f, 1.0f);
	UVs.push_back(uv_up_left);
	UVs.push_back(uv_down_left);
	UVs.push_back(uv_up_right);
	
	UVs.push_back(uv_down_right);
	UVs.push_back(uv_up_right);
	UVs.push_back(uv_down_left);

	glGenVertexArrays(1, &vao);
	// Initialize VBO
	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &uvBufferID);

	glBindVertexArray(vao);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Unbind VAO so no one can change it
	glBindVertexArray(0);
}
