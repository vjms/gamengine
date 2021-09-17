#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "renderable.h"
#include "shader.h"

#include <glad/glad.h>

#include <memory>

static inline const char *vertexShaderSource = "#version 330 core\n"
											   "layout (location = 0) in vec3 aPos;\n"
											   "void main()\n"
											   "{\n"
											   "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
											   "}\0";
static inline const char *fragmentShaderSource = "#version 330 core\n"
												 "out vec4 FragColor;\n"
												 "void main()\n"
												 "{\n"
												 "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
												 "}\n\0";

class Mesh : public Renderable
{
public:
	Mesh(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<uint32_t> indices)
		: m_vertices(vertices), m_normals(normals), m_indices(indices){};

	virtual void render() const override
	{
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glUseProgram(m_shader->get_handle());
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * 3, m_vertices.data(), GL_STATIC_DRAW);

		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void set_shader(const std::shared_ptr<ShaderProgram> &shader)
	{
		m_shader = shader;
	}

private:
	std::shared_ptr<ShaderProgram> m_shader;
	std::string m_name;
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;
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
		for (auto &mesh : m_meshes)
		{
			mesh.render();
		}
	}

	void set_shader(const std::shared_ptr<ShaderProgram> &shader)
	{
		for (auto &mesh : m_meshes)
		{
			mesh.set_shader(shader);
		}
	}

private:
	std::vector<Mesh> m_meshes;
};
