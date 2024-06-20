#pragma once
#include <core/Log.hpp>
#include <tinyxml2.h>


namespace StevEngine {
    using ID = uint16_t;

	class GameObject;
	class Scene;

	class Component {
		friend class GameObject;
		//Properties
		public:
			static const bool unique = false;
		private:
			ID gameObject;
			std::string scene;
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
			virtual void Draw() = 0;
			void SetObject(ID object, std::string scene);
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