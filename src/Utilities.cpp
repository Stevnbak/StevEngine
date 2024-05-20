#include "Utilities.hpp"
#include "Log.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <SDL2/SDL_opengl.h>
namespace StevEngine {
	namespace Utilities {
		//Vector3d
#pragma region Vector3d
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
		Vector3d Vector3d::Get() {
			return Vector3d(X, Y, Z);
		}
		double Vector3d::Magnitude() {
			return sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
		}
		void Vector3d::Normalize() {
			double mag = Magnitude();
			if (mag != 0) Divide(mag);
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
		Vector3d Vector3d::operator*(const double& other) const {
			return Vector3d(X * other, Y * other, Z * other);
		}
		Vector3d Vector3d::operator/(const double& other) const {
			return Vector3d(X / other, Y / other, Z / other);
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
		Vector3d Vector3d::Cross(const Vector3d & other) const {
			return Vector3d(Y * other.Z - Z * other.Y, -(X * other.Z - Z * other.X), X * other.Y - Y * other.X);
		}
		Vector2d Vector3d::ConvertTo2D() {
			return Vector2d(X, Y);
		}
#pragma endregion
		//Vector2d
#pragma region Vector2d
		Vector2d::Vector2d(double x, double y) {
			X = x;
			Y = y;
		}
		Vector2d::Vector2d() {
			X = 0;
			Y = 0;
		}
		Vector2d Vector2d::Get() {
			return Vector2d(X, Y);
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
		Vector2d& Vector2d::operator+=(const Vector2d& other) {
			this->X += other.X;
			this->Y += other.Y;
			return *this;
		}
		Vector2d& Vector2d::operator-=(const Vector2d& other) {
			this->X -= other.X;
			this->Y -= other.Y;
			return *this;
		}
		Vector2d Vector2d::operator*(const double& other) const {
			return Vector2d(X * other, Y * other);
		}
		Vector2d Vector2d::operator/(const double& other) const {
			return Vector2d(X / other, Y / other);
		}
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
			if (mag != 0) Divide(mag);
		} 
		Vector3d Vector2d::ConvertTo3D() {
			return Vector3d(X, Y, 0);
		}
#pragma endregion
		//Rotation3d
#pragma region Rotation3d
		Rotation3d::Rotation3d() {
			yaw = 0;
			pitch = 0;
			roll = 0;
		}
		Rotation3d::Rotation3d(double pitchDegrees, double yawDegrees, double rollDegrees) {
			yaw = yawDegrees;
			pitch = pitchDegrees;
			roll = rollDegrees;
		}
		void Rotation3d::OpenGLRotate() {
			glRotated(yaw, 0, 1, 0);
			glRotated(pitch, 1, 0, 0);
			glRotated(roll, 0, 0, 1);
		}
		Vector3d Rotation3d::forward() {
			Vector3d direction (
				sin(DegreesToRadians(yaw)),
				-sin(DegreesToRadians(pitch)),
				cos(DegreesToRadians(pitch)) * cos(DegreesToRadians(yaw))
			);
			return direction;
		}
		Vector3d Rotation3d::right() {
			Vector3d direction(
				cos(DegreesToRadians(roll)) * cos(DegreesToRadians(yaw)),
				sin(DegreesToRadians(roll)),
				-sin(DegreesToRadians(yaw))
			);
			return direction;
		}
		Vector3d Rotation3d::up() {
			Vector3d direction(
				-sin(DegreesToRadians(roll)),
				cos(DegreesToRadians(roll)) * cos(DegreesToRadians(pitch)),
				sin(DegreesToRadians(pitch))
			);
			return direction;
		}
		double DegreesToRadians(double degrees) {
			return degrees * (M_PI / 180);
		}
#pragma endregion
		//Range3d
#pragma region Range3d
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
#pragma endregion
	}
}