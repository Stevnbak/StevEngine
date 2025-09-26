#pragma once
#include <fstream>
#include <sstream>

namespace StevEngine::Resources {
	class Resource;
}

namespace StevEngine::Utilities {
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
			Stream(StreamType type = Binary) : type(type) {};

			/**
			 * @brief Create new stream of type, with data already added
			 * @param data Data to add to stream
			 * @param type Type of stream to create
			 */
			template <typename T> Stream(const T& data, StreamType type = Binary) : type(type) {
				*this << data;
			};


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

			/**
			 * @brief Write full stream content to a file
			 * @param path Path of the file to write to
			 *
			 * Writes the full stream content to a file with the specified path.
			 */
			void WriteToFile(const char* path) const;

			/**
			 * @brief Read content from a file to stream
			 * @param file File to read from
			 */
			bool ReadFromFile(const Resources::Resource& file);

			/**
			 * @brief Read content from an input file stream to stream
			 * @param file Input file stream to read from
			 */
			bool ReadFromFile(std::ifstream& file);

			/**
			 * @brief Get underlying std::stringstream object
			 * @return Underlying std::stringstream
			 */
			std::stringstream& GetStream() { return stream; }
			operator std::stringstream&() { return stream; }

			/**
			 * @brief Get underlying std::stringstream object
			 * @return Underlying std::stringstream
			 */
			const std::stringstream& GetStream() const { return stream; }
			operator const std::stringstream&() const { return stream; }

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