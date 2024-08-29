#pragma once
#include "main/Log.hpp"
#include "utilities/ID.hpp"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/node.h"

#include <functional>
#include <glm/mat4x4.hpp>

#include <sys/types.h>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

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
			virtual ~Component();
			GameObject* GetParent();
			Scene* GetScene();
			virtual YAML::Node Export(YAML::Node node) const;
			YAML::Node Export() const;
			Component(YAML::Node node);
		private:
			virtual void Start() {};
			virtual void Deactivate() {};
			virtual void Update(double deltaTime) {};
			virtual void Draw(glm::mat4x4 transform) {};
			virtual void TransformUpdate(bool position, bool rotation, bool scale) {};
			void SetObject(Utilities::ID object, std::string scene);
	};

	class CreateComponents {
		static inline std::unordered_map<std::string, std::function<Component*(YAML::Node node)>> factories = std::unordered_map<std::string, std::function<Component*(YAML::Node node)>>();
		public:
			static Component* Create(YAML::Node node);
			template <class T> static bool RegisterComponentType(std::string type) {
				if(factories.contains(type)) return false;
				factories.insert({type, [](YAML::Node node) -> Component*  {
					return (Component*) (new T(node));
				}});
				return true;
			}
	};
}
