#pragma once
#include <string>
#include <map>
namespace StevEngine {
	namespace ResourceManager {
		class Resource;
		extern std::string resourcePath;
		///extern std::vector<const Resource> resources;
		extern std::map<short, const Resource> resources;
		void RefreshMetadata();

		class Resource {
			public:
				const short id;
				std::string fullPath;
				Resource(std::string path);
				Resource(std::string path, short oldId);
			private:
				static short currentId;
				static short GetNewId();
		};
	}
}