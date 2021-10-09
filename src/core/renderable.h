#pragma once

#include "node.h"

class Camera;

class Renderable : public Node
{
public:
  virtual ~Renderable() = default;

  virtual void render(Camera *camera) const = 0;
  virtual bool is_visible() const = 0;
};
