#pragma once
#include <SDL2/SDL.h>
#include <utilities/Vector3.hpp>
#include <utilities/Quaternion.hpp>
#include <scenes/Component.hpp>
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
		private:
			void Update(double deltaTime) {};
			void Draw();
			void Deactivate() {};
			void Start() {};
			void Export(tinyxml2::XMLElement* element);
	};
}