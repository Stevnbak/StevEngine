#include "Stream.hpp"
#include "data/DataManager.hpp"
#include <cstdint>
#include <cstdio>
#include <fstream>

namespace StevEngine {
	//Write stream to file
	#ifdef StevEngine_PLAYER_DATA
	void Stream::WriteToFile(const char* name) const {
		std::ofstream file;
		file.open(Data::data.GetAppdataPath() + name, (type == Binary ? std::ios::out | std::ios::binary : std::ios::out));
		file << stream.str();
	}
	#endif

	//Read from file
	void Stream::ReadFromFile(const Resources::Resource& file) {
		const char* data = file.GetRawData();
		size_t size = file.GetSize();
		for(int i = 0; i < size; i++) *this << (char)data[i];
	}

	//Write stream to stream
	template <> void Stream::Write(const Stream& data) {
		if(data.type != type) throw("Stream types are not matching!");
		stream << data.stream.rdbuf();
	}
	//Read char from stream
	template <> char Stream::Read<char>() {
		if(stream.eof()) return (char)NULL;
		char value;
		switch(type) {
			case Text:
				stream >> value;
				break;
			case Binary:
				stream.read((char*)&value, sizeof(value));
				break;
		}
		return value;
	}
	//Write char to stream
	template <> void Stream::Write<char>(const char& data) {
		switch(type) {
			case Text:
				stream << data;
				break;
			case Binary:
				stream.write((char*)&data, sizeof(data));
				break;
		}
	}
	//Read bool from stream
	template <> bool Stream::Read<bool>() {
		if(stream.eof()) return (bool)NULL;
		bool value;
		switch(type) {
			case Text:
				stream >> value;
				break;
			case Binary:
				stream.read((char*)&value, sizeof(value));
				break;
		}
		return value;
	}
	//Write bool to stream
	template <> void Stream::Write<bool>(const bool& data) {
		switch(type) {
			case Text:
				stream << data;
				break;
			case Binary:
				stream.write((char*)&data, sizeof(data));
				break;
		}
	}
	//Read string from stream
	template <> std::string Stream::Read<std::string>() {
		uint32_t size;
		*this >> size;
		char* text = new char[size];
		if(type == Text) {
			stream.read(text, size);
		} else {
			for(uint32_t i = 0; i < size; i++) {
				*this >> text[i];
			}
		}
		return std::string(text, size);
	}
	//Write string to stream
	template <> void Stream::Write<std::string>(const std::string& data) {
		*this << (uint32_t)data.size();
		switch(type) {
			case Text:
				stream << data;
				break;
			case Binary:
				for(uint32_t i = 0; i < data.size(); i++) *this << data[i];
				break;
		}
	}
	//Read uint8 from stream
	template <> uint8_t Stream::Read<uint8_t>() {
		if(stream.eof()) return (uint8_t)NULL;
		uint8_t value;
		switch(type) {
			case Text:
				char c;
				stream >> value >> c;
				break;
			case Binary:
				stream.read((char*)&value, sizeof(value));
				break;
		}
		return value;
	}
	//Write uint8 to stream
	template <> void Stream::Write<uint8_t>(const uint8_t& data) {
		switch(type) {
			case Text:
				stream << data << ';';
				break;
			case Binary:
				stream.write((char*)&data, sizeof(data));
				break;
		}
	}
	//Read uint8 from stream
	template <> uint16_t Stream::Read<uint16_t>() {
		if(stream.eof()) return (uint16_t)NULL;
		uint16_t value;
		switch(type) {
			case Text:
				char c;
				stream >> value >> c;
				break;
			case Binary:
				stream.read((char*)&value, sizeof(value));
				break;
		}
		return value;
	}
	//Write uint8 to stream
	template <> void Stream::Write<uint16_t>(const uint16_t& data) {
		switch(type) {
			case Text:
				stream << data << ';';
				break;
			case Binary:
				stream.write((char*)&data, sizeof(data));
				break;
		}
	}
	//Read uint from stream
	template <> uint32_t Stream::Read<uint32_t>() {
		if(stream.eof()) return (uint32_t)NULL;
		uint32_t value;
		switch(type) {
			case Text:
				char c;
				stream >> value >> c;
				break;
			case Binary:
				stream.read((char*)&value, sizeof(value));
				break;
		}
		return value;
	}
	//Write uint to stream
	template <> void Stream::Write<uint32_t>(const uint32_t& data) {
		switch(type) {
			case Text:
				stream << data << ';';
				break;
			case Binary:
				stream.write((char*)&data, sizeof(data));
				break;
		}
	}
	//Read int from stream
	template <> int Stream::Read<int>() {
		if(stream.eof()) return (int)NULL;
		int value;
		switch(type) {
			case Text:
				char c;
				stream >> value >> c;
				break;
			case Binary:
				stream.read((char*)&value, sizeof(value));
				break;
		}
		return value;
	}
	//Write int to stream
	template <> void Stream::Write<int>(const int& data) {
		switch(type) {
			case Text:
				stream << data << ';';
				break;
			case Binary:
				stream.write((char*)&data, sizeof(data));
				break;
		}
	}
	//Read long from stream
	template <> long Stream::Read<long>() {
		if(stream.eof()) return (long)NULL;
		long value;
		switch(type) {
			case Text:
				char c;
				stream >> value >> c;
				break;
			case Binary:
				stream.read((char*)&value, sizeof(value));
				break;
		}
		return value;
	}
	//Write long to stream
	template <> void Stream::Write<long>(const long& data) {
		switch(type) {
			case Text:
				stream << data << ';';
				break;
			case Binary:
				stream.write((char*)&data, sizeof(data));
				break;
		}
	}
	//Read long from stream
	template <> uint64_t Stream::Read<uint64_t>() {
		if(stream.eof()) return (uint64_t)NULL;
		long value;
		switch(type) {
			case Text:
				char c;
				stream >> value >> c;
				break;
			case Binary:
				stream.read((char*)&value, sizeof(value));
				break;
		}
		return value;
	}
	//Write long to stream
	template <> void Stream::Write<uint64_t>(const uint64_t& data) {
		switch(type) {
			case Text:
				stream << data << ';';
				break;
			case Binary:
				stream.write((char*)&data, sizeof(data));
				break;
		}
	}
	//Read float from stream
	template <> float Stream::Read<float>() {
		if(stream.eof()) return (float)NULL;
		float value;
		switch(type) {
			case Text:
				char c;
				stream >> value >> c;
				break;
			case Binary:
				stream.read((char*)&value, sizeof(value));
				break;
		}
		return value;
	}
	//Write float to stream
	template <> void Stream::Write<float>(const float& data) {
		switch(type) {
			case Text:
				stream << data << ';';
				break;
			case Binary:
				stream.write((char*)&data, sizeof(data));
				break;
		}
	}
	//Read double from stream
	template <> double Stream::Read<double>() {
		if(stream.eof()) return (double)NULL;
		double value;
		switch(type) {
			case Text:
				char c;
				stream >> value >> c;
				break;
			case Binary:
				stream.read((char*)&value, sizeof(value));
				break;
		}
		return value;
	}
	//Write double to stream
	template <> void Stream::Write<double>(const double& data) {
		switch(type) {
			case Text:
				stream << data << ';';
				break;
			case Binary:
				stream.write((char*)&data, sizeof(data));
				break;
		}
	}
}