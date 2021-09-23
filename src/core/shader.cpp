#include "shader.h"

#include <fmt/format.h>

#include <glad/glad.h>

template<>
struct fmt::formatter<Shader::Type>
{
  constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto format(const Shader::Type type, FormatContext &ctx) -> decltype(ctx.out())
  {
    switch (type) {
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
  if (!success) {
    GLint length = 0;
    glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length);
    m_log.resize(length);
    glGetShaderInfoLog(m_handle, length, nullptr, m_log.data());
  }
  if (print_log) {
    fmt::print("{} shader compilation: {}\n", m_type, success ? "Success" : "Failed");
    if (!success) {
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
  if (m_handle != 0) {
    glDeleteShader(m_handle);
    m_handle = 0;
  }
}

unsigned int Shader::type_to_gl(Type type)
{
  switch (type) {
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
  if (!success) {
    char log[512];
    glGetProgramInfoLog(m_handle, 512, nullptr, log);
    fmt::print(stderr, "Shader Program link error: {}\n", log);
  }

  GLint max_length = 0;
  glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);
  std::string buf{};

  GLint count = 0;
  glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count);
  for (GLint i = 0; i < count; i++) {
    GLint size = 0;
    GLenum type = 0;
    GLsizei length = 0;
    buf.resize(max_length);
    glGetActiveUniform(m_handle, static_cast<GLuint>(i), max_length, &length, &size, &type, buf.data());
    buf.resize(length);
    fmt::print("max: {}, length: {}, size: {}, type: {}, name: {}\n", max_length, length, size, type, buf);
    auto location = glGetUniformLocation(m_handle, buf.data());
    m_uniforms[buf.substr(0, buf.find('['))] = Uniform{ type, size, location };
  }

	for (const auto &[key, val] : m_uniforms)
	{
		fmt::print("{} {} {} {}\n", key, val.location, val.size, val.type);
	}
  return success;
}

void ShaderProgram::use() const
{
  glUseProgram(m_handle);
}


bool ShaderProgram::set(std::string_view name, GLsizei count, const float *value) const
{
  auto iter = m_uniforms.find(name.data());
  if (iter != m_uniforms.end()) {
    auto &u = iter->second;
    glUniformMatrix4fv(u.location, count, GL_FALSE, value);
  }
  return true;
}

// GL_FLOAT
// GL_FLOAT_VEC2
// GL_FLOAT_VEC3
// GL_FLOAT_VEC4
// GL_DOUBLE
// GL_DOUBLE_VEC2
// GL_DOUBLE_VEC3
// GL_DOUBLE_VEC4
// GL_INT
// GL_INT_VEC2
// GL_INT_VEC3
// GL_INT_VEC4
// GL_UNSIGNED_INT 
// GL_UNSIGNED_INT_VEC2
// GL_UNSIGNED_INT_VEC3
// GL_UNSIGNED_INT_VEC4
// GL_BOOL
// GL_BOOL_VEC2
// GL_BOOL_VEC3
// GL_BOOL_VEC4
// GL_FLOAT_MAT2
// GL_FLOAT_MAT3
// GL_FLOAT_MAT4
// GL_FLOAT_MAT2x3
// GL_FLOAT_MAT2x4
// GL_FLOAT_MAT3x2
// GL_FLOAT_MAT3x4
// GL_FLOAT_MAT4x2
// GL_FLOAT_MAT4x3
// GL_DOUBLE_MAT2
// GL_DOUBLE_MAT3
// GL_DOUBLE_MAT4
// GL_DOUBLE_MAT2x3
// GL_DOUBLE_MAT2x4
// GL_DOUBLE_MAT3x2
// GL_DOUBLE_MAT3x4
// GL_DOUBLE_MAT4x2
// GL_DOUBLE_MAT4x3
// GL_SAMPLER_1D
// GL_SAMPLER_2D
// GL_SAMPLER_3D
// GL_SAMPLER_CUBE
// GL_SAMPLER_1D_SHADOW
// GL_SAMPLER_2D_SHADOW
// GL_SAMPLER_1D_ARRAY
// GL_SAMPLER_2D_ARRAY
// GL_SAMPLER_1D_ARRAY_SHADOW
// GL_SAMPLER_2D_ARRAY_SHADOW
// GL_SAMPLER_2D_MULTISAMPLE
// GL_SAMPLER_2D_MULTISAMPLE_ARRAY
// GL_SAMPLER_CUBE_SHADOW
// GL_SAMPLER_BUFFER
// GL_SAMPLER_2D_RECT
// GL_SAMPLER_2D_RECT_SHADOW
// GL_INT_SAMPLER_1D
// GL_INT_SAMPLER_2D
// GL_INT_SAMPLER_3D
// GL_INT_SAMPLER_CUBE
// GL_INT_SAMPLER_1D_ARRAY
// GL_INT_SAMPLER_2D_ARRAY
// GL_INT_SAMPLER_2D_MULTISAMPLE
// GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY
// GL_INT_SAMPLER_BUFFER
// GL_INT_SAMPLER_2D_RECT
// GL_UNSIGNED_INT_SAMPLER_1D
// GL_UNSIGNED_INT_SAMPLER_2D
// GL_UNSIGNED_INT_SAMPLER_3D
// GL_UNSIGNED_INT_SAMPLER_CUBE
// GL_UNSIGNED_INT_SAMPLER_1D_ARRAY
// GL_UNSIGNED_INT_SAMPLER_2D_ARRAY
// GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE
// GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY
// GL_UNSIGNED_INT_SAMPLER_BUFFER
// GL_UNSIGNED_INT_SAMPLER_2D_RECT
// GL_IMAGE_1D
// GL_IMAGE_2D
// GL_IMAGE_3D
// GL_IMAGE_2D_RECT
// GL_IMAGE_CUBE
// GL_IMAGE_BUFFER
// GL_IMAGE_1D_ARRAY
// GL_IMAGE_2D_ARRAY
// GL_IMAGE_2D_MULTISAMPLE
// GL_IMAGE_2D_MULTISAMPLE_ARRAY
// GL_INT_IMAGE_1D
// GL_INT_IMAGE_2D
// GL_INT_IMAGE_3D
// GL_INT_IMAGE_2D_RECT
// GL_INT_IMAGE_CUBE
// GL_INT_IMAGE_BUFFER
// GL_INT_IMAGE_1D_ARRAY
// GL_INT_IMAGE_2D_ARRAY
// GL_INT_IMAGE_2D_MULTISAMPLE
// GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY
// GL_UNSIGNED_INT_IMAGE_1D
// GL_UNSIGNED_INT_IMAGE_2D
// GL_UNSIGNED_INT_IMAGE_3D
// GL_UNSIGNED_INT_IMAGE_2D_RECT
// GL_UNSIGNED_INT_IMAGE_CUBE
// GL_UNSIGNED_INT_IMAGE_BUFFER
// GL_UNSIGNED_INT_IMAGE_1D_ARRAY
// GL_UNSIGNED_INT_IMAGE_2D_ARRAY
// GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE
// GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY
// GL_UNSIGNED_INT_ATOMIC_COUNTER