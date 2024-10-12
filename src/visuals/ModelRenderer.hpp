#pragma once
#ifdef StevEngine_RENDERER_GL
#include "scenes/Component.hpp"
#include "utilities/Model.hpp"

#include "visuals/render/Component.hpp"
#include "visuals/render/Object.hpp"

namespace StevEngine {
	namespace Visuals {
		class ModelRenderer : public Render::RenderComponent {
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
}
#endif
