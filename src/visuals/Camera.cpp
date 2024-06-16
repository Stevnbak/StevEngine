#include "Camera.hpp"
#include <core/Engine.hpp>
#include <core/Utilities.hpp>
#include <core/Log.hpp>

#include <GL/glew.h>
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
	glTranslated(-gameObject->position.X, -gameObject->position.Y, -gameObject->position.Z);
	//Rotate everything else based on camera rotation
	glRotated(-gameObject->rotation.yaw,  0, 1, 0);
	
	Vector3d right = gameObject->rotation.right();
	glRotated(-gameObject->rotation.pitch, right.X, right.Y, right.Z);
	glRotated(-gameObject->rotation.roll, 0, 0, 1);
	//Set projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (isOrthographic)
	{
		double width = (1 / aspect) / (zoom / gameObject->position.Z);
		double height = (1 * aspect) / (zoom / gameObject->position.Z);
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
