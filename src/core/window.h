#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <functional>

#include "event.h"
#include "key_event.h"

/**
 * Window to render stuff into.
 * Handles OpenGL initialization and is an OpenGL context.
 * 
 * Only use on the main thread.
*/
class Window
{
public:
  Window();
  virtual ~Window();
  Window(const Window &other) = delete;
  Window &operator=(const Window &other) = delete;

  [[nodiscard]] bool is_open() const;
  [[nodiscard]] bool should_close() const;
  // Closes the window. Explicit calling not necessary, as the destructor calls this when needed.
  void close();
  void clear() const;
  void refresh() const;
  void set_as_active() const;
  [[nodiscard]] bool is_active() const;


  EventDispatcher<KeyEvent> key_event_dispatcher{};


private:
  // GLFW window handle.
  GLFWwindow *m_window = nullptr;

  void swap_buffers() const;
  void poll_events() const;

private: /* Static functionality */
  // Count the number of active windows as they are created and destroyed,
  // and when it reaches 0, call glfwterminate from the destructor
  static int s_active_windows;

  // GLFW Initialization
  static bool s_glfw_initialized;
  static bool glfw_initialize();

  // GLFW Callbacks
  static void error_callback(int error, const char *description);
  static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
  static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
  static void cursor_enter_callback(GLFWwindow *window, int entered);
};
