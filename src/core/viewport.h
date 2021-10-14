#pragma once

#include <memory>

#include "camera.h"
#include "object.h"
#include "renderable.h"
#include "node.h"

#include "window_resize_event.h"

class Viewport : public Renderable
  , public EventListener<WindowResizeEvent>
{
public:
  Viewport();
  virtual ~Viewport() = default;

  virtual void render(Camera *camera) const override;
  virtual bool is_visible() const override;

  virtual void process(WindowResizeEvent &event) override;
  std::shared_ptr<Camera> get_camera() { return m_camera; }

private:
  std::shared_ptr<Camera> m_camera = std::make_shared<Camera>();

  int left = 0;
  int right = 0;
  int up = 0;
  int down = 0;
};
