#include "ResourceManager.hpp"
#include "Log.hpp"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <string>
namespace fs = std::filesystem;

namespace StevEngine {
	namespace ResourceManager {
		std::string resourcePath = fs::absolute("./Resources").generic_string();
		std::map<short, const Resource> resources = std::map<short, const Resource>();
		void RefreshMetadata() {
			std::string metaDataPath = resourcePath + "/meta.data";
			//Read already established meta data
			std::string metaDataText = "";
			if (fs::exists(metaDataPath)) std::ifstream(metaDataPath) >> metaDataText;
			Log::Normal(metaDataText);
			//Add files to resources from meta data
			std::string str;
			std::stringstream ss(metaDataText);
			std::vector<std::string> metaFiles;
			while (std::getline(ss, str)) {
				std::string idStr, pathStr;
				std::stringstream sss(str);
				std::getline(sss, idStr, ';');
				std::getline(sss, pathStr, ';');
				short id = stoi(idStr);
				std::string fullPath = fs::absolute("./Resources" + pathStr).generic_string();
				Log::Normal("Full path: " + fullPath + " Id: " + idStr + " Exists: " + std::format("{}", fs::exists(fullPath)), true);
				if (fs::exists(fullPath)) {
					const Resource resource(fullPath, id);
					resources.insert({ resource.id, resource });
					metaFiles.push_back(fullPath);
					Log::Normal("Resource (from meta.data): " + resource.fullPath, true);
				}
				else {
					Log::Error(std::format("File ({}) from meta.data, has been deleted or moved.", fullPath));
				}
			}
			//Go through all files directory and add new ones to meta data
			for (const auto& entry : fs::directory_iterator(resourcePath)) {
				std::string fullPath = fs::absolute(entry.path()).generic_string();
				if (fullPath.find("/meta.data") != std::string::npos) continue;
				if (std::find(metaFiles.begin(), metaFiles.end(), fullPath) != metaFiles.end()) continue;
                ///std::ifstream myData(entry.path().generic_string(), std::ios::binary);
                ///std::filebuf* data = myData.rdbuf();
				const Resource resource (fs::absolute(entry.path()).generic_string());
				resources.insert({ resource.id, resource });
				Log::Normal("Resource (from file): " + resource.fullPath, true);
			}
			//Update meta data based on resources
			std::string newMetaDataText = "";
			for (std::pair<short, const Resource> resourcePair : resources) {
				std::string s = resourcePair.second.fullPath;
				s.replace(s.begin(), s.begin() + resourcePath.length(), "");
				newMetaDataText += std::format("{};{}\n", resourcePair.second.id, s);
			}
			std::ofstream outputFile(metaDataPath);
			outputFile.write(newMetaDataText.c_str(), newMetaDataText.size());
		}

		short Resource::currentId = 0;
		short Resource::GetNewId() {
			short id = -1;
			while (id == -1) {
				if (resources.count(Resource::currentId) != 1) {
					id = Resource::currentId++;
				}
			};
			return id;
		}
		Resource::Resource(std::string path) : id(GetNewId()) {
			fullPath = path;
		}
		Resource::Resource(std::string path, short oldId) : id(oldId) {
			fullPath = path;
		}
	}
}