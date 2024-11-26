#include "ResourceManager.hpp"
#include "main/Log.hpp"

#include <cstddef>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>

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
		ResourceManager resourceManager = ResourceManager();
		//Create resource system by reading all files
		ResourceManager::ResourceManager() {
			resources = std::map<unsigned short, const Resource>();
			pathToId = std::map<std::string, unsigned short>();
		}

		//Get file
		const Resource& ResourceManager::GetFile(unsigned short id) const {
			if(resources.contains(id)) {
				return resources.at(id);
			}
			else {
				Log::Error(std::format("Resource \"{}\" not found.", id), true);
				throw std::runtime_error(std::format("Resource {} not found.", id));
			}
		}
		const Resource& ResourceManager::GetFile(std::string path) const {
			if(pathToId.contains(path)) {
				return resources.at(pathToId.at(path));
			}
			else {
				Log::Error(std::format("Resource \"{}\" not found.", path), true);
				throw std::runtime_error("Failed to find resource!");
			}
		}

		void ResourceManager::AddFile(std::string path, const char* data, const int size) {
			Resource resource(path, data, size);
			resources.insert({resource.id, resource});
			pathToId.insert({resource.path, resource.id});
		}

		template<int hexSize> void ResourceManager::AddFileFromHex(std::string path, const char* hex, const int size) {
			char* data = new char[size];
			char h[hexSize+1];
			h[hexSize] = '\0';
			for(int i = 0; i < size; i++) {
				for(int j = 0; j < hexSize; j++) {
					h[j] = hex[i*hexSize + j];
				}
				data[i] = strtoul(h, NULL, hexSize * 8);
			}
			Resource resource (path, data, size);
			resources.insert({resource.id, resource});
			pathToId.insert({resource.path, resource.id});
		}

		unsigned short Resource::currentId = 0;
		Resource::Resource() : id(Resource::currentId++), path(), size() {}
		Resource::Resource(std::string path, const char* data, const int size) : id(Resource::currentId++), path(path), data(data), size(size) {}

		int Resource::GetSize() const {
			return size;
		}
		const char* Resource::GetRawData() const {
			return data;
		}
		SDL_RWops* Resource::GetSDLData() const {
			return SDL_RWFromConstMem(data, size);
		};
		std::string Resource::GetStrData() const {
			std::stringstream ss;
			for(int i = 0; i < size; i++) {
				ss << data[i];
			}
			return ss.str();
		}
	}
}
