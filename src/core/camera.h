#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
  enum class Projection {
    Ortographic,
    Perspective
  };

  void set_projection(Projection projection);
  void set_tracking(bool enable);
  void set_position(glm::vec3 position);
  void set_direction(glm::vec3 direction);
  void set_target(glm::vec3 target);
  void add_pitch(float amount);
  void add_yaw(float amount);

  const glm::mat4 &get_view_matrix() const;
  void update_view_matrix();

private:
  glm::mat4 get_projection_matrix();


  bool m_tracking = false;
  float m_aspect = 16.f / 9.f;
  float m_fov = 70.f;
  float m_ysize = 100.f;
  float m_xsize = 100.f;
  float m_near = 0.f;
  float m_far = 0.f;

  Projection m_projection = Projection::Perspective;
  glm::vec3 m_position{};
  glm::vec3 m_direction = FORWARD;
  glm::vec3 m_up = UP;
  glm::vec3 m_forward = FORWARD;
  glm::vec3 m_right = RIGHT;
  glm::mat4 m_view{};


public:
  static const glm::vec3 UP;
  static const glm::vec3 FORWARD;
  static const glm::vec3 RIGHT;
};
