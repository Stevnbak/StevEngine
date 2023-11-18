#pragma once

namespace StevEngine {
	namespace Utilities {
		class Vector2d;
		class Vector3d;
		class Vector3d {
			public:
				double X = 0, Y = 0, Z = 0;
				Vector3d(double x, double y, double z);
				Vector3d(void);
				double Magnitude();
				double Distance(Vector3d target);
				Vector3d operator+(const Vector3d& other) const;
				Vector3d operator-(const Vector3d& other) const;
				Vector3d& operator+=(const Vector3d& other);
				Vector3d& operator-=(const Vector3d& other);
				Vector3d operator*(const double& other) const;
				Vector3d operator/(const double& other) const;
				/*Vector3d operator*(const Vector3d& other) const;
				Vector3d operator/(const Vector3d& other) const;*/
				Vector3d Mult(double value);
				Vector3d Divide(double value);
				Vector3d Cross(const Vector3d& other) const;
				void Normalize();
				Vector3d Get();
				Vector2d To2D();
		};

		class Vector2d {
			public:
				double X = 0, Y = 0;
				Vector2d(double x, double y);
				Vector2d();
				double Magnitude();
				double Distance(Vector2d target);
				Vector2d operator+(const Vector2d& other) const;
				Vector2d operator-(const Vector2d& other) const;
				/*Vector2d operator*(const Vector2d& other) const;
				Vector2d operator/(const Vector2d& other) const;*/
				Vector2d Mult(double value);
				Vector2d Divide(double value);
				void Normalize();
				Vector3d To3D();
		};

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