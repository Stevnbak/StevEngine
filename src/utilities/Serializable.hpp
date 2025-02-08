#pragma once
#include "main/ResourceManager.hpp"
#include <sstream>

namespace StevEngine {
	class SerializableStream {
		public:
			#ifdef StevEngine_PLAYER_DATA
			//Write stream to file
			virtual void WriteToFile(const char* name) const = 0;
			#endif
			//Read from file
			virtual void ReadFromFile(const Resources::Resource& file) = 0;

			//Get raw string stream
			std::stringstream& GetStream() { return stream; }

			//Destructor default
			virtual ~SerializableStream() = default;
		protected:
			std::stringstream stream;
	};

	class TextSerializableStream : public SerializableStream {
		public:
			template <typename T> T Read();
			template <typename T> void Write(const T& data);

			//Stream operators
			/*
			  Write to stream
		 	*/
			template <typename T> TextSerializableStream& operator << (const T& data) {
				Write<T>(data);
				return *this;
			}
			/*
			  Read from stream
		 	*/
			template <typename T> TextSerializableStream& operator >> (T& out) {
				out = Read<T>();
				return *this;
			}

			#ifdef StevEngine_PLAYER_DATA
			//Write stream to file
			void WriteToFile(const char* name) const;
			#endif
			void ReadFromFile(const Resources::Resource& file);
	};
	class BinarySerializableStream : public SerializableStream {
		public:
			template <typename T> T Read();
			template <typename T> void Write(const T& data);

			//Stream operators
			/*
			  Write to stream
		 	*/
			template <typename T> BinarySerializableStream& operator << (const T& data) {
				Write<T>(data);
				return *this;
			}
			/*
			  Read from stream
		 	*/
			template <typename T> BinarySerializableStream& operator >> (T& out) {
				out = Read<T>();
				return *this;
			}

			#ifdef StevEngine_PLAYER_DATA
			//Write stream to file
			void WriteToFile(const char* name) const;
			#endif
			void ReadFromFile(const Resources::Resource& file);
	};
}