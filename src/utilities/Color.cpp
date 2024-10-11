#include "Color.hpp"

namespace YAML {
	Node convert<StevEngine::Utilities::Color>::encode(const StevEngine::Utilities::Color& rhs) {
		Node node;
		node.push_back((uint32_t)rhs.r);
		node.push_back((uint32_t)rhs.g);
		node.push_back((uint32_t)rhs.b);
		node.push_back((uint32_t)rhs.a);
		return node;
	}
	bool convert<StevEngine::Utilities::Color>::decode(const Node& node, StevEngine::Utilities::Color& rhs) {
		if(!node.IsSequence() || node.size() != 4) return false;
		rhs.r = node[0].as<uint32_t>();
		rhs.g = node[1].as<uint32_t>();
		rhs.b = node[2].as<uint32_t>();
		rhs.a = node[3].as<uint32_t>();
		return true;
	}
}
