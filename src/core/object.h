#pragma once

#include <string>


class Object
{
public:
  Object(std::string name = "") : m_name(name) {}

  const std::string &get_name() const { return m_name; }
  void set_name(std::string name) { m_name = name; }

private:
  std::string m_name;
};
