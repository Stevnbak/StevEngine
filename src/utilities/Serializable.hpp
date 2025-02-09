#pragma once
#include "main/ResourceManager.hpp"
#include <sstream>

namespace StevEngine {
	enum StreamType {
		Text,
		Binary
	};
	class Stream {
		public:
			Stream(StreamType type) : type(type) {};

			template <typename T> T Read();
			template <typename T> void Write(const T& data);

			//Stream operators
			/*
			  Write to stream
		 	*/
			template <typename T> Stream& operator << (const T& data) {
				Write<T>(data);
				return *this;
			}
			/*
			  Read from stream
		 	*/
			template <typename T> Stream& operator >> (T& out) {
				out = Read<T>();
				return *this;
			}

			#ifdef StevEngine_PLAYER_DATA
			//Write stream to file
			void WriteToFile(const char* name) const;
			#endif
			//Read from file
			void ReadFromFile(const Resources::Resource& file);

			//Get raw string stream
			std::stringstream& GetStream() { return stream; }

			//Copy constructor
			Stream(const Stream& stream) : type(stream.type) {
				*this << stream;
			}

			//Destructor default
			virtual ~Stream() = default;
		protected:
			std::stringstream stream;
			const StreamType type;
	};
}