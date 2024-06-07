#include "ResourceManager.hpp"

#include <filesystem>
#include <fstream>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <string.h>
namespace fs = std::filesystem;

static std::vector<char> ReadAllBytes(char const* filename)
{
	std::ifstream ifs(filename, std::ios::binary|std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();
	std::vector<char>  result(pos);
	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], pos);
	return result;
}

namespace StevEngine {
	namespace ResourceManager {
		//Create resource system by reading all files
		ResourceSystem::ResourceSystem(std::string resourcePath) : resourcePath(resourcePath) {
			resources = std::map<short, const Resource>();
			std::filesystem::create_directories(resourcePath);
			for (const auto& entry : fs::directory_iterator(resourcePath)) {
				std::string fullPath = fs::absolute(entry.path()).generic_string();
                std::vector<char> data = ReadAllBytes(entry.path().c_str());
				const Resource resource (fs::relative(fullPath, resourcePath).generic_string(), data);
				resources.insert({ resource.id, resource });
				pathToId.insert({ resource.path, resource.id });
			}
		}
		//Get file
		Resource ResourceSystem::GetFile(ushort id) const {
			return resources.at(id);
		}
		Resource ResourceSystem::GetFile(std::string path) const {
			return resources.at(pathToId.at(path));
		}

		ushort Resource::currentId = 0;

		Resource::Resource(std::string path, std::vector<char> data) : id(Resource::currentId++), path(path), data(data) {}
		Resource::Resource() : id(Resource::currentId++), path("") {}
	}
}