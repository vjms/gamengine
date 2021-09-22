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
	mesh.set_shader(prog);

	while (!window.should_close())
	{
		sw.toggle() ? window.clear() : mesh.render();
		window.refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
