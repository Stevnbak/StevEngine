#pragma once
#include <SDL2/SDL.h>
#include <core/Utilities.hpp>
#include <core/GameObject.hpp>
namespace StevEngine {
	enum PrimitiveType {
		None,
		Cube,
		Sphere,
		Cylinder,
		Capsule
	};
	class Primitive : public Component {
		public:
			Primitive(PrimitiveType type);
			void Update(double deltaTime) {}
			void Draw();
			void Start() {}
			//Basic properties
			Utilities::Vector3d position = Utilities::Vector3d();
			Utilities::Rotation3d rotation = Utilities::Rotation3d();
			Utilities::Vector3d scale = Utilities::Vector3d(1, 1, 1);
			PrimitiveType type = (PrimitiveType)0;
			SDL_Color colour = SDL_Color(1,1,1,1);
			bool unique = false;
	};
}