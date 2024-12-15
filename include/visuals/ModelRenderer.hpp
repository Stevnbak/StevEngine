#pragma once
#ifdef StevEngine_RENDERER_GL
#include "main/Component.hpp"
#include "utilities/Model.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Vector3.hpp"
#include "visuals/renderer/Object.hpp"
#include "visuals/Material.hpp"
#include <cstddef>

namespace StevEngine::Visuals {
	class ModelRenderer : public Component {
		public:
			ModelRenderer(const Utilities::Model& model, const Material& material = Material(), Utilities::Vector3 position = Utilities::Vector3(), Utilities::Quaternion rotation = Utilities::Quaternion(), Utilities::Vector3 scale = Utilities::Vector3(1));
			ModelRenderer(YAML::Node node);
			//Basic properties
			Utilities::Vector3 position = Utilities::Vector3();
			Utilities::Quaternion rotation = Utilities::Quaternion();
			Utilities::Vector3 scale = Utilities::Vector3(1);
			//Functions
			size_t MeshCount() const { return objects.size(); }
			Renderer::Object& GetObject(uint index) { return objects.at(index); };
			Visuals::Material& GetMaterials(uint index) { return GetObject(index).material; };
			void AddShader(Renderer::ShaderProgram program);
			void RemoveShader(Renderer::ShaderType type);
			YAML::Node Export(YAML::Node node) const;
			void Draw(const Utilities::Matrix4& transform);
			~ModelRenderer();
		private:
			std::vector<Renderer::Object> objects;
			Utilities::Model model;
			std::map<Renderer::ShaderType, Renderer::ShaderProgram> shaders;
	};
	inline bool modelRenderer = CreateComponents::RegisterComponentType<ModelRenderer>("ModelRenderer");
}
#endif
