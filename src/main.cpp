#include <GLFW/glfw3.h>

#include <fmt/format.h>

void error_callback(int error, const char *description)
{
	fmt::print(stderr, "Error Code{}: {}", error, description);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
	if (!glfwInit())
	{
		fmt::print("GLFW initialization failed\n");
		exit(1);
	}
	glfwSetErrorCallback(error_callback);
	auto window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
