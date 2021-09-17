#include "core/window.h"
#include "core/importer.h"
#include "core/staticmesh.h"
#include "core/shader.h"

#include <memory>
#include <vector>

#include <fmt/format.h>

#include <chrono>
#include <thread>

static const char *vertexsource = "#version 330 core\n"
								  "layout (location = 0) in vec3 aPos;\n"
								  "void main()\n"
								  "{\n"
								  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								  "}\0";
static const char *fragmentsource = "#version 330 core\n"
									"out vec4 FragColor;\n"
									"void main()\n"
									"{\n"
									"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
									"}\n\0";

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

	FragmentShader frag{};
	VertexShader vert{};
	auto prog = std::make_shared<ShaderProgram>();

	frag.compile(fragmentsource);
	vert.compile(vertexsource);
	prog->attach(frag);
	prog->attach(vert);
	prog->link();

	auto mesh = importer.load_static_mesh("assets/cube.fbx");
	mesh.set_shader(prog);

	while (!window.should_close())
	{
		sw.toggle() ? window.clear() : mesh.render();
		window.refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
