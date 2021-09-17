#include "shader.h"

#include <glad/glad.h>

#include <fmt/format.h>

bool Shader::compile(const char *source)
{
	m_handle = glCreateShader(get_opengl_shader_type());
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

Shader::~Shader()
{
	if (m_handle != 0)
	{
		glDeleteShader(m_handle);
		m_handle = 0;
	}
}

unsigned int VertexShader::get_opengl_shader_type() const
{
	return GL_VERTEX_SHADER;
}

unsigned int FragmentShader::get_opengl_shader_type() const
{
	return GL_FRAGMENT_SHADER;
}

ShaderProgram::ShaderProgram()
{
	m_handle = glCreateProgram();
}

void ShaderProgram::attach(const Shader &shader)
{
	glAttachShader(m_handle, shader.get_handle());
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
