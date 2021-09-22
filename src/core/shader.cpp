#include "shader.h"

#include <fmt/format.h>

#include <glad/glad.h>

template <>
struct fmt::formatter<Shader::Type>
{
	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
	{
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const Shader::Type type, FormatContext &ctx) -> decltype(ctx.out())
	{
		switch (type)
		{
		case Shader::Type::Fragment:
			return format_to(ctx.out(), "Fragment");
		case Shader::Type::Vertex:
			return format_to(ctx.out(), "Vertex");
		}
		return ctx.out();
	}
};

bool Shader::compile(const char *source, bool print_log)
{
	glShaderSource(m_handle, 1, &source, nullptr);
	glCompileShader(m_handle);
	GLint success = 0;
	glGetShaderiv(m_handle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint length = 0;
		glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length);
		m_log.reserve(length);
		glGetShaderInfoLog(m_handle, length, nullptr, m_log.data());
	}
	if (print_log)
	{
		fmt::print("{} shader compilation: {}\n", m_type, success ? "Success" : "Failed");
		if(!success){
			fmt::print("Reason: {}\n", m_log);
		}
	}

	return success;
}

Shader::Shader(Shader::Type type)
{
	m_type = type;
	m_handle = glCreateShader(type_to_gl(type));
}

Shader::Shader(Type type, const char *source, bool print_log)
	: Shader(type)
{
	compile(source, print_log);
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
