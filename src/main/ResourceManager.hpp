#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>

namespace StevEngine {
	class Engine;
	namespace Resources {
		class Resource {
			friend class System;
			public:
				const ushort id;
				const std::string path;
				SDL_RWops* GetSDLData();
				std::string GetStrData();
			private:
				Resource();
				Resource(std::string path, const char* data, const int size);
				const char* data;
				const int size;
				static ushort currentId;
		};

		class System {
			friend class StevEngine::Engine;
			public:
				Resource GetFile(ushort id) const;
				Resource GetFile(std::string path) const;
				void AddFile(std::string path, const char* data, const int size);
				void AddFileFromHex(std::string path, const char* hex, const int size, const int hexSize = 2);
			private:
				System();
				std::map<ushort, const Resource> resources;
				std::map<std::string, ushort> pathToId;
		};
	}
}