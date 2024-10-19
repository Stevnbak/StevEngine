#include "Vector2.hpp"
#include "Vector3.hpp"

#include "main/Log.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <format>
#include <sstream>

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
		return sqrt(exp2(X) + exp2(Y));
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
		return sqrt(exp2(a.X - b.X) + exp2(a.Y - b.Y));
	}
	double Vector2::Dot(const Vector2& a, const Vector2& b) {
		return (a.X * b.X) + (a.Y * b.Y);
	}
}

namespace YAML {
	Node convert<StevEngine::Utilities::Vector2>::encode(const StevEngine::Utilities::Vector2& rhs) {
		Node node;
		node.push_back(rhs.X);
		node.push_back(rhs.Y);
		return node;
	}
	bool convert<StevEngine::Utilities::Vector2>::decode(const Node& node, StevEngine::Utilities::Vector2& rhs) {
		if(!node.IsSequence() || node.size() != 2) {
			return false;
		}
		rhs.X = node[0].as<double>();
		rhs.Y = node[1].as<double>();
		return true;
	}
}
