#pragma once

#include <string>

class Shader
{
public:
	virtual ~Shader();
	Shader(const Shader &other) = default;
	Shader &operator=(const Shader &other) = default;

	bool compile(const char *source);

	unsigned int get_handle() const { return m_handle; };

protected:
	virtual unsigned int get_opengl_shader_type() const = 0;
	Shader() = default;

private:
	unsigned int m_handle = 0;
};

class FragmentShader : public Shader
{
public:
	FragmentShader() = default;

protected:
	unsigned int get_opengl_shader_type() const override;
};

class VertexShader : public Shader
{
public:
	VertexShader() = default;

protected:
	unsigned int get_opengl_shader_type() const override;
};

class ShaderProgram
{
public:
	ShaderProgram();

	void attach(const Shader &shader);
	bool link();

	unsigned int get_handle() const { return m_handle; }

private:
	unsigned int m_handle = 0;
};
