#pragma once

#include <string>

#include <fmt/format.h>

#include "window.h"
#include "key_event.h"
#include "viewport.h"

class Application
{
public:
  Application()
  {
    m_window.set_as_active();
    m_window.key_event_dispatcher.subscribe(&key_event_dispatcher);
    m_window.window_resize_event_dispatcher.subscribe(&m_viewport);
  }
  void handle()
  {
    m_window.set_as_active();
    m_window.clear();
    dowork_impl();
    m_window.refresh();
  }

  void set_name(const std::string &name) { m_name = name; }

  [[nodiscard]] virtual bool should_close() const { return m_window.should_close(); }

  EventDispatcher<KeyEvent> key_event_dispatcher{};

protected:
  virtual void dowork_impl() = 0;
  Window m_window{};
  Viewport m_viewport{};

private:
  std::string m_name{};
};