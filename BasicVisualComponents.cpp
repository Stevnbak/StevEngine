#include <SDL2/include/SDL.h>
#include <SDL2/include/SDL_opengl.h>
#include "BasicVisualComponents.hpp"
#include "Log.hpp"
using namespace StevEngine;

//Primitive component
void DrawCube();

void Primitive::Draw() {
	//Colour
	glColor4d(colour.r, colour.g, colour.b, colour.a);
	//Push Matrix
	glPushMatrix();
	//Rotation
	glRotated(rotation.X, 1, 0, 0);
	glRotated(rotation.Y, 0, 1, 0);
	glRotated(rotation.Z, 0, 0, 1);
	//Position
	glTranslated(position.X, position.Y, position.Z);
	//Scale
	glScaled(scale.X, scale.Y, scale.Z);
	//Draw primitive
	switch (type) {
		case PrimitiveType::Cube:
			DrawCube();
			break;
		default:
			break;
	}

	//Pop matrix
	glPopMatrix();
	
}

void DrawCube() {
	// BACK
	glBegin(GL_POLYGON);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glEnd();
	// FRONT
	glBegin(GL_POLYGON);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glEnd();
	// RIGHT
	glBegin(GL_POLYGON);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glEnd();
	// LEFT
	glBegin(GL_POLYGON);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glEnd();
	// TOP
	glBegin(GL_POLYGON);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glEnd();
	// BOTTOM
	glBegin(GL_POLYGON);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glEnd();
}