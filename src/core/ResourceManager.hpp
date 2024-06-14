#pragma once
#include <SDL.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>

namespace StevEngine {
	namespace Resources {
		class Resource {
			public:
				const ushort id;
				const std::string path;
				Resource();
				Resource(std::string path);
				SDL_RWops* GetData();
			private:
				static ushort currentId;
		};

		class System {
			public:
				System(std::string resourcePath);
				Resource GetFile(ushort id) const;
				Resource GetFile(std::string path) const;
				const std::string resourcePath;
			private:
				std::map<ushort, const Resource> resources;
				std::map<std::string, ushort> pathToId;
		};

		//Helper functions
		constexpr std::string DataToText(SDL_RWops* data)  {
			bool reading = true;
			std::vector<char> chars;
			while(reading) {
				Uint8 b = SDL_ReadU8(data);
				if(b == 0) reading = false;
				else chars.push_back(b);
			}
			SDL_FreeRW(data);
			return std::string(chars.begin(), chars.end());
		}
	}
}