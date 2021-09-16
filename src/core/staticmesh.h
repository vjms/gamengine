#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

class Mesh
{
public:
	Mesh(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<uint32_t> indices)
		: m_vertices(vertices), m_normals(normals), m_indices(indices){};

private:
	std::string m_name;
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<uint32_t> m_indices;
};

class StaticMesh
{
public:
	// todo: clean this shit up
	void add_mesh(Mesh mesh)
	{
		m_meshes.emplace_back(mesh);
	};

private:
	std::vector<Mesh> m_meshes;
};
