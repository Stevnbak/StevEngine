#pragma once
#include "Utilities.h"
#include <map>

namespace StevEngine {
	class GameObject {
		public:
			Utilities::Vector3d position = Utilities::Vector3d();
			Utilities::Vector3d rotation = Utilities::Vector3d();
			Utilities::Vector3d scale = Utilities::Vector3d(1,1,1);
			void Start();
			void Update(double deltaTime);
			void Draw();
			//Constructors
			GameObject();
			GameObject(const GameObject& object);
			//Destructor
			void Destroy();
			//Get map
			static std::map<long, GameObject*>& GetMapOfObjects() {
				return objects;
			};
		private:
			static long curID;
			static std::map<long, GameObject*> objects;
			long id;
	};
}