#include "BasicComponents.hpp"
#include <core/Log.hpp>
#include <core/Utilities.hpp>


#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <math.h>

namespace StevEngine {
	//Primitive functions

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

	void DrawCylinder() {
		int detail = 50;
		std::vector<Utilities::Vector2d> circle;
		float anglePerStep = (2 * M_PI) / detail; //In Radians
		for (int i = 0; i <= detail; i++)
		{
			float currentAngle = (anglePerStep)*i;
			circle.push_back(Utilities::Vector2d(0.5 * std::cos(currentAngle), 0.5 * std::sin(currentAngle)));
		}
		//Draw bottom
		glBegin(GL_POLYGON);
		for (Utilities::Vector2d point : circle) {
			glVertex3f(point.X, point.Y, -0.5);
		}
		glEnd();
		//Draw top
		glBegin(GL_POLYGON);
		for (Utilities::Vector2d point : circle) {
			glVertex3f(point.X, point.Y, 0.5);
		}
		glEnd();
		//Draw sides
		for (int i = 0; i < circle.size() - 1; i++) {
			Utilities::Vector2d point1 = circle[i];
			Utilities::Vector2d point2 = circle[i + 1];
			glBegin(GL_POLYGON);
			glVertex3f(point1.X, point1.Y, -0.5);
			glVertex3f(point1.X, point1.Y, 0.5);
			glVertex3f(point2.X, point2.Y, 0.5);
			glVertex3f(point2.X, point2.Y, -0.5);
			glEnd();
		}
	}

	void DrawSphere() {
		int detail = 50;
		float anglePerStep = (2 * M_PI) / detail; //In Radians
		//Calculate circle layers
		std::vector<std::vector<Utilities::Vector3d>> layers;
		for (int stack = -detail / 2; stack < detail / 2; stack++) {
			float stackAngle = anglePerStep * stack;
			float currentHeight = std::sin(stackAngle) * 0.5;
			float cosStackAngle = std::cos(stackAngle);
			std::vector<Utilities::Vector3d> circle;
			for (int section = 0; section <= detail; section++)
			{
				float sectionAngle = (anglePerStep)*section;
				circle.push_back(Utilities::Vector3d(0.5 * std::cos(sectionAngle) * cosStackAngle, 0.5 * std::sin(sectionAngle) * cosStackAngle, currentHeight));
			}
			layers.push_back(circle);
		}
		//Draw polygons
		for (int i = 0; i < layers.size() - 1; i++) {
			std::vector<Utilities::Vector3d> circle1 = layers[i];
			std::vector<Utilities::Vector3d> circle2 = layers[i + 1];
			for (int j = 0; j < circle1.size() - 1; j++) {
				float currentAngle = (anglePerStep)*i;
				glBegin(GL_POLYGON);
				glVertex3d(circle1[j].X, circle1[j].Y, circle1[j].Z);
				glVertex3d(circle1[j + 1].X, circle1[j + 1].Y, circle1[j + 1].Z);
				glVertex3d(circle2[j + 1].X, circle2[j + 1].Y, circle2[j + 1].Z);
				glVertex3d(circle2[j].X, circle2[j].Y, circle2[j].Z);
				glEnd();
			}
		}
	}

