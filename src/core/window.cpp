#include "window.h"

#include <fmt/format.h>

#include <mutex>

int Window::s_active_windows = 0;
bool Window::s_glfw_initialized = false;

bool Window::glfw_initialize()
{
  if (s_glfw_initialized) {
    return true;
  }
  if (!glfwInit()) {
    fmt::print(stderr, "GLFW initialization failed\n");
    return false;
  }
  glfwSetErrorCallback(Window::error_callback);
  s_glfw_initialized = true;
  return true;
}

Window::Window()
{
  if (glfw_initialize()) {
    s_active_windows += 1;
    m_window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
    set_as_active();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      fmt::print(stderr, "GLAD initialization failed\n");
    }
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, Window::key_callback);
    glfwSetMouseButtonCallback(m_window, Window::mouse_button_callback);
    glfwSetCursorPosCallback(m_window, Window::cursor_position_callback);
    glfwSetCursorEnterCallback(m_window, Window::cursor_enter_callback);
    glfwSetFramebufferSizeCallback(m_window, Window::frame_buffer_size_callback);
  }
}

Window::~Window()
{
  if (is_open()) {
    close();
  }
  if (s_active_windows == 0) {
    glfwTerminate();
    s_glfw_initialized = false;
  }
}

bool Window::is_open() const
{
  return m_window != nullptr;
}

bool Window::should_close() const
{
  return glfwWindowShouldClose(m_window);
}

void Window::close()
{
  glfwDestroyWindow(m_window);
  m_window = nullptr;
  s_active_windows -= 1;
}

void Window::clear() const
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::refresh() const
{
  swap_buffers();
  poll_events();
}

void Window::swap_buffers() const
{
  glEnable(GL_DEPTH_TEST);
  glfwSwapBuffers(m_window);
}

void Window::poll_events() const
{
  glfwPollEvents();
}

void Window::set_as_active() const
{
  glfwMakeContextCurrent(m_window);
}

bool Window::is_active() const
{
  return glfwGetCurrentContext() == m_window;
}


// OpenGL Callbacks

void Window::error_callback(int error, const char *description)
{
  fmt::print(stderr, "OpenGL Error {}: {}\n", error, description);
}

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  auto custom = static_cast<Window *>(glfwGetWindowUserPointer(window));
  KeyEvent event{ key, scancode, action, mods };
  custom->key_event_dispatcher.process(event);
}

void Window::mouse_button_callback(GLFWwindow *window, [[maybe_unused]] int button, [[maybe_unused]] int action, [[maybe_unused]] int mods)
{
  static_cast<Window *>(glfwGetWindowUserPointer(window));
}

void Window::cursor_position_callback(GLFWwindow *window, [[maybe_unused]] double xpos, [[maybe_unused]] double ypos)
{
  static_cast<Window *>(glfwGetWindowUserPointer(window));
}

void Window::cursor_enter_callback(GLFWwindow *window, [[maybe_unused]] int entered)
{
  static_cast<Window *>(glfwGetWindowUserPointer(window));
}


void Window::frame_buffer_size_callback(GLFWwindow *window, int width, int height)
{
  auto custom = static_cast<Window *>(glfwGetWindowUserPointer(window));
  auto current = glfwGetCurrentContext();
  int old_height, old_width;
  glfwGetFramebufferSize(custom->m_window, &old_width, &old_height);
  glfwMakeContextCurrent(custom->m_window);
  glViewport(0, 0, width, height);
  glfwMakeContextCurrent(current);
  WindowResizeEvent event{ width, height, old_width, old_height };
  custom->window_resize_event_dispatcher.process(event);
}