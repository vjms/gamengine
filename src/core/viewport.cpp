#include "viewport.h"

void Viewport::render() const
{
  for (auto &child : get_children_of_type<Renderable>()) {
    if (child->is_visible()) {
      child->render();
    }
  }
}

bool Viewport::is_visible() const
{
  return true;
}