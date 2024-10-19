#pragma once
#include "main/EventSystem.hpp"
#include "main/Log.hpp"
#include "utilities/ID.hpp"
#include "utilities/Matrix4.hpp"

#include <yaml-cpp/yaml.h>

#include <sys/types.h>
#include <functional>
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
			GameObject* GetParent() const;
			Scene* GetScene() const;
			std::string GetType() const { return type; }
			virtual YAML::Node Export(YAML::Node node) const;
			YAML::Node Export() const;
			Component(YAML::Node node);
		private:
			virtual void Start() {};
			virtual void Deactivate() {};
			virtual void Update(double deltaTime) {};
			#ifdef StevEngine_SHOW_WINDOW
			virtual void Draw(const Utilities::Matrix4& transform) {};
			#endif
			void SetObject(GameObject* object, std::string scene);
		//Events
		protected:
			std::vector<std::pair<Utilities::ID, std::string>> handlers;
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
