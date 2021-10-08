#include "camera.h"

#include <glm/gtx/transform.hpp>


const glm::vec3 Camera::UP{ 0.f, 1.f, 0.f };
const glm::vec3 Camera::FORWARD{ 1.f, 0.f, 0.f };
const glm::vec3 Camera::RIGHT{ 0.f, 0.f, 1.f };


const glm::mat4 &Camera::get_view_matrix() const
{
  return m_view;
}


void Camera::set_projection(Projection projection)
{
  m_projection = projection;
}
glm::mat4 Camera::get_projection_matrix()
{
  switch (m_projection) {
  case Projection::Ortographic: return glm::ortho(m_xsize * -0.5f, m_xsize * 0.5f, m_ysize * -0.5f, m_ysize * 0.5f, m_near, m_far);
  case Projection::Perspective: return glm::perspective(m_fov, m_aspect, m_near, m_far);
  }
  return glm::mat4{};
}

void Camera::set_tracking(bool enable)
{
  m_tracking = enable;
}
void Camera::set_position(glm::vec3 position)
{
  m_position = position;
}
void Camera::set_direction(glm::vec3 direction)
{
  m_direction = direction;
}
void Camera::set_target(glm::vec3 target)
{
  m_direction = glm::normalize(target - m_position);
}
void Camera::add_pitch([[maybe_unused]] float amount)
{
}
void Camera::add_yaw([[maybe_unused]] float amount)
{
}

void Camera::update_view_matrix()
{
  m_view = glm::lookAt(m_position, m_direction, m_up);
  m_view *= get_projection_matrix();
}
