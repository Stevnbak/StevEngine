#include "Camera.hpp"
#include <main/Engine.hpp>
#include <utilities/Vector3.hpp>
#include <utilities/Quaternion.hpp>
#include <main/Log.hpp>
#include <scenes/GameObject.hpp>

#include <SDL2/SDL_opengl.h>
#include <math.h>
using namespace StevEngine;
using namespace StevEngine::Utilities;


Camera::Camera(bool orthographic, double zoomValue, double aspectRatio) : Component("Camera") {
	isOrthographic = orthographic;
	zoom = zoomValue;
	aspect = aspectRatio;
}

void Camera::Export(tinyxml2::XMLElement* element) {
	element->SetAttribute("orthographic", isOrthographic);
	element->SetAttribute("zoom", zoom);
	element->SetAttribute("aspect", aspect);
}
Camera::Camera(tinyxml2::XMLElement* node) : Camera(node->BoolAttribute("orthographic"), node->DoubleAttribute("zoom"), node->DoubleAttribute("aspect")) {}
FactoryBase* factory = GameObject::AddComponentFactory<Camera>(std::string("Camera"));


void Camera::UpdateView() {
	//Move everything else based on camera position
	Vector3 position = GetParent()->GetWorldPosition();
	glTranslated(-position.X, -position.Y, -position.Z);
	//Rotate everything else based on camera rotation
	Quaternion rot = Quaternion::Conjugate(GetParent()->GetWorldRotation());
	std::tuple<double, Vector3> angleAxis = rot.GetAngleAxis();
	double angle = Quaternion::RadiansToDegrees(std::get<0>(angleAxis));
	Vector3 v = std::get<1>(angleAxis);
	glRotated(angle, v.X, v.Y, v.Z);
	//Set projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (isOrthographic)
	{
		double width = (1 / aspect) / (zoom / position.Z);
		double height = (1 * aspect) / (zoom / position.Z);
		//Set up an orthographic projection with the same near clip plane
		glOrtho(-width, width, -height, height, nearClip, farClip);
	}
	else
	{
		double width = (1 / aspect) / zoom;
		double height = (1 * aspect) / zoom;
		//Set the perspective with the appropriate aspect ratio
		glFrustum(-width, width, -height, height, nearClip, farClip);
	}
	glMatrixMode(GL_MODELVIEW);
}
