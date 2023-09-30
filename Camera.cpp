#include "Camera.h"
#include "Engine.h"
#include <vector>
#include <SDL_opengl.h>
#include <iostream>
using namespace StevEngine;

Camera::Camera(std::vector<double> pos, std::vector<double> rot, bool orthographic, double zoomValue, double aspectRatio) {
	position = pos;
	rotation = rot;
	isOrthographic = orthographic;
	zoom = zoomValue;
	aspect = aspectRatio;
}

void Camera::UpdateView() {
	std::cout << "Updating camera" << std::endl;

	glTranslated(-position[0], position[1], position[2]);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (isOrthographic)
	{
		double width = (1 / aspect) / (zoom / position[2]);
		double height = (1 * aspect) / (zoom / position[2]);
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
