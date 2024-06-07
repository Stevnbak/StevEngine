#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
namespace StevEngine {
	namespace ResourceManager {
		class Resource {
			public:
				const ushort id;
				const std::string path;
				std::vector<char> data;
				Resource();
				Resource(std::string path, std::vector<char> data);
			private:
				static ushort currentId;
		};

		class ResourceSystem {
			public:
				ResourceSystem(std::string resourcePath);
				Resource GetFile(ushort id) const;
				Resource GetFile(std::string path) const;
				const std::string resourcePath;
			private:
				std::map<short, const Resource> resources;
				std::map<std::string, short> pathToId;
		};

		//Helper functions

		constexpr std::string DataToText(std::vector<char> data) {
			return std::string(data.begin(), data.end());
		}
	}
}

/* TODO:
[] Read all asset files
[] Be able to get file-data
*/