#pragma once

#include "Model.h"

// Class that represents plane model for rendering 2D images
class Model2D : public Model
{
public:
	// Create plane mesh to render texture at texturePath with left bottom corner at x,y and of passed size
	Model2D(const std::string& texturePath, int x, int y, int size);
	~Model2D();
	void render(const Shader& shader) const override;
private:
	unsigned int textureID;           // Texture
	unsigned int vao;
	unsigned int vertexBufferID;      // Buffer containing the vertices
	unsigned int uvBufferID;          // Buffer containing UVs

	// Create plane mesh with passed size and using x and y as starting position
	void setupMesh(int x, int y, int size);
};