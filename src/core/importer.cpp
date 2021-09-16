#include "importer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <fmt/format.h>
#include <algorithm>

#include "staticmesh.h"

template <>
struct fmt::formatter<glm::vec3>
{
	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
	{
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const glm::vec3 v, FormatContext &ctx) -> decltype(ctx.out())
	{
		return format_to(ctx.out(),
						 "{} {} {}", v.x, v.y, v.z);
	}
};

StaticMesh load_static_mesh(const std::string &path)
{
	Assimp::Importer importer{};
	auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		fmt::print(stderr, "assimp fuckup: {}\n", importer.GetErrorString());
	}
	if (!scene->HasMeshes())
	{
		fmt::print(stderr, "No meshes in {}\n", path);
	}

	StaticMesh sm{};
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		auto mesh = scene->mMeshes[i];
		std::vector<glm::vec3> vertices;
		std::for_each(mesh->mVertices, &mesh->mVertices[mesh->mNumVertices], [&](auto vertex)
					  { vertices.emplace_back(vertex.x, vertex.y, vertex.z); });
		for (auto &vertex : vertices)
		{
			fmt::print("{}\n", vertex);
		}
		// sm.add_mesh(Mesh{});
	}
	return sm;
}