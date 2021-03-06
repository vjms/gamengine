#include "viewport.h"


Viewport::Viewport()
{
  m_camera->update_projection();
}

void Viewport::render([[maybe_unused]] Camera *camera) const
{
  m_camera->update_view();
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
  m_camera->update_projection();
}
