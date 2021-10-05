#include "core/application.h"
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


class Test : public Application
{
public:
  Test()
  {
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
    mesh = importer.load_static_mesh("assets/cube.fbx");
    mesh2 = importer.load_static_mesh("assets/cone.fbx");
    mesh.set_shader(prog);
    mesh2.set_shader(prog);
  }


  void dowork_impl() override
  {
    mesh.set_rotation(glm::vec3{ angle, 0.f, 0.f });
    mesh2.set_rotation(glm::vec3{ 0.f, angle, 0.f });

    mesh2.set_position(glm::vec3{ glm::sin(angle), glm::cos(angle), 0.f } * 3.f);
    angle += 0.01f;
    mesh.render();
    mesh2.render();
  }

private:
  float angle = 0.f;
  StaticMesh mesh;
  StaticMesh mesh2;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  Test app1{};

  while (!app1.should_close()) {
    app1.handle();
  }
}
