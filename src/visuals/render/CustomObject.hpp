#pragma once
#ifdef StevEngine_RENDERER_GL

#include <glm/mat4x4.hpp>


namespace StevEngine {
    namespace Render {
        class CustomObject {
            public:
                CustomObject() {}
                virtual void Draw(glm::mat4x4 transform) = 0;
		};
    }
}
#endif