#ifdef StevEngine_SHOW_WINDOW
#include "Camera.hpp"
#include "main/Engine.hpp"
#include "main/Component.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Matrix4.hpp"
#include "main/GameObject.hpp"

#include <math.h>
using namespace StevEngine;
using namespace StevEngine::Utilities;

namespace StevEngine::Visuals {
	Camera::Camera(bool orthographic, float fov, float zoomValue, float nearClip, float farClip)
	  : isOrthographic(orthographic), fov(fov), zoom(zoomValue), nearClip(nearClip), farClip(farClip) {}

	Camera::Camera(Utilities::Stream& stream)
	  : isOrthographic(stream.Read<bool>()),
		zoom(stream.Read<float>()),
		fov(stream.Read<float>()),
		farClip(stream.Read<float>()),
		nearClip(stream.Read<float>())
	{}

	Utilities::Stream Camera::Export(Utilities::StreamType type) const {
		Utilities::Stream stream(type);
		stream << isOrthographic << fov << zoom << farClip << nearClip;
		return stream;
	}

	Matrix4 Camera::GetView() const {
		//Rotate everything else based on camera rotation
		Matrix4 rotation = Matrix4::FromRotation(Quaternion::Conjugate(GetParent().GetWorldRotation()));

		//Move everything else based on camera position
		Matrix4 translation = Matrix4::FromTranslation(-GetParent().GetWorldPosition());

		return rotation * translation;
	}

	Matrix4 Camera::GetProjection() const {
		GameSettings s = engine->GetGameSettings();
		float aspect = (float)s.WIDTH / s.HEIGHT;
		if (isOrthographic)
		{
			Vector3 position = GetParent().GetWorldPosition();
			double width = (1 * aspect) / (zoom / position.Z);
			double height = (1) / (zoom / position.Z);
			//Set up an orthographic projection
			return Matrix4::FromOrthographic(width, height, nearClip, farClip);
		}
		else
		{
			//Set the perspective projection
			return Matrix4::FromPerspective(fov / zoom, aspect, nearClip * zoom, farClip * zoom);
		}
	}
}
#endif
