#pragma once
#ifdef StevEngine_RENDERER_GL
#include "Object.hpp"
#include "main/Component.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Quaternion.hpp"
#include "visuals/shaders/Shader.hpp"

#include <SDL.h>
#include <glad/gl.h>

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
			 */
			RenderComponent(const Object& object);

			/**
			 * @brief Create render component with transform
			 * @param object Renderable object
			 * @param position Local position offset
			 * @param rotation Local rotation offset
			 * @param scale Local scale modifier
			 */
			RenderComponent(const Object& object, Utilities::Vector3 position, Utilities::Quaternion rotation, Utilities::Vector3 scale);

			/**
			 * @brief Create from serialized data
			 * @param object Renderable object
			 * @param stream Stream containing serialized component data
			 */
			RenderComponent(const Object& object, Stream& stream);

			/**
			 * @brief Get component type
			 * @return Type identifier string
			 */
			virtual std::string GetType() const { return "RenderComponent"; }


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
			 * @brief Serialize component to a stream
			 * @param type Type of stream to export to
			 * @return Serialized stream
			 */
			virtual Stream Export(StreamType type) const;

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
