#pragma once
#ifdef StevEngine_RENDERER_GL
#include "main/Component.hpp"
#include "utilities/Vector3.hpp"
#include "visuals/shaders/ShaderProgram.hpp"

#define DIRECTIONAL_LIGHT_TYPE "DirectionalLight"
#define POINT_LIGHT_TYPE "PointLight"
#define SPOT_LIGHT_TYPE "SpotLight"

namespace StevEngine::Visuals {
	/**
	 * @brief Base class for light components
	 *
	 * Provides common functionality for different types of lights.
	 * Handles shader updates and light properties.
	 */
	class Light : public Component {
		public:
			Utilities::Vector3 diffuse;	 ///< Diffuse light color
			Utilities::Vector3 specular;	///< Specular light color

			/**
			 * @brief Update shader uniforms for this light
			 * @param program Shader program to update
			 */
			virtual void UpdateShader(const Renderer::ShaderProgram& program) const = 0;

			/**
			 * @brief Reset shader uniforms for this light
			 * @param program Shader program to reset
			 */
			virtual void ResetShader(const Renderer::ShaderProgram& program) const = 0;

			/**
			 * @brief Get shader light ID
			 * @return Light index in shader
			 */
			uint32_t GetShaderLightID() const { return shaderLightID; }

		protected:
			Light(uint32_t shaderID, Utilities::Vector3 diffuse, Utilities::Vector3 specular, std::string type);
			Light(Stream& stream, std::string type);
			const uint32_t shaderLightID;  ///< Light index in shader
			virtual ~Light();
	};

	/**
	 * @brief Directional light for parallel light rays
	 *
	 * Simulates light from an infinitely distant source like the sun.
	 * Direction matters but position does not affect lighting.
	 */
	class DirectionalLight final : public Light {
		public:
			/**
			 * @brief Create directional light
			 * @param diffuse Diffuse light color
			 * @param specular Specular light color
			 */
			DirectionalLight(Utilities::Vector3 diffuse = Utilities::Vector3(1.0), Utilities::Vector3 specular = Utilities::Vector3(1.0));

			/**
			 * @brief Create directional light from text serialized data
			 * @param stream Stream containing serialized component data
			 */
			DirectionalLight(Stream& stream);

			/**
			 * @brief Get component type
			 * @return Type identifier string
			 */
			std::string GetType() const { return DIRECTIONAL_LIGHT_TYPE; }

			/**
			 * @brief Serialize component to a stream
			 * @param type Type of stream to export to
			 * @return Serialized stream
			 */
			Stream Export(StreamType type) const;

			void UpdateShader(const Renderer::ShaderProgram& program) const;
			void ResetShader(const Renderer::ShaderProgram& program) const;
			~DirectionalLight();
	};

	/**
	 * @brief Point light for omni-directional lighting
	 *
	 * Light emanates in all directions from a point.
	 * Intensity diminishes with distance based on attenuation.
	 */
	class PointLight final : public Light {
		public:
			/**
			 * @brief Create point light
			 * @param diffuse Diffuse light color
			 * @param specular Specular light color
			 * @param constant Constant attenuation factor
			 * @param linear Linear attenuation factor
			 * @param quadratic Quadratic attenuation factor
			 */
			PointLight(Utilities::Vector3 diffuse = Utilities::Vector3(1.0), Utilities::Vector3 specular = Utilities::Vector3(1.0), float constant = 1.0, float linear = 0.02, float quadratic = 0.0017);

			/**
			 * @brief Create point light from text serialized data
			 * @param stream Stream containing serialized component data
			 */
			PointLight(Stream& stream);

			/**
			 * @brief Get component type
			 * @return Type identifier string
			 */
			std::string GetType() const { return POINT_LIGHT_TYPE; }

			/**
			 * @brief Serialize component to a stream
			 * @param type Type of stream to export to
			 * @return Serialized stream
			 */
			Stream Export(StreamType type) const;

			float constant;	///< Constant attenuation factor
			float linear;	 ///< Linear attenuation factor
			float quadratic;  ///< Quadratic attenuation factor

			void UpdateShader(const Renderer::ShaderProgram& program) const;
			void ResetShader(const Renderer::ShaderProgram& program) const;
			~PointLight();
	};

	/**
	 * @brief Spot light for cone-shaped directional lighting
	 *
	 * Light emanates in a cone from a point.
	 * Has inner and outer cone angles for smooth edges.
	 */
	class SpotLight final : public Light {
		public:
			/**
			 * @brief Create spot light
			 * @param diffuse Diffuse light color
			 * @param specular Specular light color
			 * @param cutOff Inner cone angle in degrees
			 * @param outerCutOff Outer cone angle in degrees
			 */
			SpotLight(Utilities::Vector3 diffuse = Utilities::Vector3(1.0), Utilities::Vector3 specular = Utilities::Vector3(1.0), float cutOff = 12.5, float outerCutOff = 17.5);

			/**
			 * @brief Create spot light from text serialized data
			 * @param stream Stream containing serialized component data
			 */
			SpotLight(Stream& stream);

			/**
			 * @brief Get component type
			 * @return Type identifier string
			 */
			std::string GetType() const { return SPOT_LIGHT_TYPE; }

			/**
			 * @brief Serialize component to a stream
			 * @param type Type of stream to export to
			 * @return Serialized stream
			 */
			Stream Export(StreamType type) const;

			float cutOff;		///< Inner cone angle in degrees
			float outerCutOff;   ///< Outer cone angle in degrees

			void UpdateShader(const Renderer::ShaderProgram& program) const;
			void ResetShader(const Renderer::ShaderProgram& program) const;
			~SpotLight();
	};

	// Register light components
	inline bool dl = CreateComponents::RegisterComponentType<DirectionalLight>(DIRECTIONAL_LIGHT_TYPE);
	inline bool pl = CreateComponents::RegisterComponentType<PointLight>(POINT_LIGHT_TYPE);
	inline bool sl = CreateComponents::RegisterComponentType<SpotLight>(SPOT_LIGHT_TYPE);
}
#endif
