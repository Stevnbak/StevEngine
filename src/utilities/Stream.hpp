#pragma once
#include "main/ResourceManager.hpp"

#include <sstream>

namespace StevEngine {
	/**
	 * @brief Available types of streams to use
	 */
	enum StreamType {
		Text,
		Binary
	};
	/**
	 * @brief Stream for serialization of data
	 *
	 * Handles writing and reading basic data types, utility types, components, GameObjects etc. to a text or binary stream.
	 */
	class Stream {
		public:
			const StreamType type; ///< Type of serialization used by stream

			/**
			 * @brief Create new stream of type
			 * @param type Type of stream to create
			 */
			Stream(StreamType type) : type(type) {};

			/**
			 * @brief Read data of specified type from stream
			 * @tparam T Type of data to read
			 * @return Read value of type T
			 */
			template <typename T> T Read();

			/**
			 * @brief Write data of specified type to stream
			 * @tparam T Type of data to write
			 * @param data Value to write
			 */
			template <typename T> void Write(const T& data);

			//Stream operators
			/**
			 * @brief Write data of specified type to stream
			 * @tparam T Type of data to write
			 * @param data Value to write
			 */
			template <typename T> Stream& operator << (const T& data) {
				Write<T>(data);
				return *this;
			}
			/**
			 * @brief Read data of specified type from stream
			 * @tparam T Type of data to read
			 * @return Read value of type T
			 */
			template <typename T> Stream& operator >> (T& out) {
				out = Read<T>();
				return *this;
			}

			#ifdef StevEngine_PLAYER_DATA
			/**
			 * @brief Write full stream content to a file
			 * @param name Name of the file to write to
			 *
			 * Writes the full stream content to a file with the specified name in the appdata folder.
			 */
			void WriteToFile(const char* name) const;
			#endif
			/**
			 * @brief Read content from a file to stream
			 * @param file File to read from
			 */
			void ReadFromFile(const Resources::Resource& file);

			/**
			 * @brief Get underlying std::stringstream object
			 * @return Underlying std::stringstream
			 */
			std::stringstream& GetStream() { return stream; }

			/**
			 * @brief make a copy of a stream
			 * @param stream Stream to copy
			 */
			Stream(const Stream& stream) : type(stream.type) {
				*this << stream;
			}
		protected:
			std::stringstream stream;
	};
}