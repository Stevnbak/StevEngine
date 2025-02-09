#include "Serializable.hpp"
#include "data/DataManager.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

namespace StevEngine {
	//Write stream to file
	#ifdef StevEngine_PLAYER_DATA
	void TextStream::WriteToFile(const char* name) const {
		std::ofstream file;
		file.open(Data::data.GetAppdataPath() + name);
		file << stream.str();
	}

	void BinaryStream::WriteToFile(const char* name) const {
		std::ofstream file;
		file.open(Data::data.GetAppdataPath() + name, std::ios::out|std::ios::binary);
		file << stream.str();
	}
	#endif

	//Read from file
	void TextStream::ReadFromFile(const Resources::Resource& file) {
		const char* data = file.GetRawData();
		size_t size = file.GetSize();
		for(int i = 0; i < size; i++) *this << (char)data[i];
	}

	void BinaryStream::ReadFromFile(const Resources::Resource& file) {
		const char* data = file.GetRawData();
		size_t size = file.GetSize();
		for(int i = 0; i < size; i++) *this << (char)data[i];
	}


	//Write text stream to text stream
	template <> void TextStream::Write(const TextStream& data) {
		stream << data.stream.str();
	}
	//Read char from text stream
	template <> char TextStream::Read<char>() {
		if(stream.eof()) return (char)NULL;
		char value;
		stream.read(&value, sizeof(char));
		return value;
	}
	//Write char to text stream
	template <> void TextStream::Write<char>(const char& data) {
		stream << data;
	}
	//Write string to text stream
	template <> std::string TextStream::Read<std::string>() {
		std::string value;
		getline(stream, value, '\000');
		return value;
	}
	//Write string to text stream
	template <> void TextStream::Write<std::string>(const std::string& data) {
		stream << data << '\000';
	}
	//Read uint8 from text stream
	template <> uint8_t TextStream::Read<uint8_t>() {
		if(stream.eof()) return (uint8_t)NULL;
		uint8_t value;
		stream.read((char*)&value, sizeof(uint8_t));
		char c; stream >> c; //Delimiter
		return value;
	}
	//Write uint8 to text stream
	template <> void TextStream::Write<uint8_t>(const uint8_t& data) {
		stream << data;
		stream << ';'; //Delimiter
	}
	//Read uint from text stream
	template <> uint TextStream::Read<uint>() {
		if(stream.eof()) return (uint)NULL;
		uint value;
		stream >> value;
		char c; stream >> c; //Delimiter
		return value;
	}
	//Write uint to text stream
	template <> void TextStream::Write<uint>(const uint& data) {
		stream << data;
		stream << ';'; //Delimiter
	}
	//Read int from text stream
	template <> int TextStream::Read<int>() {
		if(stream.eof()) return (int)NULL;
		int value;
		stream >> value;
		char c; stream >> c; //Delimiter
		return value;
	}
	//Write int to text stream
	template <> void TextStream::Write<int>(const int& data) {
		stream << data;
		stream << ';'; //Delimiter
	}
	//Read long from text stream
	template <> long TextStream::Read<long>() {
		if(stream.eof()) return (long)NULL;
		long value;
		stream >> value;
		char c; stream >> c; //Delimiter
		return value;
	}
	//Write long to text stream
	template <> void TextStream::Write<long>(const long& data) {
		stream << data;
		stream << ';'; //Delimiter
	}
	//Read long from text stream
	template <> ulong TextStream::Read<ulong>() {
		if(stream.eof()) return (ulong)NULL;
		long value;
		stream >> value;
		char c; stream >> c; //Delimiter
		return value;
	}
	//Write long to text stream
	template <> void TextStream::Write<ulong>(const ulong& data) {
		stream << data;
		stream << ';'; //Delimiter
	}
	//Read float from text stream
	template <> float TextStream::Read<float>() {
		if(stream.eof()) return (float)NULL;
		float value;
		stream >> value;
		char c; stream >> c; //Delimiter
		return value;
	}
	//Write float to text stream
	template <> void TextStream::Write<float>(const float& data) {
		stream << data;
		stream << ';'; //Delimiter
	}
	//Read double from text stream
	template <> double TextStream::Read<double>() {
		if(stream.eof()) return (double)NULL;
		double value;
		stream >> value;
		char c; stream >> c; //Delimiter
		return value;
	}
	//Write double to text stream
	template <> void TextStream::Write<double>(const double& data) {
		stream << data;
	}

	//Write binary stream to binary stream
	template <> void BinaryStream::Write(const BinaryStream& data) {
		stream << data.stream.str();
	}
	//Read char from text stream
	template <> char BinaryStream::Read<char>() {
		if(stream.eof()) return (char)NULL;
		char value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write char to text stream
	template <> void BinaryStream::Write<char>(const char& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Write string to text stream
	template <> std::string BinaryStream::Read<std::string>() {
		std::string value;
		getline(stream, value, '\000');
		return value;
	}
	//Write string to text stream
	template <> void BinaryStream::Write<std::string>(const std::string& data) {
		stream.write((char const*)&data, sizeof(data));
		stream << '\000';
	}
	//Read uint8 from text stream
	template <> uint8_t BinaryStream::Read<uint8_t>() {
		if(stream.eof()) return (uint8_t)NULL;
		uint8_t value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write uint8 to text stream
	template <> void BinaryStream::Write<uint8_t>(const uint8_t& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Read uint from binary stream
	template <> uint BinaryStream::Read<uint>() {
		if(stream.eof()) return (uint)NULL;
		uint value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write uint to binary stream
	template <> void BinaryStream::Write<uint>(const uint& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Read int from binary stream
	template <> int BinaryStream::Read<int>() {
		if(stream.eof()) return (int)NULL;
		int value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write int to binary stream
	template <> void BinaryStream::Write<int>(const int& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Read long from binary stream
	template <> long BinaryStream::Read<long>() {
		if(stream.eof()) return (long)NULL;
		long value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write long to binary stream
	template <> void BinaryStream::Write<long>(const long& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Read long from binary stream
	template <> ulong BinaryStream::Read<ulong>() {
		if(stream.eof()) return (ulong)NULL;
		long value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write long to binary stream
	template <> void BinaryStream::Write<ulong>(const ulong& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Read float from binary stream
	template <> float BinaryStream::Read<float>() {
		if(stream.eof()) return (float)NULL;
		float value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write float to binary stream
	template <> void BinaryStream::Write<float>(const float& data) {
		stream.write((char const*)&data, sizeof(data));
	}
	//Read double from binary stream
	template <> double BinaryStream::Read<double>() {
		if(stream.eof()) return (double)NULL;
		double value;
		stream.read((char*)&value, sizeof(value));
		return value;
	}
	//Write double to binary stream
	template <> void BinaryStream::Write<double>(const double& data) {
		stream.write((char const*)(&data), sizeof(data));
	}
}