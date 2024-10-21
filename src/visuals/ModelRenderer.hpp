#pragma once
#ifdef StevEngine_RENDERER_GL
#include "main/Component.hpp"
#include "utilities/Model.hpp"
#include "visuals/renderer/RenderComponent.hpp"
#include "visuals/renderer/Object.hpp"
#include "visuals/Material.hpp"

namespace StevEngine::Visuals {
	class ModelRenderer : public Renderer::RenderComponent {
		friend class StevEngine::GameObject;

		public:
			ModelRenderer(const Utilities::Model& model, const Material& material = Material());
			ModelRenderer(YAML::Node node);
			YAML::Node Export(YAML::Node node) const;
		private:
			Utilities::Model model;
	};
	inline bool modelRenderer = CreateComponents::RegisterComponentType<ModelRenderer>("ModelRenderer");
}
#endif
