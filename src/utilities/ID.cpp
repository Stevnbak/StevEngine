#include "ID.hpp"
#include "utilities/Random.hpp"
#include "utilities/Stream.hpp"

#include <chrono>
#include <cstdint>
#include <stdlib.h>
#include <string>

bool seededRand = StevEngine::Utilities::SetRandomSeed();

uint8_t last[16];

namespace StevEngine::Utilities {
	uint8_t e[16];
	ID ID::empty = ID(e);

	const char* ID::GetString() const {
		char* newText = new char[37];
		sprintf(newText, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x", raw[0], raw[1], raw[2], raw[3], raw[4], raw[5], raw[6], raw[7], raw[8], raw[9], raw[10], raw[11], raw[12], raw[13], raw[14], raw[15]);
		return newText;
	}

	ID::ID() {
		uint64_t unix_ts_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		//fill full 128bits with random values
		for (int i = 0; i < 16; i++) {
			raw[i] = rand();
		}
		//timestamp (48 bits)
		for (uint8_t i = 0; i < 5; i++) {
			raw[i] = unix_ts_ms >> (40 - (i * 8)) & 0xFF;
		}
		//version and variant (16 bits, 10 are random)
		raw[6] = (raw[6] & 0x0F) | 0x70;
		raw[8] = (raw[8] & 0x3F) | 0x80;
	}

	ID::ID(const ID& other) {
		for (int i = 0; i < 16; i++) raw[i] = other.raw[i];
	}

	ID::ID(uint8_t* raw) {
		for (int i = 0; i < 16; i++) this->raw[i] = raw[i];
	}

	ID::ID(const char* text) {
		uint8_t r = 0;
		for (uint8_t i = 0; i < 36;) {
			char sz[3];
			sz[0] = text[i++];
			if (sz[0] == '-') continue;
			sz[1] = text[i++];
			sz[2] = '\000';
			uint8_t c = std::stoul(sz, NULL, 16);
			raw[r++] = c;
		}
	}

	bool ID::IsNull() const { return (*this == empty); }

	ID& ID::operator=(const ID& other) {
		for (int i = 0; i < 16; i++) raw[i] = other.raw[i];
		return *this;
	}

	bool ID::operator==(const ID& other) const {
		for (uint8_t i = 0; i < 16; i++) {
			if (raw[i] != other.raw[i]) return false;
		}
		return true;
	}

	bool ID::operator<(const ID& other) const {
		for (uint8_t i = 0; i < 16; i++) {
			if (raw[i] >= other.raw[i]) return false;
		}
		return true;
	}

	bool ID::operator()(const ID& lhs, const ID& rhs) const { return lhs < rhs; }

	//Read from stream
	template<> Utilities::ID Stream::Read<Utilities::ID>() {
		if (type == Utilities::StreamType::Text) {
			char text[37];
			for (int i = 0; i < 36; i++) *this >> text[i];
			text[36] = '\000';
			return Utilities::ID(text);
		}
		else {
			uint8_t value[16];
			for (int i = 0; i < 16; i++) *this >> value[i];
			return Utilities::ID(value);
		}
	}

	//Write to stream
	template<> void Stream::Write<Utilities::ID>(const Utilities::ID& data) {
		if (type == Utilities::StreamType::Text) {
			std::string str = data.GetString();
			for (int i = 0; i < 36; i++) *this << str[i];
		}
		else
			for (int i = 0; i < 16; i++) *this << data.GetRaw()[i];
	}
}
