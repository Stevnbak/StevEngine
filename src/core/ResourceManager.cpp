#include "ResourceManager.hpp"
#include "Engine.hpp"

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
	namespace Resources {
		//Create resource system by reading all files
		System::System() {
			resources = std::map<ushort, const Resource>();
			pathToId = std::map<std::string, ushort>();
		}

		//Get file
		Resource System::GetFile(ushort id) const {
			return resources.at(id);
		}
		Resource System::GetFile(std::string path) const {
			return resources.at(pathToId.at(path));
		}
		
		void System::AddFile(std::string path, const char* data, const int size) {
			Resource resource (path, data, size);
			resources.insert({resource.id, resource});
			pathToId.insert({resource.path, resource.id});
		}

		ushort Resource::currentId = 0;
		Resource::Resource() : id(Resource::currentId++), path(), size() {}
		Resource::Resource(std::string path, const char* data, const int size) : id(Resource::currentId++), path(path), data(data), size(size) {}

		SDL_RWops* Resource::GetSDLData() {
			return SDL_RWFromConstMem(data, size);
		};
		std::string Resource::GetStrData() {
			std::stringstream ss;
			for(int i = 0; i < size; i++) {
				ss << data[i];
			}
			return ss.str();
		}
	}
}