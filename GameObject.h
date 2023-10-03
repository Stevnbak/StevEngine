#pragma once
#include "Utilities.h"
#include <map>

namespace StevEngine {
	class GameObject {
		public:
			Utilities::Vector3d position = Utilities::Vector3d();
			Utilities::Vector3d rotation = Utilities::Vector3d();
			Utilities::Vector3d scale = Utilities::Vector3d(1,1,1);
			void Update(double deltaTime);
			void Draw();
			//Constructors
			GameObject();
			GameObject(const GameObject& object);
			//Destructor
			void Destroy();
			~GameObject() {
				Destroy();
			}
			//Get map
			static std::map<int, GameObject*>& GetMapOfObjects() {
				return objects;
			};
		private:
			static int curID;
			static std::map<int, GameObject*> objects;
			int id;
	};
}