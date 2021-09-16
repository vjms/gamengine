#include "core/window.h"
#include "core/importer.h"
#include "core/staticmesh.h"

#include <memory>
#include <vector>

#include <fmt/format.h>

#include <chrono>
#include <thread>

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
	auto mesh = load_static_mesh("assets/cube.fbx");

	Window window{};

	Switch sw{};

	while (!window.should_close())
	{
		sw.toggle() ? window.clear() : mesh.render();
		window.refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
