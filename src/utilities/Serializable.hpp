#pragma once
#include "main/ResourceManager.hpp"
#include <sstream>

namespace StevEngine {
	class Stream {
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
			virtual ~Stream() = default;
		protected:
			std::stringstream stream;
	};

	class TextStream : public Stream {
		public:
			template <typename T> T Read();
			template <typename T> void Write(const T& data);

			//Stream operators
			/*
			  Write to stream
		 	*/
			template <typename T> TextStream& operator << (const T& data) {
				Write<T>(data);
				return *this;
			}
			/*
			  Read from stream
		 	*/
			template <typename T> TextStream& operator >> (T& out) {
				out = Read<T>();
				return *this;
			}

			#ifdef StevEngine_PLAYER_DATA
			//Write stream to file
			void WriteToFile(const char* name) const;
			#endif
			void ReadFromFile(const Resources::Resource& file);
	};
	class BinaryStream : public Stream {
		public:
			template <typename T> T Read();
			template <typename T> void Write(const T& data);

			//Stream operators
			/*
			  Write to stream
		 	*/
			template <typename T> BinaryStream& operator << (const T& data) {
				Write<T>(data);
				return *this;
			}
			/*
			  Read from stream
		 	*/
			template <typename T> BinaryStream& operator >> (T& out) {
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