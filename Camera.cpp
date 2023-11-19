#include "Camera.hpp"
#include "Engine.hpp"
#include "Utilities.hpp"
#include <GLEW/include/GL/glew.h> 
#include <SDL2/include/SDL_opengl.h>
#include "Log.hpp"
#include <math.h>
using namespace StevEngine;
using namespace StevEngine::Utilities;


void Camera::SetOptions(bool orthographic, double zoomValue, double aspectRatio) {
	isOrthographic = orthographic;
	zoom = zoomValue;
	aspect = aspectRatio;
}

void Camera::UpdateView() {
	//Log::Normal("Updating camera view", true);
	//Rotate everything else based on camera rotation
	glRotated(gameObject->rotation.roll, 0, 0, -1);
	glRotated(gameObject->rotation.yaw, 0, -1, 0);
	glRotated(gameObject->rotation.pitch, -1, 0, 0);
	//Move everything else based on camera position
	glTranslated(-gameObject->position.X, -gameObject->position.Y, -gameObject->position.Z);
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
