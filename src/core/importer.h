#pragma once

#include <string>
#include <memory>

class StaticMesh;

/**
 * Import and cache stuff.
*/
class Importer
{
public:
  std::shared_ptr<StaticMesh> load_static_mesh(const std::string &path);
};
