#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 

		glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		int model_loc = glGetUniformLocation(m_shader->get_handle(), "model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

		int view_loc = glGetUniformLocation(m_shader->get_handle(), "view");
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

		int projection_loc = glGetUniformLocation(m_shader->get_handle(), "projection");
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

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
