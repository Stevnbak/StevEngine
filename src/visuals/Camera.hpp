#pragma once
#include "utilities/Matrix4.hpp"
#ifdef StevEngine_SHOW_WINDOW
#include "main/Component.hpp"

namespace StevEngine {
	namespace Visuals {
		/**
		 * @brief Camera component for view and projection control
		 *
		 * Handles camera view matrix and projection settings.
		 * Supports both perspective and orthographic projections.
		 */
		class Camera : public Component {
			public:
				/**
				 * @brief Create new camera
				 * @param orthographic Whether to use orthographic projection
				 * @param fov Field of view in degrees (for perspective)
				 * @param zoomValue Zoom level
				 * @param nearClip Near clip plane distance
				 * @param farClip Far clip plane distance
				 */
				Camera(bool orthographic = false, float fov = 70, float zoomValue = 1, float nearClip = 0.01, float farClip = 1000);

				/**
				 * @brief Create from serialized data
				 * @param node YAML node containing camera data
				 */
				Camera(YAML::Node node);

				/**
				 * @brief Serialize camera to YAML
				 * @param node Node to serialize into
				 * @return Updated YAML node
				 */
				YAML::Node Export(YAML::Node node) const;

				/**
				 * @brief Get view matrix for camera
				 * @return View transformation matrix
				 */
				Utilities::Matrix4 GetView() const;

				/**
				 * @brief Get projection matrix for camera
				 * @return Projection matrix based on current settings
				 */
				Utilities::Matrix4 GetProjection() const;

				bool isOrthographic;	///< Whether using orthographic projection
				float zoom;			///< Current zoom level
				float fov;			 ///< Field of view in degrees
				float nearClip;		///< Near clip plane distance
				float farClip;		 ///< Far clip plane distance
				static const bool unique = true;  ///< Only one camera per GameObject
		};

		/* Register Camera as component type */
		inline bool camera = CreateComponents::RegisterComponentType<Camera>("Camera");

	}
}
#endif
