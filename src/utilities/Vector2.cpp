#include "Vector2.hpp"
#include "Vector3.hpp"
#include "utilities/Stream.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <format>

namespace StevEngine::Utilities {
	//Constructors
	Vector2::Vector2(double x, double y) : X(x), Y(y) {}
	Vector2::Vector2(const Vector2& from) : X(from.X), Y(from.Y)  {}
	Vector2::Vector2() : X(0), Y(0) {}
	//Functions
	Vector2 Vector2::Get() const {
		return *this;
	}
	double Vector2::Magnitude() const {
		return sqrt(pow(X, 2) + pow(Y, 2));
	}
	Vector2& Vector2::Normalize() {
		double mag = Magnitude();
		if (mag != 0) {
			X /= mag;
			Y /= mag;
		}
		return *this;
	}
	Vector2 Vector2::Normalized() const {
		return Get().Normalize();
	}
	//Operators
	Vector2 Vector2::operator + (const Vector2& other) const {
		return Vector2(X + other.X, Y + other.Y);
	}
	Vector2 Vector2::operator - (const Vector2& other) const {
		return Vector2(X - other.X, Y - other.Y);
	}
	Vector2& Vector2::operator += (const Vector2& other) {
		X += other.X;
		Y += other.Y;
		return *this;
	}
	Vector2& Vector2::operator -= (const Vector2& other) {
		X -= other.X;
		Y -= other.Y;
		return *this;
	}
	Vector2 Vector2::operator * (const double& other) const {
		return Vector2(X * other, Y * other);
	}
	Vector2 Vector2::operator / (const double& other) const {
		return Vector2(X / other, Y / other);
	}
	bool Vector2::operator == (const Vector2& other) const {
		return (X == other.X) && (Y == other.Y);
	}
	//Conversions
	Vector2::operator Vector3() const {
		return Vector3(X, Y, 0);
	}
	Vector2::operator std::string() const {
		return std::format("[{}, {}]", X, Y);
	}
	const float* Vector2::data() const {
		float* data = new float[2];
		data[0] = (float)X;
		data[1] = (float)Y;
		return data;
	}
	//Static stuff
	Vector2 Vector2::up = Vector2(0,1);
	Vector2 Vector2::right = Vector2(1,0);
	double Vector2::Distance(const Vector2& a, const Vector2& b) {
		return sqrt(pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2));
	}
	double Vector2::Dot(const Vector2& a, const Vector2& b) {
		return (a.X * b.X) + (a.Y * b.Y);
	}

	//Read from stream
	template <> Utilities::Vector2 Stream::Read<Utilities::Vector2>() {
		Utilities::Vector2 value;
		*this >> value.X >> value.Y;
		return value;
	}
	//Write to stream
	template <> void Stream::Write<Utilities::Vector2>(const Utilities::Vector2& data) {
		*this << data.X << data.Y;
	}
}
