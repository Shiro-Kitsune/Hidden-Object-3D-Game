#include "Shader.h"
#include "FileUtil.h"
#include <glad/glad.h>
#include <string>
#include <iostream>

Shader::Shader()
	: m_compiled(false), m_program(0)
{
}

Shader::~Shader()
{
	if (m_compiled)
	{
		glDeleteProgram(m_program);
	}
}

void Shader::compile(const char * vsPath, const char * fsPath, const char* gsPath, const char* tcsPath, const char* tesPath)
{
	m_compiled = true;
	m_program = compileShader(vsPath, fsPath, gsPath, tcsPath, tesPath);
}

void Shader::bind() const
{
	if (m_compiled)
	{
		glUseProgram(m_program);
	}
}

void Shader::unbind()
{
	glUseProgram(0);
}

void Shader::bindUniform(const char * name, const glm::mat4 & mat) const
{
	GLint id = glGetUniformLocation(m_program, name);
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::bindUniform(const char * name, const glm::vec3 & vec) const
{
	GLint id = glGetUniformLocation(m_program, name);
	glUniform3fv(id, 1, glm::value_ptr(vec));
}

void Shader::bindUniform(const char * name, const glm::vec2 & vec) const
{
	GLint id = glGetUniformLocation(m_program, name);
	glUniform2fv(id, 1, glm::value_ptr(vec));
}

void Shader::bindUniform(const char * name, int value) const
{
	GLint id = glGetUniformLocation(m_program, name);
	glUniform1i(id, value);
}

void Shader::bindUniform(const char * name, unsigned int value) const
{
	GLint id = glGetUniformLocation(m_program, name);
	glUniform1ui(id, value);
}

void Shader::bindUniform(const char * name, float value) const
{
	GLint id = glGetUniformLocation(m_program, name);
	glUniform1f(id, value);
}

Shader::Shader(Shader && shader) noexcept
	: m_compiled(shader.m_compiled), m_program(shader.m_program)
{
}

Shader & Shader::operator=(Shader&& shader) noexcept
{
	if (this == &shader)
	{
		return *this;
	}

	m_compiled = shader.m_compiled;
	m_program = shader.m_program;

	return *this;
}

unsigned int Shader::compileShader(const char * vsPath, const char * fsPath, const char* gsPath, const char* tcsPath, const char* tesPath)
{
	//Read vertex shader
	std::string vsSource = FileUtil::loadFile(vsPath);
	const char* ccVs = vsSource.c_str();
	//Compile vertex shader
	GLuint vsShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsShaderId, 1, &ccVs, nullptr);
	glCompileShader(vsShaderId);
	//Check compile errors
	if (!checkCompileErrors(vsShaderId, "VERTEX"))
	{
		std::cout << "Vertex Shader Compiled Successfully" << std::endl;
	}

	//Read fragment shader
	std::string fsSource = FileUtil::loadFile(fsPath);
	const char* ccFs = fsSource.c_str();
	//Compile fragment shader
	GLuint fsShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsShaderId, 1, &ccFs, nullptr);
	glCompileShader(fsShaderId);
	//Check compile errors
	if (!checkCompileErrors(fsShaderId, "FRAGMENT"))
	{
		std::cout << "Fragment Shader Compiled Successfully" << std::endl;
	}

	GLuint gsShaderId;
	if (gsPath != nullptr)
	{
		//Read geometry shader
		std::string gsSource = FileUtil::loadFile(gsPath);
		const char* ccGs = gsSource.c_str();
		//Compile geometry shader
		gsShaderId = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gsShaderId, 1, &ccGs, nullptr);
		glCompileShader(gsShaderId);
		//Check compile errors
		if (!checkCompileErrors(gsShaderId, "GEOMETRY"))
		{
			std::cout << "Geometry Shader Compiled Successfully" << std::endl;
		}
	}

	GLuint tcsShaderId;
	if (tcsPath != nullptr)
	{
		//Read tesselation control shader
		std::string tcsSource = FileUtil::loadFile(tcsPath);
		const char* ccTcs = tcsSource.c_str();
		//Compile tesselation control shader
		tcsShaderId = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tcsShaderId, 1, &ccTcs, nullptr);
		glCompileShader(tcsShaderId);
		//Check compile errors
		if (!checkCompileErrors(tcsShaderId, "TESSELATION CONTROL"))
		{
			std::cout << "Tesselation Control Shader Compiled Successfully" << std::endl;
		}
	}

	GLuint tesShaderId;
	if (tesPath != nullptr)
	{
		//Read tesselation evaluation shader
		std::string tesSource = FileUtil::loadFile(tesPath);
		const char* ccTes = tesSource.c_str();
		//Compile tesselation evaluation shader
		tesShaderId = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tesShaderId, 1, &ccTes, nullptr);
		glCompileShader(tesShaderId);
		//Check compile errors
		if (!checkCompileErrors(tesShaderId, "TESSELATION EVALUATION"))
		{
			std::cout << "Tesselation Evaluation Shader Compiled Successfully" << std::endl;
		}
	}

	//Create and link shader program
	GLuint program = glCreateProgram();
	glAttachShader(program, vsShaderId);
	glAttachShader(program, fsShaderId);
	if (gsPath != nullptr)
		glAttachShader(program, gsShaderId);
	if (tcsPath != nullptr)
		glAttachShader(program, tcsShaderId);
	if (tesPath != nullptr)
		glAttachShader(program, tesShaderId);
	glLinkProgram(program);
	checkCompileErrors(program, "PROGRAM");

	//Opengl has linked program so we can delete the copy
	glDeleteProgram(vsShaderId);
	glDeleteProgram(fsShaderId);
	if (gsPath != nullptr)
		glDeleteProgram(gsShaderId);
	if (tcsPath != nullptr)
		glDeleteProgram(tcsShaderId);
	if (tesPath != nullptr)
		glDeleteProgram(tesShaderId);

	return program;
}

bool Shader::checkCompileErrors(unsigned int shader, const char* type)
{
	int logLength;
	if (strcmp(type, "PROGRAM") != 0)
	{
		//Get error message from shader compilation
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength != 0)
		{
			char* logMessage = static_cast<char*>(malloc(static_cast<size_t>(logLength))); // reserve the space upfront for the error message
			glGetShaderInfoLog(shader, logLength, NULL, logMessage);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << logMessage << std::endl;
			delete logMessage;
			return true;
		}
	}
	else
	{
		//Get error message from program compilation
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength != 0)
		{
			char* logMessage = static_cast<char*>(malloc(static_cast<size_t>(logLength))); // reserve the space upfront for the error message
			glGetProgramInfoLog(shader, logLength, NULL, logMessage);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << logMessage << std::endl;
			delete logMessage;
			return true;
		}
	}
	return false;
}
