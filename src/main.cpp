


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fmt/format.h>

#include <chrono>

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
	auto window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetErrorCallback(error_callback);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		fmt::print("GLAD initialization failed\n");
		exit(1);
	}

	while (!glfwWindowShouldClose(window))
	{
		// auto begin = std::chrono::high_resolution_clock::now();
		glfwSwapBuffers(window);
		glfwPollEvents();
		// auto end = std::chrono::high_resolution_clock::now();
		//fmt::print("fps {}\n", 1000000 / static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()));
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}



