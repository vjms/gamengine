#pragma once

#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <concepts>
#include <type_traits>
#include <map>

#include <glad/glad.h>

class Shader
{
public:
  enum class Type {
    Vertex,
    Fragment
  };

  Shader(Type type);
  Shader(Type type, const char *source, bool print_log = true);
  ~Shader();
  Shader(const Shader &other) = default;
  Shader &operator=(const Shader &other) = default;

  bool compile(const char *source, bool print_log = true);

  unsigned int get_handle() const { return m_handle; };
  const std::string &get_compile_log() const { return m_log; }
  Type get_type() const { return m_type; }

private:
  unsigned int m_handle = 0;
  std::string m_log;
  Type m_type;

  unsigned int type_to_gl(Type type);
};


template<typename T>
concept NotPointerType = !std::is_pointer_v<T>;

template<typename T>
concept IntegralSigned = NotPointerType<T> && std::is_integral_v<T> && std::is_signed_v<T>;

template<typename T>
concept IntegralUnsigned = NotPointerType<T> && std::is_integral_v<T> && !std::is_signed_v<T>;

template<typename T>
concept FloatingPoint = NotPointerType<T> && std::is_floating_point_v<T>;


class ShaderProgram
{
public:
  ShaderProgram();
  ShaderProgram(const std::shared_ptr<Shader> &vertex_shader, const std::shared_ptr<Shader> &fragment_shader);

  void attach(const std::shared_ptr<Shader> &shader);
  bool link();

  void use() const;


  // Used to set multiple values, e.g. a vec3
  template<IntegralSigned... T>
  bool set(std::string_view name, T... args) const;
  // Used to set multiple values, e.g. a vec3
  template<IntegralUnsigned... T>
  bool set(std::string_view name, T... args) const;
  // Used to set multiple values, e.g. a vec3
  template<FloatingPoint... T>
  bool set(std::string_view name, T... args) const;


  // Used to set arrays
  template<typename T>
  bool set(std::string_view name, int count, T *value) const;
  // Used to set matrices
  bool set(std::string_view name, int count, bool transpose, const float *value) const;

  unsigned int get_handle() const { return m_handle; }

private:
  unsigned int m_handle = 0;
  std::vector<std::shared_ptr<Shader>> m_shaders;

  struct Uniform
  {
    unsigned int type;
    int size;
    int location;
  };
  std::map<const std::string, Uniform> m_uniforms;
};


template<IntegralSigned... T>
bool ShaderProgram::set(std::string_view name, T... args) const
{
  auto iter = m_uniforms.find(name.data());
  if (iter == m_uniforms.end()) {
    return false;
  }
  auto &u = iter->second;
  if constexpr (sizeof...(T) == 1) {
    glUniform1i(u.location, args...);
  }
  if constexpr (sizeof...(T) == 2) {
    glUniform2i(u.location, args...);
  }
  if constexpr (sizeof...(T) == 3) {
    glUniform3i(u.location, args...);
  }
  if constexpr (sizeof...(T) == 4) {
    glUniform4i(u.location, args...);
  }
  return true;
}

template<IntegralUnsigned... T>
bool ShaderProgram::set(std::string_view name, T... args) const
{
  auto iter = m_uniforms.find(name.data());
  if (iter == m_uniforms.end()) {
    return false;
  }
  auto &u = iter->second;
  if constexpr (sizeof...(T) == 1) {
    glUniform1ui(u.location, args...);
  }
  if constexpr (sizeof...(T) == 2) {
    glUniform2ui(u.location, args...);
  }
  if constexpr (sizeof...(T) == 3) {
    glUniform3ui(u.location, args...);
  }
  if constexpr (sizeof...(T) == 4) {
    glUniform4ui(u.location, args...);
  }
  return true;
}

template<FloatingPoint... T>
bool ShaderProgram::set(std::string_view name, T... args) const
{
  auto iter = m_uniforms.find(name.data());
  if (iter == m_uniforms.end()) {
    return false;
  }
  auto &u = iter->second;
  if constexpr (sizeof...(T) == 1) {
    glUniform1f(u.location, args...);
  }
  if constexpr (sizeof...(T) == 2) {
    glUniform2f(u.location, args...);
  }
  if constexpr (sizeof...(T) == 3) {
    glUniform3f(u.location, args...);
  }
  if constexpr (sizeof...(T) == 4) {
    glUniform4f(u.location, args...);
  }
  return true;
}

template<typename T>
bool ShaderProgram::set(std::string_view name, int count, T *value) const
{
  auto iter = m_uniforms.find(name.data());
  if (iter == m_uniforms.end()) {
    return false;
  }
  auto &u = iter->second;
  if constexpr (std::is_floating_point_v<T>) {
    switch (u.type) {
    case GL_FLOAT: glUniform1fv(u.location, count, value); break;
    case GL_FLOAT_VEC2: glUniform2fv(u.location, count, value); break;
    case GL_FLOAT_VEC3: glUniform3fv(u.location, count, value); break;
    case GL_FLOAT_VEC4: glUniform4fv(u.location, count, value); break;
    default: return false;
    }
  }
  if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
    switch (u.type) {
    case GL_INT: glUniform1iv(u.location, count, value); break;
    case GL_INT_VEC2: glUniform2iv(u.location, count, value); break;
    case GL_INT_VEC3: glUniform3iv(u.location, count, value); break;
    case GL_INT_VEC4: glUniform4iv(u.location, count, value); break;
    default: return false;
    }
  }
  if constexpr (std::is_integral_v<T> && !std::is_signed_v<T>) {
    switch (u.type) {
    case GL_UNSIGNED_INT: glUniform1uiv(u.location, count, value); break;
    case GL_UNSIGNED_INT_VEC2: glUniform2uiv(u.location, count, value); break;
    case GL_UNSIGNED_INT_VEC3: glUniform3uiv(u.location, count, value); break;
    case GL_UNSIGNED_INT_VEC4: glUniform4uiv(u.location, count, value); break;
    default: return false;
    }
  }
  return true;
}
