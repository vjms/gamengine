#pragma once

#include "node.h"

class Renderable : public Node
{
public:
  virtual ~Renderable() = default;

  virtual void render() const = 0;
  virtual bool is_visible() const = 0;
};
