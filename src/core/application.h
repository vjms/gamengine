#pragma once

#include <string>

#include <fmt/format.h>

#include "window.h"
#include "key_event.h"


class Application
{
public:
  Application()
  {
    m_window.set_as_active();
    m_window.key_event_dispatcher.subscribe(&key_event_dispatcher);
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

private:
  std::string m_name{};
};