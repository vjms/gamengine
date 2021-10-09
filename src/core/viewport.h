#pragma once

#include <memory>

#include "camera.h"
#include "object.h"
#include "renderable.h"
#include "node.h"

class Viewport : public Renderable
{
public:
  virtual void render() const override;
  virtual bool is_visible() const override;

private:
  std::shared_ptr<Camera> m_camera = std::make_shared<Camera>();
};
