#ifdef StevEngine_SHOW_WINDOW
#include "Camera.hpp"
#include "main/Engine.hpp"
#include "scenes/Component.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Quaternion.hpp"
#include "main/Log.hpp"
#include "scenes/GameObject.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
using namespace StevEngine;
using namespace StevEngine::Utilities;

namespace StevEngine::Visuals {
	Camera::Camera(bool orthographic, double fov, double zoomValue)
		: Component("Camera"), isOrthographic(orthographic), fov(fov), zoom(zoomValue) {}

	Camera::Camera(YAML::Node node)
		: Component(node),
		isOrthographic(node["orthographic"].as<bool>()),
		zoom(node["zoom"].as<double>()),
		fov(node["fov"].as<double>()),
		farClip(node["farClip"].as<double>()),
		nearClip(node["nearClip"].as<double>())
	{}

	YAML::Node Camera::Export(YAML::Node node) const {
		node["orthographic"] = isOrthographic;
		node["fov"] = fov;
		node["zoom"] = zoom;
		node["farClip"] = farClip;
		node["nearClip"] = nearClip;
		return node;
	}

	glm::mat4x4 Camera::GetView() const {
		glm::mat4x4 transform = glm::mat4(1.0f);

		//Rotate everything else based on camera rotation
		Quaternion rotation = Quaternion::Conjugate(GetParent()->GetWorldRotation());
		#ifdef GLM_FORCE_QUAT_DATA_XYZW
		transform *= glm::mat4_cast(glm::quat(rotation.X, rotation.Y, rotation.Z, rotation.W));
		#else
		transform *= glm::mat4_cast(glm::quat(rotation.W, rotation.X, rotation.Y, rotation.Z));
		#endif

		//Move everything else based on camera position
		Vector3 position = GetParent()->GetWorldPosition();
		transform = glm::translate(transform, (glm::vec3)(-position));

		return transform;
	}

	glm::mat4x4 Camera::GetProjection() const {
		GameSettings s = engine->GetGameSettings();
		double aspect = (double)s.WIDTH / s.HEIGHT;
		if (isOrthographic)
		{
			Vector3 position = GetParent()->GetWorldPosition();
			double width = (1 * aspect) / (zoom / position.Z);
			double height = (1) / (zoom / position.Z);
			//Set up an orthographic projection with the same near clip plane
			return glm::ortho(-width, width, -height, height, nearClip, farClip);
		}
		else
		{
			//Set the perspective with the appropriate aspect ratio
			return glm::perspective(glm::radians(fov / aspect), aspect, nearClip, farClip);
		}
	}
}
#endif
