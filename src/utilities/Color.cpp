#include "Color.hpp"

namespace YAML {
	Node convert<StevEngine::Utilities::Color>::encode(const StevEngine::Utilities::Color& rhs) {
		Node node;
		node.push_back((unsigned int)rhs.r);
		node.push_back((unsigned int)rhs.g);
		node.push_back((unsigned int)rhs.b);
		node.push_back((unsigned int)rhs.a);
		return node;
	}
	bool convert<StevEngine::Utilities::Color>::decode(const Node& node, StevEngine::Utilities::Color& rhs) {
		if(!node.IsSequence() || node.size() != 4) return false;
		rhs.r = node[0].as<unsigned int>();
		rhs.g = node[1].as<unsigned int>();
		rhs.b = node[2].as<unsigned int>();
		rhs.a = node[3].as<unsigned int>();
		return true;
	}
}
