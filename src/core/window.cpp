#include "window.h"

#include <fmt/format.h>

#include <mutex>

int Window::s_active_windows = 0;
bool Window::s_opengl_initialized = false;

bool Window::glfw_initialize()
{
  if (s_opengl_initialized) {
    return true;
  }
  if (!glfwInit()) {
    fmt::print(stderr, "GLFW initialization failed\n");
    return false;
  }
  glfwSetErrorCallback(Window::error_callback);
  s_opengl_initialized = true;
  return true;
}

Window::Window()
{
  if (glfw_initialize()) {
    s_active_windows += 1;
    m_window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
    make_context_current();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      fmt::print(stderr, "GLAD initialization failed\n");
    }
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, Window::key_callback);
    glfwSetMouseButtonCallback(m_window, Window::mouse_button_callback);
    glfwSetCursorPosCallback(m_window, Window::cursor_position_callback);
    glfwSetCursorEnterCallback(m_window, Window::cursor_enter_callback);
  }
}

Window::~Window()
{
  if (is_open()) {
    close();
  }
  if (s_active_windows == 0) {
    glfwTerminate();
    s_opengl_initialized = false;
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

void Window::make_context_current() const
{
  glfwMakeContextCurrent(m_window);
}

bool Window::is_current_context() const
{
  return glfwGetCurrentContext() == m_window;
}

void Window::set_key_callback(std::function<void(const KeyEvent &)> kcb)
{
  m_key_callback = kcb;
}


// OpenGL Callbacks

void Window::error_callback(int error, const char *description)
{
  fmt::print(stderr, "OpenGL Error {}: {}\n", error, description);
}

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  auto custom = static_cast<Window *>(glfwGetWindowUserPointer(window));
  custom->m_key_callback(
    KeyEvent{
      static_cast<KeyEvent::Key>(key),
      scancode,
      static_cast<KeyEvent::Action>(action),
      static_cast<KeyEvent::Mod>(mods) });


  // GLFW_PRESS
  // GLFW_RELEASE
  // GLFW_REPEAT
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