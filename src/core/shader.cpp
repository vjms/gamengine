#include "shader.h"

#include <fmt/format.h>

#include <glad/glad.h>

bool Shader::compile(const char *source)
{
	glShaderSource(m_handle, 1, &source, nullptr);
	glCompileShader(m_handle);
	GLint success = 0;
	glGetShaderiv(m_handle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[512];
		glGetShaderInfoLog(m_handle, 512, nullptr, log);
		fmt::print(stderr, "Shader compilation failed.\n - Source: {}\n - Reason: {}\n", source, log);
	}
	return success;
}

Shader::Shader(Shader::Type type)
{
	m_handle = glCreateShader(type_to_gl(type));
}

Shader::Shader(Type type, const char *source)
	: Shader(type)
{
	compile(source);
}

Shader::~Shader()
{
	if (m_handle != 0)
	{
		glDeleteShader(m_handle);
		m_handle = 0;
	}
}

unsigned int Shader::type_to_gl(Type type)
{
	switch (type)
	{
	case Type::Vertex:
		return GL_VERTEX_SHADER;
	case Type::Fragment:
		return GL_FRAGMENT_SHADER;
	}
	return 0;
}

ShaderProgram::ShaderProgram()
{
	m_handle = glCreateProgram();
}

ShaderProgram::ShaderProgram(const std::shared_ptr<Shader> &vertex_shader, const std::shared_ptr<Shader> &fragment_shader) : ShaderProgram()
{
	attach(vertex_shader);
	attach(fragment_shader);
	link();
}

void ShaderProgram::attach(const std::shared_ptr<Shader> &shader)
{
	m_shaders.emplace_back(shader);
	glAttachShader(m_handle, shader->get_handle());
}

bool ShaderProgram::link()
{
	glLinkProgram(m_handle);
	GLint success = 0;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
	if (!success)
	{
		char log[512];
		glGetProgramInfoLog(m_handle, 512, nullptr, log);
		fmt::print(stderr, "Shader Program link error: {}\n", log);
	}
	return success;
}
