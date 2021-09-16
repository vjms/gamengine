#include "core/window.h"
#include "core/importer.h"
#include "core/staticmesh.h"

#include <memory>
#include <vector>

#include <fmt/format.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
	load_static_mesh("assets/cube.fbx");

	Window window{};
	while (!window.should_close())
	{
		window.swap_buffers();
		window.poll_events();
	}
}
