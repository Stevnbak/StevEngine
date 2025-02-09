#include "Serializable.hpp"
#include "data/DataManager.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

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
	//Read char from text stream
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
	//Write char to text stream
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
	//Read string from stream
	template <> std::string Stream::Read<std::string>() {
		std::string value;
		getline(stream, value, '\000');
		return value;
	}

	//Write string to text stream
	template <> void Stream::Write<std::string>(const std::string& data) {
		switch(type) {
			case Text:
				stream << data << '\000';
				break;
			case Binary:
				stream.write((char*)&data, sizeof(data));
				stream << '\000';
				break;
		}
	}
	//Read uint8 from text stream
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
	//Write uint8 to text stream
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
	//Read uint from text stream
	template <> uint Stream::Read<uint>() {
		if(stream.eof()) return (uint)NULL;
		uint value;
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
	//Write uint to text stream
	template <> void Stream::Write<uint>(const uint& data) {
		switch(type) {
			case Text:
				stream << data << ';';
				break;
			case Binary:
				stream.write((char*)&data, sizeof(data));
				break;
		}
	}
	//Read int from text stream
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
	//Write int to text stream
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
	//Read long from text stream
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
	//Write long to text stream
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
	//Read long from text stream
	template <> ulong Stream::Read<ulong>() {
		if(stream.eof()) return (ulong)NULL;
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
	//Write long to text stream
	template <> void Stream::Write<ulong>(const ulong& data) {
		switch(type) {
			case Text:
				stream << data << ';';
				break;
			case Binary:
				stream.write((char*)&data, sizeof(data));
				break;
		}
	}
	//Read float from text stream
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
	//Write float to text stream
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
	//Read double from text stream
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
	//Write double to text stream
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