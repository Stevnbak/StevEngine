#pragma once
#include "Stream.hpp"
#include <map>

namespace StevEngine::Utilities {
	class KeyValueStore {
		public:
			KeyValueStore() {};

			/**
			 * @brief Read data of specified type
			 * @tparam T Type of data to read
			 * @param key Key to read
			 * @return Read value of type T
			 */
			template <typename T> T Read(const std::string& key) const {
				Stream stream(Text);
				stream.GetStream() << data.at(key).GetStream().str();
				return stream.Read<T>();
			}

			/**
			 * @brief Write data of specified type
			 * @tparam T Type of data to write
			 * @param key Key to data
			 * @param data Value to write
			 */
			template <typename T> void Write(const std::string& key, const T& input) {
				if(data.contains(key)) {
					data.erase(key);
				}
				Stream stream(Text);
				stream << input;
				data.emplace(key, stream);
			}

			/**
			 * @brief Erase key from store
			 * @param key Key to data
			 */
			void Erase(std::string key);

			/**
			 * @brief Erase key from store
			 * @param key Key to data
			 * @return Boolean showing if key contains any data
			 */
			bool Contains(std::string key) const;

			/**
			 * @brief Write full content to a file
			 * @param path Path of the file to write to
			 *
			 * Writes the full key value store content to a file with the specified path.
			 */
			void WriteToFile(const char* path) const;

			/**
			 * @brief Read content from a file
			 * @param file File to read from
			 */
			void ReadFromFile(const Resources::Resource& file);

			/**
			 * @brief Read content from an input file stream
			 * @param file Input file stream to read from
			 */
			void ReadFromFile(std::ifstream& file);
		private:
			/** @brief Map containing all saved data streams */
			std::map<std::string, Stream> data;
			/** @brief Read from stream */
			void ReadFromStream(Stream& stream);
	};
}