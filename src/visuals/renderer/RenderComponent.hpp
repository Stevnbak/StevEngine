#pragma once
#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"
#include "main/Component.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Quaternion.hpp"
#include "visuals/shaders/Shader.hpp"

#include <SDL.h>
#include <glad/gl.h>
#include <yaml-cpp/yaml.h>

namespace StevEngine::Renderer {
	class RenderComponent : public Component {
		public:
			RenderComponent(const Object& object, std::string type);
			RenderComponent(const Object& object, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, std::string type);
			RenderComponent(const Object& object, YAML::Node node);
			//Basic properties
			Utilities::Vector3 position = Utilities::Vector3();
			Utilities::Quaternion rotation = Utilities::Quaternion();
			Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1);
			Object& GetObject() { return object; };
			Visuals::Material& GetMaterial() { return object.material; };
			void AddShader(ShaderProgram program);
			void RemoveShader(ShaderType type);
			~RenderComponent();
			virtual YAML::Node Export(YAML::Node node) const;
		public:
			Object object;
			std::map<ShaderType, ShaderProgram> shaders;
		//Main functions
		private:
			void Draw(const Utilities::Matrix4& transform);
	};
}
#endif
