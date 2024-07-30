#pragma once
#ifdef StevEngine_RENDERER_GL
#include <scenes/Component.hpp>

#include "visuals/render/Component.hpp"
#include "visuals/render/Object.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace StevEngine {
    namespace Visuals {
        class ModelRenderer : public Render::RenderComponent {
			friend class StevEngine::GameObject;

            public:
                ModelRenderer(std::string pathm);
				ModelRenderer(tinyxml2::XMLElement* element);
            private:
                const aiScene* scene;
                std::string path;
				void Export(tinyxml2::XMLElement* element);
        };
    }
}
#endif
