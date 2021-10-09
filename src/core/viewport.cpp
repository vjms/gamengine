#include "viewport.h"

void Viewport::render([[maybe_unused]]Camera *camera) const
{
  m_camera->update();
  for (auto &child : get_children_of_type<Renderable>()) {
    if (child->is_visible()) {
      child->render(m_camera.get());
    }
  }
}

bool Viewport::is_visible() const
{
  return true;
}


void Viewport::process(WindowResizeEvent &event)
{
  m_camera->set_aspect_ratio(static_cast<float>(event.width) / static_cast<float>(event.height));
}
