#pragma once
#include <uuidv7.h>
#include <string>
#include <stdint.h>

namespace StevEngine::Utilities {
	class ID {
		public:
			ID ();
			ID (const ID& other);
			ID (uint8_t* raw);
			ID (std::string string);
			std::string GetString() const { return std::string(string); }
			ID& operator=(const ID& other);
			bool operator==(const ID& other) const;
			bool operator<(const ID& other) const;
			bool operator() (const ID& lhs, const ID& rhs) const;
			bool IsNull() const;
			static ID empty;
		private:
			uint8_t raw[16];
			std::string string;
	};
}

#include <yaml-cpp/yaml.h>
namespace YAML {
	template<> struct convert<StevEngine::Utilities::ID> {
		static Node encode(const StevEngine::Utilities::ID& rhs);
		static bool decode(const Node& node, StevEngine::Utilities::ID& rhs);
	};
}

template <>
struct std::hash<StevEngine::Utilities::ID>
{
	std::size_t operator()(const StevEngine::Utilities::ID& k) const
	{
		return (std::hash<std::string>()(k.GetString()));
	}
};