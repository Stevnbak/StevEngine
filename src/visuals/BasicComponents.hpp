#pragma once
#include <SDL2/SDL.h>
#include <core/Utilities.hpp>
#include <core/Component.hpp>
namespace StevEngine {
	enum PrimitiveType {
		None,
		Cube,
		Sphere,
		Cylinder,
		Capsule
	};
	class Primitive : public Component {
		friend class StevEngine::GameObject;
		public:
			Primitive(PrimitiveType type);
			Primitive(tinyxml2::XMLElement* element);
			//Basic properties
			Utilities::Vector3 position = Utilities::Vector3();
			Utilities::Quaternion rotation = Utilities::Quaternion();
			Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1);
			PrimitiveType type = (PrimitiveType)0;
			SDL_Color colour = SDL_Color(1,1,1,1);
			static const bool unique = false;
		private:
			void Update(double deltaTime) {};
			void Draw();
			void Start() {};
			void Export(tinyxml2::XMLElement* element);
	};
}