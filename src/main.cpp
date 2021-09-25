#include "core/window.h"
#include "core/importer.h"
#include "core/staticmesh.h"
#include "core/shader.h"


#include <fmt/format.h>

#include <memory>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include <sstream>

class Switch
{
public:
  bool toggle()
  {
    return m_state = !m_state;
  }

private:
  bool m_state = false;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  Window window{};
  Switch sw{};
  Importer importer{};

  std::ifstream vx_shader_source("shader/test_vertex.glsl");
  std::stringstream vx_shader;
  vx_shader << vx_shader_source.rdbuf();

  std::ifstream fg_shader_source("shader/test_fragment.glsl");
  std::stringstream fg_shader;
  fg_shader << fg_shader_source.rdbuf();

  auto vertex = std::make_shared<Shader>(Shader::Type::Vertex, vx_shader.str().data());
  auto fragment = std::make_shared<Shader>(Shader::Type::Fragment, fg_shader.str().data());
  auto prog = std::make_shared<ShaderProgram>(vertex, fragment);
  auto mesh = importer.load_static_mesh("assets/cube.fbx");
  auto mesh2 = importer.load_static_mesh("assets/cube.fbx");
  auto mesh3 = importer.load_static_mesh("assets/cube.fbx");
  auto mesh4 = importer.load_static_mesh("assets/cube.fbx");
  mesh.set_shader(prog);
  mesh2.set_shader(prog);
  mesh3.set_shader(prog);
  mesh4.set_shader(prog);


  float angle = 0.f;
  while (!window.should_close()) {
    mesh.set_rotation(glm::vec3{ angle, 0.f, 0.f });
    mesh2.set_rotation(glm::vec3{ 0.f, angle, 0.f });

    mesh2.set_position(glm::vec3{ glm::sin(angle), glm::cos(angle), 0.f } * 3.f);
    angle += 0.01f;
    window.clear();
    mesh.render();
    mesh2.render();
    window.refresh();
  }
}
