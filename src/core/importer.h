#pragma once

#include <string>

class StaticMesh;

/**
 * Import and cache stuff.
*/
class Importer
{
public:
	StaticMesh load_static_mesh(const std::string &path);
};
