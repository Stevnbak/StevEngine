#pragma once
#include <SDL.h>
#include <cstddef>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>

namespace StevEngine {
	namespace Resources {
		class Resource {
			friend class ResourceManager;
			public:
				const unsigned short id;
				const std::string path;
				SDL_RWops* GetSDLData();
				std::string GetStrData();
				const char* GetRawData();
				int GetSize();
			private:
				Resource();
				Resource(std::string path, const char* data, const int size);
				const char* data;
				const int size;
				static unsigned short currentId;
		};

		class ResourceManager {
			public:
				ResourceManager();
				Resource GetFile(unsigned short id) const;
				Resource GetFile(std::string path) const;
				void AddFile(std::string path, const char* data, const int size);
				template<int hexSize = 2> void AddFileFromHex(std::string path, const char* hex, const int size);
			private:
				std::map<unsigned short, const Resource> resources;
				std::map<std::string, unsigned short> pathToId;
		};

		extern ResourceManager resourceManager;
	}
}
