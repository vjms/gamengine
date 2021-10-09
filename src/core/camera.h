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
  void set_aspect_ratio(float aspect);
  void add_pitch(float amount);
  void add_yaw(float amount);

  const glm::mat4 &get_view_matrix() const;
  const glm::mat4 &get_projection_matrix() const;
  void update();

private:
  

  bool m_tracking = false;
  float m_aspect = 16.f / 9.f;
  float m_fov = 45.f;
  float m_ysize = 100.f;
  float m_xsize = 100.f;
  float m_near = 0.01f;
  float m_far = 100.f;

  Projection m_projection = Projection::Perspective;

  glm::vec3 m_position{};
  glm::vec3 m_direction{};
  glm::vec3 m_up = UP;
  glm::vec3 m_forward = FORWARD;
  glm::vec3 m_right = RIGHT;
  glm::mat4 m_view_matrix{};
  glm::mat4 m_projection_matrix{};


public:
  static const glm::vec3 UP;
  static const glm::vec3 FORWARD;
  static const glm::vec3 RIGHT;
};
