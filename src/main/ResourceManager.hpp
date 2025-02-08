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
		/**
		 * @brief Container for loaded resource data
		 */
		class Resource {
			friend class ResourceManager;
			public:
				/** @brief Unique resource identifier */
				const unsigned short id;

				/** @brief Path to resource file */
				const std::string path;

				/**
				 * @brief Get SDL-compatible data handle
				 * @return SDL_RWops pointer for data
				 */
				SDL_RWops* GetSDLData() const;

				/**
				 * @brief Get resource data as string
				 * @return String containing resource data
				 */
				std::string GetStrData() const;

				/**
				 * @brief Get raw resource data
				 * @return Pointer to raw data bytes
				 */
				const char* GetRawData() const;

				/**
				 * @brief Get size of resource data
				 * @return Size in bytes
				 */
				int GetSize() const;

			private:
				Resource();
				Resource(std::string path, const char* data, const int size);
				const char* data;		   ///< Raw resource data
				const int size;			 ///< Data size in bytes
				static unsigned short currentId;  ///< Next resource ID
		};

		/**
		 * @brief Manages loading and access to game resources
		 *
		 * Handles loading and caching of resource files like textures,
		 * models, audio etc. Resources can be accessed by ID or path.
		 */
		class ResourceManager {
			public:
				ResourceManager();

				/**
				 * @brief Get resource by ID
				 * @param id Resource identifier
				 * @return Reference to resource
				 * @throws Error if resource not found
				 */
				const Resource& GetFile(unsigned short id) const;

				/**
				 * @brief Get resource by path
				 * @param path Resource file path
				 * @return Reference to resource
				 * @throws Error if resource not found
				 */
				const Resource& GetFile(std::string path) const;

				/**
				 * @brief Add resource from raw data
				 * @param path Virtual path for resource
				 * @param data Raw data bytes
				 * @param size Size of data in bytes
				 */
				void AddFile(std::string path, const char* data, const int size);

				/**
				 * @brief Add resource from hex string data
				 * @tparam hexSize Size of hex values (usually 2)
				 * @param path Virtual path for resource
				 * @param hex Hex string data
				 * @param size Size of final data in bytes
				 */
				template<int hexSize = 2> void AddFileFromHex(std::string path, const char* hex, const int size);

			private:
				/** @brief Map of resources by ID */
				std::map<unsigned short, const Resource> resources;

				/** @brief Map of resource IDs by path */
				std::map<std::string, unsigned short> pathToId;
		};

		/** Global resource manager instance */
		extern ResourceManager resourceManager;
	}
}
