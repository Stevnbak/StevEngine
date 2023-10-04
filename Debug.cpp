#include "Debug.hpp"
#include "Engine.hpp"
#include <SDL_opengl.h>
#include <iostream>

void DrawCube() {
	// White side - BACK
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glEnd();

	// Purple side - RIGHT
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glEnd();

	// Green side - LEFT
	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glEnd();

	// Blue side - TOP
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glEnd();

	// Red side - BOTTOM
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glEnd();
	glPopMatrix();
}

void DrawBox(double x, double y, double z, double w, double h) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glBegin(GL_QUADS);
	glVertex3f(-w / 2, h / 2, 0); // top left
	glVertex3f(w / 2, h / 2, 0); // top right 
	glVertex3f(w / 2, -h / 2, 0); // bottom right
	glVertex3f(-w / 2, -h / 2, 0); // bottom left
	/*
	glVertex2f(0, (h / HEIGHT)); // top left
	glVertex2f((w / WIDTH), (h / HEIGHT)); // top right 
	glVertex2f((w / WIDTH), 0); // bottom right
	glVertex2f(0, 0); // bottom left
	*/
	glEnd();
	glPopMatrix();
}