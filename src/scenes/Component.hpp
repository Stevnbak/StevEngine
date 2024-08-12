#pragma once
#include <main/Log.hpp>
#include <utilities/ID.hpp>
#include <tinyxml2.h>
#include <glm/mat4x4.hpp>

namespace StevEngine {
	class GameObject;
	class Scene;

	class Component {
		friend class GameObject;
		//Properties
		private:
			Utilities::ID gameObject = Utilities::ID::empty;
			std::string scene;
			static const bool unique = false;
		protected:
		    std::string type;
		//Functions
		public:
			Component(std::string type);
			Component(tinyxml2::XMLElement* node);
			virtual ~Component();
			GameObject* GetParent();
			Scene* GetScene();
		private:
			virtual void Start() = 0;
			virtual void Deactivate() = 0;
			virtual void Update(double deltaTime) = 0;
			virtual void Draw(glm::mat4x4 transform) = 0;
			void SetObject(Utilities::ID object, std::string scene);
			virtual void TransformUpdate(bool position, bool rotation, bool scale) {};
			virtual void Export(tinyxml2::XMLElement* element) = 0;
			std::string Export();
	};

    class FactoryBase {
		public:
			virtual Component* create(tinyxml2::XMLElement* node) const = 0;
	};
	template <class T> class ComponentFactory : public FactoryBase {
		public:
			Component* create(tinyxml2::XMLElement* node) const {
				return new T(node);
			}
	};
}
