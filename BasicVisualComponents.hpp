#pragma once
#include <SDL2/include/SDL.h>
#include "Utilities.hpp"
#include "GameObject.hpp"
namespace StevEngine {
	enum PrimitiveType {
		None,
		Quad,
		Cube,
		Circle,
		Sphere,
		Cylinder,
		Capsule
	};
	class Primitive : public Component {
		public:
			void Update(double deltaTime) {}
			void Draw();
			//Basic properties
			Utilities::Vector3d position = Utilities::Vector3d();
			Utilities::Vector3d rotation = Utilities::Vector3d();
			Utilities::Vector3d scale = Utilities::Vector3d(1, 1, 1);
			PrimitiveType type = (PrimitiveType)0;
			SDL_Color colour = SDL_Color(1,1,1,1);
			bool unique = false;
	};
}