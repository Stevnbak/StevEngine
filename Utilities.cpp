#include "Utilities.hpp"
#include <math.h>
namespace StevEngine {
	namespace Utilities {
		//Vector3d
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
		Vector3d Vector3d::operator+(const Vector3d & other) const {
			return Vector3d(X + other.X, Y + other.Y, Z + other.Z);
		}
		Vector3d Vector3d::operator-(const Vector3d & other) const {
			return Vector3d(X - other.X, Y - other.Y, Z - other.Z);
		}
		Vector3d& Vector3d::operator+=(const Vector3d& other) {
			this->X += other.X;
			this->Y += other.Y;
			this->Z += other.Z;
			return *this;
		}
		Vector3d& Vector3d::operator-=(const Vector3d& other) {
			this->X -= other.X;
			this->Y -= other.Y;
			this->Z -= other.Z;
			return *this;
		}
		/*Vector3d Vector3d::operator*(const Vector3d& other) const {
			return Vector3d(X * other.X, Y * other.Y, Z * other.Z);
		}
		Vector3d Vector3d::operator/(const Vector3d & other) const {
			return Vector3d(X / other.X, Y / other.Y, Z / other.Z);
		}*/
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
		Vector3d Vector3d::Cross(const Vector3d & other) const {
			return Vector3d(Y * other.Z - Z * other.Y, -(X * other.Z - Z * other.X), X * other.Y - Y * other.X);
		}
		Vector3d Vector3d::Get() {
			return Vector3d(X, Y, Z);
		}
		Vector2d Vector3d::To2D() {
			return Vector2d(X, Y);
		}
		//Vector2d
		Vector2d::Vector2d(double x, double y) {
			X = x;
			Y = y;
		}
		Vector2d::Vector2d() {
			X = 0;
			Y = 0;
		}
		double Vector2d::Magnitude() {
			return sqrt(pow(X, 2) + pow(Y, 2));
		}
		double Vector2d::Distance(Vector2d target) {
			return sqrt(pow(X - target.X, 2) + pow(Y - target.Y, 2));
		}
		Vector2d Vector2d::operator+(const Vector2d& other) const {
			return Vector2d(X + other.X, Y + other.Y);
		}
		Vector2d Vector2d::operator-(const Vector2d& other) const {
			return Vector2d(X - other.X, Y - other.Y);
		}
		/*Vector2d Vector2d::operator*(const Vector2d& other) const {
			return Vector2d(X * other.X, Y * other.Y);
		}
		Vector2d Vector2d::operator/(const Vector2d& other) const {
			return Vector2d(X / other.X, Y / other.Y);
		}*/
		Vector2d Vector2d::Mult(double value) {
			X *= value;
			Y *= value;
			return Vector2d(X, Y);
		}
		Vector2d Vector2d::Divide(double value) {
			X /= value;
			Y /= value;
			return Vector2d(X, Y);
		}
		void Vector2d::Normalize() {
			double mag = Magnitude();
			Divide(mag);
		}
		Vector3d Vector2d::To3D() {
			return Vector3d(X, Y, 0);
		}
		//Range3d
		Range3d::Range3d() {
			Low = Vector3d(0, 0, 0);
			High = Vector3d(0, 0, 0);
		}
		Range3d::Range3d(double lowX, double highX, double lowY, double highY, double lowZ, double highZ) {
			Low = Vector3d(lowX, lowY, lowZ);
			High = Vector3d(highX, highY, highZ);
		}
		Range3d::Range3d(Vector3d low, Vector3d high) {
			Low = low;
			High = high;
		}
		Vector3d Range3d::Center() {
			return Vector3d(Low.X + ((High.X - Low.X) / 2), Low.Y + ((High.Y - Low.Y) / 2), Low.Z + ((High.Z - Low.Z) / 2));
		}
		bool Range3d::IsIntersecting(Range3d other) {
			if ((High.X >= other.Low.X && High.X <= other.High.X) || (Low.X >= other.Low.X && Low.X <= other.High.X)) {
				if ((High.Y >= other.Low.Y && High.Y <= other.High.Y) || (Low.Y >= other.Low.Y && Low.Y <= other.High.Y)) {
					if ((High.Z >= other.Low.Z && High.Z <= other.High.Z) || (Low.Z >= other.Low.Z && Low.Z <= other.High.Z)) {
						return true;
					}
				}
			}
			return false;
		}
	}
}