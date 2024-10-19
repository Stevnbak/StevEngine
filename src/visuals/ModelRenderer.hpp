#pragma once
#ifdef StevEngine_RENDERER_GL
#include "main/Component.hpp"
#include "utilities/Model.hpp"

#include "visuals/renderer/RenderComponent.hpp"
#include "visuals/renderer/Object.hpp"

namespace StevEngine::Visuals {
	class ModelRenderer : public Renderer::RenderComponent {
		friend class StevEngine::GameObject;

		public:
			ModelRenderer(const Utilities::Model& model, const Utilities::Color& color = Utilities::Color(255, 255, 255, 255), Visuals::Texture surface = Visuals::Texture::empty);
			ModelRenderer(YAML::Node node);
			YAML::Node Export(YAML::Node node) const;
		private:
			Utilities::Model model;
	};
	inline bool modelRenderer = CreateComponents::RegisterComponentType<ModelRenderer>("ModelRenderer");
}
#endif
