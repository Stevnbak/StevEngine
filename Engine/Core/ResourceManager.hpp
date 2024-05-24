#pragma once
#include <string>
#include <map>
#include <vector>
namespace StevEngine {
	namespace ResourceManager {
		class Resource;
		extern std::string resourcePath;
		extern std::map<short, const Resource> resources;
		void RefreshMetadata();
		const Resource* GetResource(short id);
		const Resource* GetResource(std::string path);

		class Resource {
			public:
				const short id;
				std::string fullPath;
				Resource();
				Resource(std::string path);
				Resource(std::string path, short oldId);
			private:
				static short currentId;
				static short GetNewId();
		};

		//Read file functions
		std::string ReadTextFile(const Resource* file);
		void ReadJsonFile(const Resource* file);
		void ReadXmlFile(const Resource* file);
		std::vector<std::vector<std::string>> ReadCsvFile(const Resource* file);
	}
}