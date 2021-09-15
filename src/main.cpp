#include "window/window.h"

#include <memory>
#include <vector>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
	Window window{};
	while (!window.should_close())
	{
		window.swap_buffers();
		window.poll_events();
	}
}
