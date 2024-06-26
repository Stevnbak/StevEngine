#pragma once
#include "render/Component.hpp"

namespace StevEngine {
	namespace Visuals {
		class CubePrimitive : public Render::RenderComponent {
			public:
				CubePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
				CubePrimitive(tinyxml2::XMLElement* element);
		};
		class SpherePrimitive : public Render::RenderComponent {
			public:
				SpherePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
				SpherePrimitive(tinyxml2::XMLElement* element);
		};
		class CylinderPrimitive : public Render::RenderComponent {
			public:
				CylinderPrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
				CylinderPrimitive(tinyxml2::XMLElement* element);
		};
		class CapsulePrimitive : public Render::RenderComponent {
			public:
				CapsulePrimitive(Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1,1,1));
				CapsulePrimitive(tinyxml2::XMLElement* element);
		};
	}
}