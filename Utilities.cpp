#include "Utilities.h"
#include <math.h>
namespace StevEngine {
	namespace Utilities {
		Vector3d::Vector3d(double x, double y, double z) {
			X = x;
			Y = y;
			Z = z;
		}
		Vector3d::Vector3d() {
			X = 0;
			Y = 0;
			Z = 0;
		}
		double Vector3d::Magnitude() {
			return sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
		}
		double Vector3d::Distance(Vector3d target) {
			return sqrt(pow(X - target.X, 2) + pow(Y - target.Y, 2) + pow(Z - target.Z, 2));
		}
		Vector3d Vector3d::operator+(const Vector3d& other) const {
			return Vector3d(X + other.X, Y + other.Y, Z + other.Z);
		}
		Vector3d Vector3d::operator-(const Vector3d& other) const {
			return Vector3d(X - other.X, Y - other.Y, Z - other.Z);
		}
		Vector3d Vector3d::operator*(const Vector3d& other) const {
			return Vector3d(X * other.X, Y * other.Y, Z * other.Z);
		}
		Vector3d Vector3d::operator/(const Vector3d& other) const {
			return Vector3d(X / other.X, Y / other.Y, Z / other.Z);
		}
		Vector3d Vector3d::Mult(double value) {
			X *= value;
			Y *= value;
			Z *= value;
			return Vector3d(X, Y, Z);
		}
		Vector3d Vector3d::Divide(double value) {
			X /= value;
			Y /= value;
			Z /= value;
			return Vector3d(X, Y, Z);
		}
		void Vector3d::Normalize() {
			double mag = Magnitude();
			Divide(mag);
		}
		Vector3d Vector3d::Cross(const Vector3d& other) const {
			return Vector3d(Y * other.Z - Z * other.Y, - (X * other.Z - Z * other.X), X * other.Y - Y * other.X);
		}
	}
}