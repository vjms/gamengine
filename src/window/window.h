#ifndef _WINDOW_H
#define _WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>

/**
 * Window to render stuff into.
 * Handles OpenGL initialization and is an OpenGL context.
 * 
 * Only use on the main thread.
*/
class Window
{
private:
	GLFWwindow *m_window = nullptr;

public:
	Window();
	~Window();
	Window(const Window &other) = delete;
	Window &operator=(const Window &other) = delete;

	[[nodiscard]] bool is_open() const;
	[[nodiscard]] bool should_close() const;
	// Closes the window. Explicit calling not necessary, as the destructor calls this when needed.
	void close();

	void swap_buffers();
	void poll_events();
	void make_context_current();
	[[nodiscard]] bool is_current_context() const;

private:
	// Count the number of active windows as they are created and destroyed,
	// and when it reaches 0, call glfwterminate from the destructor
	static int m_active_windows;

	// OpenGL Initialization
	static bool m_opengl_initialized;
	static bool glfw_initialize();

	// OpenGL Callbacks
	static void error_callback(int error, const char *description);
	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods );
	static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
	static void cursor_enter_callback(GLFWwindow *window, int entered);
};

#endif // _WINDOW_H
