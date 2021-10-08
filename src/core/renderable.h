#pragma once

class Renderable
{
public:
  virtual void render() const = 0;
  virtual bool is_visible() const = 0;
};
