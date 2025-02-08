#include "Serializable.hpp"
#include "data/DataManager.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

namespace StevEngine {
	//Write stream to file
	#ifdef StevEngine_PLAYER_DATA
	void TextSerializableStream::WriteToFile(const char* name) const {
		std::ofstream file;
		file.open(Data::data.GetAppdataPath() + name);
		file << stream.str();
	}

	void BinarySerializableStream::WriteToFile(const char* name) const {
		std::ofstream file;
		file.open(Data::data.GetAppdataPath() + name, std::ios::out|std::ios::binary);
		file << stream.str();
	}
	#endif

	//Read from file
	void TextSerializableStream::ReadFromFile(const Resources::Resource& file) {
		const char* data = file.GetRawData();
		size_t size = file.GetSize();
		for(int i = 0; i < size; i++) *this << (char)data[i];
	}

	void BinarySerializableStream::ReadFromFile(const Resources::Resource& file) {
		const char* data = file.GetRawData();
		size_t size = file.GetSize();
		for(int i = 0; i < size; i++) *this << (char)data[i];
	}


	//Write text stream to text stream
	template <> void TextSerializableStream::Write(const TextSerializableStream& data) {
		stream << data.stream.str();
	}
	//Read char from text stream
	template <> char TextSerializableStream::Read<char>() {
		if(stream.eof()) return (char)NULL;
		char value;
		stream.read(&value, sizeof(char));
		return value;
	}
	//Write char to text stream
	template <> void TextSerializableStream::Write<char>(const char& data) {
		stream << data;
	}
	//Write string to text stream
	template <> void TextSerializableStream::Write<std::string>(const std::string& data) {
		stream << data;
	}
	//Read uint8 from text stream
	template <> uint8_t TextSerializableStream::Read<uint8_t>() {
		if(stream.eof()) return (uint8_t)NULL;
		uint8_t value;
		stream.read((char*)&value, sizeof(uint8_t));
		return value;
	}
	//Write uint8 to text stream
	template <> void TextSerializableStream::Write<uint8_t>(const uint8_t& data) {
		stream << data;
	}
	//Read uint from text stream
	template <> uint TextSerializableStream::Read<uint>() {
		if(stream.eof()) return (uint)NULL;
		uint value;
		stream >> value;
		return value;
	}
	//Write uint to text stream
	template <> void TextSerializableStream::Write<uint>(const uint& data) {
		stream << data;
	}
	//Read int from text stream
	template <> int TextSerializableStream::Read<int>() {
		if(stream.eof()) return (int)NULL;
		int value;
		stream >> value;
		return value;
	}
	//Write int to text stream
	template <> void TextSerializableStream::Write<int>(const int& data) {
		stream << data;
	}
	//Read long from text stream
	template <> long TextSerializableStream::Read<long>() {
		if(stream.eof()) return (long)NULL;
		long value;
		stream >> value;
		return value;
	}
	//Write long to text stream
	template <> void TextSerializableStream::Write<long>(const long& data) {
		stream << data;
	}
	//Read long from text stream
	template <> ulong TextSerializableStream::Read<ulong>() {
		if(stream.eof()) return (ulong)NULL;
		long value;
		stream >> value;
		return value;
	}
	//Write long to text stream
	template <> void TextSerializableStream::Write<ulong>(const ulong& data) {
		stream << data;
	}
	//Read float from text stream
	template <> float TextSerializableStream::Read<float>() {
		if(stream.eof()) return (float)NULL;
		float value;
		stream >> value;
		return value;
	}
	//Write float to text stream
	template <> void TextSerializableStream::Write<float>(const float& data) {
		stream << data;
	}
	//Read double from text stream
	template <> double TextSerializableStream::Read<double>() {
		if(stream.eof()) return (double)NULL;
		double value;
		stream >> value;
		return value;
	}
	//Write double to text stream
	template <> void TextSerializableStream::Write<double>(const double& data) {
		stream << data;
	}

	//Write binary stream to binary stream
	template <> void BinarySerializableStream::Write(const BinarySerializableStream& data) {
		stream << data.stream.str();
	}
	//Read char from text stream
	template <> char BinarySerializableStream::Read<char>() {
		if(stream.eof()) return (char)NULL;
		char value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write char to text stream
	template <> void BinarySerializableStream::Write<char>(const char& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Write string to text stream
	template <> void BinarySerializableStream::Write<std::string>(const std::string& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Read uint8 from text stream
	template <> uint8_t BinarySerializableStream::Read<uint8_t>() {
		if(stream.eof()) return (uint8_t)NULL;
		uint8_t value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write uint8 to text stream
	template <> void BinarySerializableStream::Write<uint8_t>(const uint8_t& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Read uint from binary stream
	template <> uint BinarySerializableStream::Read<uint>() {
		if(stream.eof()) return (uint)NULL;
		uint value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write uint to binary stream
	template <> void BinarySerializableStream::Write<uint>(const uint& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Read int from binary stream
	template <> int BinarySerializableStream::Read<int>() {
		if(stream.eof()) return (int)NULL;
		int value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write int to binary stream
	template <> void BinarySerializableStream::Write<int>(const int& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Read long from binary stream
	template <> long BinarySerializableStream::Read<long>() {
		if(stream.eof()) return (long)NULL;
		long value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write long to binary stream
	template <> void BinarySerializableStream::Write<long>(const long& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Read long from binary stream
	template <> ulong BinarySerializableStream::Read<ulong>() {
		if(stream.eof()) return (ulong)NULL;
		long value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write long to binary stream
	template <> void BinarySerializableStream::Write<ulong>(const ulong& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Read float from binary stream
	template <> float BinarySerializableStream::Read<float>() {
		if(stream.eof()) return (float)NULL;
		float value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write float to binary stream
	template <> void BinarySerializableStream::Write<float>(const float& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Read double from binary stream
	template <> double BinarySerializableStream::Read<double>() {
		if(stream.eof()) return (double)NULL;
		double value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write double to binary stream
	template <> void BinarySerializableStream::Write<double>(const double& data) {
		stream.write((char const*)(&data), sizeof(data));
	}
}