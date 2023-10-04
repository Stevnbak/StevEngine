#include "Camera.hpp"
#include "Engine.hpp"
#include "Utilities.hpp"
#include <GL/glew.h> 
#include <SDL_opengl.h>
#include "Log.hpp"
#include <math.h>
using namespace StevEngine;
using namespace StevEngine::Utilities;


Camera::Camera(Vector3d pos, Vector3d rot, bool orthographic, double zoomValue, double aspectRatio) {
	position = pos;
	rotation = rot;
	isOrthographic = orthographic;
	zoom = zoomValue;
	aspect = aspectRatio;
}

void Camera::UpdateView() {
	//Log::Normal("Updating camera view");
	//Rotate everything else based on camera rotation
	glRotated(-rotation.X, 1, 0, 0);
	glRotated(-rotation.Y, 0, 1, 0);
	glRotated(-rotation.Z, 0, 0, 1);
	//Move everything else based on camera position
	glTranslated(-position.X, -position.Y, -position.Z);
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
