#pragma once
#include <SDL.h>
#include <string>
#include <map>

namespace StevEngine {
	namespace Resources {
		class Resource {
			friend class ResourceManager;
			public:
				const unsigned short id;
				const std::string path;
				SDL_RWops* GetSDLData() const;
				std::string GetStrData() const;
				const char* GetRawData() const;
				int GetSize() const;
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
				const Resource& GetFile(unsigned short id) const;
				const Resource& GetFile(std::string path) const;
				void AddFile(std::string path, const char* data, const int size);
				template<int hexSize = 2> void AddFileFromHex(std::string path, const char* hex, const int size);
			private:
				std::map<unsigned short, const Resource> resources;
				std::map<std::string, unsigned short> pathToId;
		};

		extern ResourceManager resourceManager;
	}
}
