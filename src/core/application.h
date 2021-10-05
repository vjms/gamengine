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
    m_window.set_key_callback([&](const KeyEvent &key) { key_callback(key); });
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

protected:
  void key_callback(const KeyEvent &key)
  {
    fmt::print("{}: {}\n", m_name, key.key);
  }

  virtual void dowork_impl() = 0;
  Window m_window{};

private:
  std::string m_name{};
};