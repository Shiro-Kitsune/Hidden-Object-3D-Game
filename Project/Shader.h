#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader();
	~Shader();
	// Link and compile shaders, only vertex and fragment shaders are mandatory
	void compile(const char* vs_path, const char* fs_path, const char* gs_path = nullptr, const char* tcs_path = nullptr, const char* tes_path = nullptr);
	// Use this shader program for rendering
	void bind() const;
	// Set current in-use shader program to default (none) for rendering
	static void unbind();
	// Overloaded functions to bind different types of uniforms
	void bindUniform(const char* name, const glm::mat4& mat) const;
	void bindUniform(const char* name, const glm::vec3& vec) const;
	void bindUniform(const char* name, const glm::vec2& vec) const;
	void bindUniform(const char* name, int value) const;
	void bindUniform(const char* name, unsigned int value) const;
	void bindUniform(const char* name, float value) const;

	Shader(const Shader& shader) = delete;
	Shader(Shader&& shader) noexcept;
	Shader& operator=(const Shader& shader) = delete;
	Shader& operator=(Shader&& shader) noexcept;
protected:
	static unsigned int compileShader(const char* vs_path, const char* fs_path, const char* gs_path, const char* tcs_path, const char* tes_path);
	// Utility method that checks and prints compile errors if any for shaders and program itself
	static bool checkCompileErrors(unsigned int shader, const char* type);
private:
	bool m_compiled;
	unsigned int m_program;
};