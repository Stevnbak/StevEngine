#include "Vector4.hpp"
#include "Vector3.hpp"
#include "utilities/Quaternion.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <format>

namespace StevEngine::Utilities {
	//Constructors
	Vector4::Vector4(double w, double x, double y, double z) : W(w), X(x), Y(y), Z(z) {}
	Vector4::Vector4(const Vector4& from) : W(from.W), X(from.X), Y(from.Y), Z(from.Z) {}
	Vector4::Vector4(double w, const Vector3& vec3) : W(w), X(vec3.X), Y(vec3.Y), Z(vec3.Z) {}
	Vector4::Vector4(double v) : W(v), X(v), Y(v), Z(v) {}
	Vector4::Vector4() : Vector4(0.0) {}
	//Functions
	Vector4 Vector4::Get() const {
		return *this;
	}
	double Vector4::Magnitude() const {
		return sqrt(pow(W, 2) + pow(X, 2) + pow(Y, 2) + pow(Z, 2));
	}
	Vector4& Vector4::Normalize() {
		double mag = Magnitude();
		if (mag != 0) {
			W /= mag;
			X /= mag;
			Y /= mag;
			Z /= mag;
		}
		return *this;
	}
	Vector4 Vector4::Normalized() const {
		return Get().Normalize();
	}
	double Vector4::Dot(const Vector4& a, const Vector4& b) {
		return (a.W * b.W) + (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
	}
	//Operators
	Vector4 Vector4::operator + (const Vector4& other) const {
		return Vector4(W + other.W, X + other.X, Y + other.Y, Z + other.Z);
	}
	Vector4 Vector4::operator - (const Vector4& other) const {
		return Vector4(W - other.W, X - other.X, Y - other.Y, Z - other.Z);
	}
	Vector4 Vector4::operator - () const {
		return Vector4(-W, -X, -Y, -Z);
	}
	Vector4& Vector4::operator += (const Vector4& other) {
		W += other.W;
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		return *this;
	}
	Vector4& Vector4::operator -= (const Vector4& other) {
		W -= other.W;
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		return *this;
	}
	Vector4 Vector4::operator * (const double& other) const {
		return Vector4(W / other, X * other, Y * other, Z * other);
	}
	Vector4 Vector4::operator / (const double& other) const {
		return Vector4(W / other, X / other, Y / other, Z / other);
	}
	bool Vector4::operator == (const Vector4& other) const {
		return (W == other.W) && (X == other.X) && (Y == other.Y) && (Z == other.Z);
	}
	//Conversions
	Vector4::operator std::string() const {
		return std::format("[{}, {}, {}, {}]", W, X, Y, Z);
	}
	Vector4::operator Quaternion() const {
		return Quaternion(W, X, Y, Z);
	}
	const float* Vector4::data() const {
		float* data = new float[4];
		data[0] = (float)W;
		data[1] = (float)X;
		data[2] = (float)Y;
		data[3] = (float)Z;
		return data;
	}
}

namespace YAML {
	Node convert<StevEngine::Utilities::Vector4>::encode(const StevEngine::Utilities::Vector4& rhs) {
		Node node;
		node.push_back(rhs.W);
		node.push_back(rhs.X);
		node.push_back(rhs.Y);
		node.push_back(rhs.Z);
		return node;
	}
	bool convert<StevEngine::Utilities::Vector4>::decode(const Node& node, StevEngine::Utilities::Vector4& rhs) {
		if(!node.IsSequence() || node.size() != 4) {
			return false;
		}
		rhs.W = node[0].as<double>();
		rhs.X = node[1].as<double>();
		rhs.Y = node[2].as<double>();
		rhs.Z = node[3].as<double>();
		return true;
	}
}