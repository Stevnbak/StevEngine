#pragma once
#include "utilities/Matrix4.hpp"
#ifdef StevEngine_SHOW_WINDOW
#include "main/Component.hpp"

#define CAMERA_TYPE "Camera"

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
				 * @brief Create camera from text serialized data
				 * @param stream Stream containing serialized component data
				 */
				Camera(Stream& stream);

				/**
				 * @brief Get component type
				 * @return Type identifier string
				 */
				std::string GetType() const { return CAMERA_TYPE; }

				/**
				 * @brief Serialize component to a text stream
				 * @param type Type of stream to export to
				 * @return Serialized stream
				 */
				Stream Export(StreamType type) const;

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
		inline bool camera = CreateComponents::RegisterComponentType<Camera>(CAMERA_TYPE);

	}
}
#endif
