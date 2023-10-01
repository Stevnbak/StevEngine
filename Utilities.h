#pragma once

namespace StevEngine {
	namespace Utilities {
		class Vector3d {
			public:
				double X = 0, Y = 0, Z = 0;
				Vector3d(double x, double y, double z);
				Vector3d();
				double Magnitude();
				double Distance(Vector3d target);
				Vector3d operator+(const Vector3d& other) const;
				Vector3d operator-(const Vector3d& other) const;
				Vector3d operator*(const Vector3d& other) const;
				Vector3d operator/(const Vector3d& other) const;
				Vector3d Mult(double value);
				Vector3d Divide(double value);
				Vector3d Cross(const Vector3d& other) const;
				void Normalize();
		};
	}
}