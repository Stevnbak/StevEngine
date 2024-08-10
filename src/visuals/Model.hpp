#pragma once
#ifdef StevEngine_RENDERER_GL
#include <scenes/Component.hpp>
#include "utilities/Model.hpp"

#include "visuals/render/Component.hpp"
#include "visuals/render/Object.hpp"

namespace StevEngine {
    namespace Visuals {
        class ModelRenderer : public Render::RenderComponent {
			friend class StevEngine::GameObject;

            public:
                ModelRenderer(Utilities::Model model);
				ModelRenderer(tinyxml2::XMLElement* element);
            private:
                Utilities::Model model;
				void Export(tinyxml2::XMLElement* element);
        };
    }
}
#endif
