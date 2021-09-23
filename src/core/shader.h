#pragma once

#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <map>


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

class ShaderProgram
{
public:
  ShaderProgram();
  ShaderProgram(const std::shared_ptr<Shader> &vertex_shader, const std::shared_ptr<Shader> &fragment_shader);

  void attach(const std::shared_ptr<Shader> &shader);
  bool link();

  void use() const;

  bool set(std::string_view name, int count, const float *value) const;

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