	void DrawCapsule() {
		int detail = 50;
		float anglePerStep = (2 * M_PI) / detail; //In Radians
		//Calculate sphere layers
		std::vector<std::vector<Utilities::Vector3d>> topLayers;
		std::vector<std::vector<Utilities::Vector3d>> bottomLayers;
		for (int stack = -detail / 2; stack <= detail / 2; stack++) {
			float stackAngle = anglePerStep * stack;
			float currentHeight = std::sin(stackAngle) * 0.25 + (stack <= 0 ? -0.25 : 0.25);
			float cosStackAngle = std::cos(stackAngle);
			std::vector<Utilities::Vector3d> circle;
			for (int section = 0; section <= detail; section++)
			{
				float sectionAngle = (anglePerStep)*section;
				circle.push_back(Utilities::Vector3d(0.5 * std::cos(sectionAngle) * cosStackAngle, 0.5 * std::sin(sectionAngle) * cosStackAngle, currentHeight));
			}
			if(stack <= 0) bottomLayers.push_back(circle);
			if (stack > 0) topLayers.push_back(circle);
		}
		//Draw bottom sphere
		for (int i = 0; i < bottomLayers.size() - 1; i++) {
			std::vector<Utilities::Vector3d> circle1 = bottomLayers[i];
			std::vector<Utilities::Vector3d> circle2 = bottomLayers[i + 1];
			for (int j = 0; j < circle1.size() - 1; j++) {
				float currentAngle = (anglePerStep)*i;
				glBegin(GL_POLYGON);
				glVertex3d(circle1[j].X, circle1[j].Y, circle1[j].Z);
				glVertex3d(circle1[j + 1].X, circle1[j + 1].Y, circle1[j + 1].Z);
				glVertex3d(circle2[j + 1].X, circle2[j + 1].Y, circle2[j + 1].Z);
				glVertex3d(circle2[j].X, circle2[j].Y, circle2[j].Z);
				glEnd();
			}
		}
		//Draw top sphere
		for (int i = 0; i < topLayers.size() - 1; i++) {
			std::vector<Utilities::Vector3d> circle1 = topLayers[i];
			std::vector<Utilities::Vector3d> circle2 = topLayers[i + 1];
			for (int j = 0; j < circle1.size() - 1; j++) {
				float currentAngle = (anglePerStep)*i;
				glBegin(GL_POLYGON);
				glVertex3d(circle1[j].X, circle1[j].Y, circle1[j].Z);
				glVertex3d(circle1[j + 1].X, circle1[j + 1].Y, circle1[j + 1].Z);
				glVertex3d(circle2[j + 1].X, circle2[j + 1].Y, circle2[j + 1].Z);
				glVertex3d(circle2[j].X, circle2[j].Y, circle2[j].Z);
				glEnd();
			}
		}
		//Draw cylinder sides
		std::vector<Utilities::Vector2d> cylinderCircle;
		for (int i = 0; i <= detail; i++)
		{
			float currentAngle = (anglePerStep)*i;
			cylinderCircle.push_back(Utilities::Vector2d(0.5 * std::cos(currentAngle), 0.5 * std::sin(currentAngle)));
		}
		for (int i = 0; i < cylinderCircle.size() - 1; i++) {
			Utilities::Vector2d point1 = cylinderCircle[i];
			Utilities::Vector2d point2 = cylinderCircle[i + 1];
			glBegin(GL_POLYGON);
			glVertex3f(point1.X, point1.Y, -0.25);
			glVertex3f(point1.X, point1.Y, 0.25);
			glVertex3f(point2.X, point2.Y, 0.25);
			glVertex3f(point2.X, point2.Y, -0.25);
			glEnd();
		}
	}

	//Constructor
	Primitive::Primitive(PrimitiveType type) : Component("Primitive") {
		this->type = type;
	}

	//Main draw function
	void Primitive::Draw() {
		//Colour
		glColor4d(colour.r, colour.g, colour.b, colour.a);
		//Push Matrix
		glPushMatrix();
		//Rotation
		rotation.OpenGLRotate();
		//Position
		glTranslated(position.X, position.Y, position.Z);
		//Scale
		glScaled(scale.X, scale.Y, scale.Z);
		//Draw primitive
		switch (type) {
		case PrimitiveType::Cube:
			DrawCube();
			break;
		case PrimitiveType::Sphere:
			DrawSphere();
			break;
		case PrimitiveType::Cylinder:
			DrawCylinder();
			break;
		case PrimitiveType::Capsule:
			DrawCapsule();
			break;
		default:
			break;
		}

		//Pop matrix
		glPopMatrix();
	}

	void Primitive::Export(tinyxml2::XMLElement* element) {
        element->SetAttribute("type", (int)type);
        element->SetAttribute("position", std::format("[{},{},{}]", position.X, position.Y, position.Z).c_str());
		element->SetAttribute("rotation", std::format("[{},{},{}]", rotation.pitch, rotation.yaw, rotation.roll).c_str());
		element->SetAttribute("scale", std::format("[{},{},{}]", scale.X, scale.Y, scale.Z).c_str());
        element->SetAttribute("color", std::format("[{},{},{},{}]", colour.r, colour.g, colour.b, colour.a).c_str());
    }
    Primitive::Primitive(tinyxml2::XMLElement* node) : Primitive((PrimitiveType)node->IntAttribute("type")) {

	}
}