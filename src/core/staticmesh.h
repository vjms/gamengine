#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <memory>
#include <vector>
#include <string>

#include "renderable.h"
#include "shader.h"
#include "vertex.h"


static inline const char *vertexShaderSource =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";
static inline const char *fragmentShaderSource =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n\0";

class Mesh : public Renderable
{
public:
  Mesh(std::vector<Vertex> &vertices, std::vector<uint32_t> indices)
    : m_vertices(vertices), m_indices(indices){};

  virtual void render() const override
  {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.5f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    m_shader->use();
    m_shader->set("model", 1, glm::value_ptr(model));
    m_shader->set("projection", 1, glm::value_ptr(projection));
    m_shader->set("view", 1, glm::value_ptr(view));

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
  }

  void set_shader(const std::shared_ptr<ShaderProgram> &shader)
  {
    m_shader = shader;
  }

private:
  std::shared_ptr<ShaderProgram> m_shader;
  std::string m_name;
  std::vector<Vertex> m_vertices;
  std::vector<uint32_t> m_indices;
};

class StaticMesh : public Renderable
{
public:
  // todo: clean this shit up
  void add_mesh(Mesh mesh)
  {
    m_meshes.emplace_back(mesh);
  };

  virtual void render() const override
  {
    for (auto &mesh : m_meshes) {
      mesh.render();
    }
  }

  void set_shader(const std::shared_ptr<ShaderProgram> &shader)
  {
    for (auto &mesh : m_meshes) {
      mesh.set_shader(shader);
    }
  }

private:
  std::vector<Mesh> m_meshes;
};
