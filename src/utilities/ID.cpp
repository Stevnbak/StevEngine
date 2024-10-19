#include "ID.hpp"

#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

uint8_t last[16];
namespace StevEngine::Utilities {
	uint8_t e[16];
	ID ID::empty = ID(e);

	ID::ID () {
		uint64_t unix_ts_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		uint8_t rand_bytes[10];
		for (int i = 0; i < 10; i++) {
			rand_bytes[i] = rand();
		}

		uuidv7_generate(
			raw,
			unix_ts_ms,
			rand_bytes,
			last
		);

		char text[37];
		uuidv7_to_string(raw, text);
		string = std::string(text);
	}
	ID::ID (const ID& other) {
		string = other.string;
		for(int i = 0; i < 16; i++)
			raw[i] = other.raw[i];
	}
	ID::ID (uint8_t* raw) {
		for(int i = 0; i < 16; i++)
			this->raw[i] = raw[i];
	}
	ID::ID (std::string string) {
		this->string = string;
		uuidv7_from_string(string.c_str(), raw);
	}

	bool ID::IsNull() const {
		return (this->string == empty.string);
	}
	ID& ID::operator=(const ID& other) {
		for(int i = 0; i < 16; i++)
			raw[i] = other.raw[i];
		string = other.string;
		return *this;
	}
	bool ID::operator==(const ID& other) const {
		return (string == other.string);
	}
	bool ID::operator<(const ID& other) const {
		return (string < other.string);
	}
	bool ID::operator() (const ID& lhs, const ID& rhs) const {
		return lhs.string < rhs.string;
	}
}

namespace YAML {
	Node convert<StevEngine::Utilities::ID>::encode(const StevEngine::Utilities::ID& rhs) {
		Node node;
		node = rhs.GetString();
		return node;
	}
	bool convert<StevEngine::Utilities::ID>::decode(const Node& node, StevEngine::Utilities::ID& rhs) {
		if(!node.IsDefined()) return false;
		rhs = StevEngine::Utilities::ID(node.as<std::string>());
		return true;
	}
}
