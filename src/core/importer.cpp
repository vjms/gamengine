#include "importer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <fmt/format.h>
#include <algorithm>
#include <memory>

#include "staticmesh.h"

template<>
struct fmt::formatter<glm::vec3>
{
  constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto format(const glm::vec3 v, FormatContext &ctx) -> decltype(ctx.out())
  {
    return format_to(ctx.out(),
      "{} {} {}",
      v.x,
      v.y,
      v.z);
  }
};

glm::vec3 to_glm(const aiVector3D &vec)
{
  return glm::vec3{ vec.x, vec.y, vec.z };
}

std::shared_ptr<StaticMesh> Importer::load_static_mesh(const std::string &path)
{
  Assimp::Importer importer{};
  auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    fmt::print(stderr, "assimp fuckup: {}\n", importer.GetErrorString());
  }
  if (!scene->HasMeshes()) {
    fmt::print(stderr, "No meshes in {}\n", path);
  }

  auto sm = std::make_shared<StaticMesh>();

  for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
    auto mesh = scene->mMeshes[i];
    std::vector<Vertex> vertices;
    for (unsigned int vi = 0; vi < mesh->mNumVertices; vi++) {
      vertices.emplace_back(Vertex{ to_glm(mesh->mVertices[vi]), to_glm(mesh->mNormals[vi]) });
    }
    std::vector<uint32_t> indices;
    std::for_each(mesh->mFaces, &mesh->mFaces[mesh->mNumFaces], [&](const aiFace &face) { std::for_each(face.mIndices, &face.mIndices[face.mNumIndices], [&](const unsigned int &index) { indices.emplace_back(index); }); });

    sm->add_mesh(Mesh{ vertices, indices });
  }
  return sm;
}
