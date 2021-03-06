#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <fmt/format.h>

#include <memory>
#include <vector>
#include <string>

#include "camera.h"
#include "renderable.h"
#include "node.h"
#include "shader.h"
#include "vertex.h"

class Mesh : public Renderable
{
public:
  Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
    : m_vertices(vertices), m_indices(indices)
  {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
  };

  virtual bool is_visible() const override { return true; }

  virtual void render([[maybe_unused]] Camera *camera) const override
  {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
  }

  void set_shader(const std::shared_ptr<ShaderProgram> &shader)
  {
    m_shader = shader;
  }

  const std::shared_ptr<ShaderProgram> &get_shader() const
  {
    return m_shader;
  }


private:
  unsigned int m_vao = 0;
  unsigned int m_vbo = 0;
  unsigned int m_ebo = 0;
  std::shared_ptr<ShaderProgram> m_shader;
  std::string m_name;
  std::vector<Vertex> m_vertices;
  std::vector<uint32_t> m_indices;
};

class StaticMesh : public Renderable
{
public:
  void add_mesh(Mesh mesh)
  {
    m_meshes.emplace_back(mesh);
  };

  glm::mat4 get_transform() const
  {
    auto ret = glm::scale(m_scale);
    ret *= glm::translate(m_position);
    ret *= glm::orientate4(m_rotation);
    return ret;
  }

  void translate(const glm::vec3 &translation)
  {
    m_position += translation;
  }
  glm::vec3 get_position() const
  {
    return m_position;
  }
  void set_position(glm::vec3 position)
  {
    m_position = position;
  }
  glm::vec3 get_rotation() const
  {
    return m_rotation;
  }
  void set_rotation(glm::vec3 rotation)
  {
    m_rotation = rotation;
  }
  glm::vec3 get_scale() const
  {
    return m_scale;
  }
  void set_scale(glm::vec3 scale)
  {
    m_scale = scale;
  }

  virtual bool is_visible() const override { return true; }
  virtual void render(Camera *camera) const override
  {
    auto model = get_transform();
    auto &view = camera->get_view_matrix();
    auto &projection = camera->get_projection_matrix();

    for (auto &mesh : m_meshes) {
      auto &shader = mesh.get_shader();
      shader->use();
      shader->set("model", 1, false, glm::value_ptr(model));
      shader->set("projection", 1, false, glm::value_ptr(projection));
      shader->set("view", 1, false, glm::value_ptr(view));
      mesh.render(camera);
    }
  }

  void set_shader(const std::shared_ptr<ShaderProgram> &shader)
  {
    for (auto &mesh : m_meshes) {
      mesh.set_shader(shader);
    }
  }

private:
  glm::vec3 m_position = glm::vec3{ 0.f, 0.f, 0.f };
  glm::vec3 m_rotation = glm::vec3{ 0.f };
  glm::vec3 m_scale = glm::vec3{ 1.f };

  std::vector<Mesh> m_meshes;
};
