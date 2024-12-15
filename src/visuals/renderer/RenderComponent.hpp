#pragma once
#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"
#include "main/Component.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Vertex.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Color.hpp"
#include "visuals/Texture.hpp"
#include "visuals/shaders/Shader.hpp"

#include <SDL.h>
#include <glad/gl.h>
#include <yaml-cpp/yaml.h>

namespace StevEngine::Renderer {
	/**
	 * @brief Base component for renderable objects
	 *
	 * Component that adds rendering capability to game objects.
	 * Handles transform offsets and shader management.
	 */
	class RenderComponent : public Component {
		public:
			/**
			 * @brief Create render component
			 * @param object Renderable object
			 * @param type Component type string
			 */
			RenderComponent(const Object& object, std::string type);

			/**
			 * @brief Create render component with transform
			 * @param object Renderable object
			 * @param position Local position offset
			 * @param rotation Local rotation offset
			 * @param scale Local scale modifier
			 * @param type Component type string
			 */
			RenderComponent(const Object& object, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale, std::string type);

			/**
			 * @brief Create from serialized data
			 * @param object Renderable object
			 * @param node YAML node with component data
			 */
			RenderComponent(const Object& object, YAML::Node node);


			Utilities::Vector3 position = Utilities::Vector3(); /// Local position offset
			Utilities::Quaternion rotation = Utilities::Quaternion(); /// Local rotation offset
			Utilities::Vector3 scale = Utilities::Vector3(1, 1, 1); /// Local scale modifier

			/**
			 * @brief Get renderable object
			 * @return Reference to object
			 */
			Object& GetObject() { return object; };

			/**
			 * @brief Get material
			 * @return Reference to material
			 */
			Visuals::Material& GetMaterial() { return object.material; };

			/**
			 * @brief Add shader program
			 * @param program Shader to add
			 */
			void AddShader(ShaderProgram program);

			/**
			 * @brief Remove shader of specified type
			 * @param type Type of shader to remove
			 */
			void RemoveShader(ShaderType type);

			/**
			 * @brief Clean up component resources
			 */
			~RenderComponent();

			/**
			 * @brief Serialize component to YAML
			 * @param node Node to serialize into
			 * @return Updated YAML node
			 */
			virtual YAML::Node Export(YAML::Node node) const;

		protected:
			/** @brief Renderable object */
			Object object;
			/** @brief Shader programs by type */
			std::map<ShaderType, ShaderProgram> shaders;

		private:
			/**
			 * @brief Draw component
			 * @param transform World transform matrix
			 */
			void Draw(const Utilities::Matrix4& transform);
	};
}
#endif
