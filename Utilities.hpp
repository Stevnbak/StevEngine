#pragma once

namespace StevEngine {
	namespace Utilities {
		//Classes
		class Vector2d;
		class Vector3d;
		class Rotation3d;
		class Range3d;

		//Vector3d definition
		class Vector3d {
			public:
				double X = 0, Y = 0, Z = 0;
				Vector3d(double x, double y, double z);
				Vector3d();
				Vector3d Get();
				double Magnitude();
				void Normalize();
				double Distance(Vector3d target);
				Vector3d operator+(const Vector3d& other) const;
				Vector3d operator-(const Vector3d& other) const;
				Vector3d& operator+=(const Vector3d& other);
				Vector3d& operator-=(const Vector3d& other);
				Vector3d operator*(const double& other) const;
				Vector3d operator/(const double& other) const;
				Vector3d Mult(double value);
				Vector3d Divide(double value);
				Vector3d Cross(const Vector3d& other) const;
				Vector2d ConvertTo2D();
		};

		//Vector2d definition
		class Vector2d {
			public:
				double X = 0, Y = 0;
				Vector2d(double x, double y);
				Vector2d();
				Vector2d Get();
				double Magnitude();
				void Normalize();
				double Distance(Vector2d target);
				Vector2d operator+(const Vector2d& other) const;
				Vector2d operator-(const Vector2d& other) const;
				Vector2d& operator+=(const Vector2d& other);
				Vector2d& operator-=(const Vector2d& other);
				Vector2d operator*(const double& other) const;
				Vector2d operator/(const double& other) const;
				Vector2d Mult(double value);
				Vector2d Divide(double value);
				Vector3d ConvertTo3D();
		};

		//Rotation3d definition
		class Rotation3d {
			public:
				double yaw = 0;
				double pitch = 0;
				double roll = 0;
				Rotation3d();
				Rotation3d(double yaw, double pitch, double roll);
				void OpenGLRotate();
				Vector3d forward();
				Vector3d right();
				Vector3d up();
		};

		double DegreesToRadians(double degrees);

		//Range3d definition
		class Range3d {
			public:
				Vector3d Low;
				Vector3d High;
				Range3d();
				Range3d(double lowX, double highX, double lowY, double highY, double lowZ, double highZ);
				Range3d(Vector3d low, Vector3d high);
				Vector3d Center();
				bool IsIntersecting(Range3d other);
		};
	}
}