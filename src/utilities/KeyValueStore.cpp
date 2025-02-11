#include "KeyValueStore.hpp"

namespace StevEngine::Utilities {
	void KeyValueStore::Erase(std::string key) {
		data.erase(key);
	}

	bool KeyValueStore::Contains(std::string key) const {
		return data.contains(key);
	}


	void KeyValueStore::WriteToFile(const char* path) const {
		Stream stream(Text);
		stream << (uint32_t)data.size();
		for(const auto& [key, str] : data) {
			stream << key << str << '\n';
		}
		stream.WriteToFile(path);
	}

	void KeyValueStore::ReadFromFile(const Resources::Resource& file) {
		Stream stream(Text);
		stream.ReadFromFile(file);
		ReadFromStream(stream);
	}

	void KeyValueStore::ReadFromFile(std::ifstream& file) {
		Stream stream(Text);
		stream.ReadFromFile(file);
		ReadFromStream(stream);
	}

	void KeyValueStore::ReadFromStream(Stream& stream) {
		uint32_t keys;
		stream >> keys;
		for(uint32_t i = 0; i < keys; i++) {
			std::string key = stream.Read<std::string>();
			Stream keyStream(Text);
			std::string v;
			getline(stream.GetStream(), v, '\n');
			for(char c : v) keyStream << c;
			data.emplace(key, keyStream);
		}
	}

}