#include "Vector3.hpp"
#include "Vector2.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <format>

namespace StevEngine::Utilities {
	//Constructors
	Vector3::Vector3(double x, double y, double z) : X(x), Y(y), Z(z) {}
	Vector3::Vector3(const Vector3& from) : X(from.X), Y(from.Y), Z(from.Z) {}
	Vector3::Vector3(double v) : X(v), Y(v), Z(v) {}
	Vector3::Vector3() : Vector3(0.0) {}
	//Functions
	Vector3 Vector3::Get() const {
		return *this;
	}
	double Vector3::Magnitude() const {
		return sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
	}
	Vector3& Vector3::Normalize() {
		double mag = Magnitude();
		if (mag != 0) {
			X /= mag;
			Y /= mag;
			Z /= mag;
		}
		return *this;
	}
	Vector3 Vector3::Normalized() const {
		return Get().Normalize();
	}
	//Operators
	Vector3 Vector3::operator + (const Vector3& other) const {
		return Vector3(X + other.X, Y + other.Y, Z + other.Z);
	}
	Vector3 Vector3::operator - (const Vector3& other) const {
		return Vector3(X - other.X, Y - other.Y, Z - other.Z);
	}
	Vector3 Vector3::operator - () const {
		return Vector3(-X, -Y, -Z);
	}
	Vector3& Vector3::operator += (const Vector3& other) {
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		return *this;
	}
	Vector3& Vector3::operator -= (const Vector3& other) {
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		return *this;
	}
	Vector3 Vector3::operator * (const double& other) const {
		return Vector3(X * other, Y * other, Z * other);
	}
	Vector3 Vector3::operator / (const double& other) const {
		return Vector3(X / other, Y / other, Z / other);
	}
	bool Vector3::operator == (const Vector3& other) const {
		return (X == other.X) && (Y == other.Y) && (Z == other.Z);
	}
	//Conversions
	Vector3::operator Vector2() const {
		return Vector2(X, Y);
	}
	Vector3::operator std::string() const {
		return std::format("[{}, {}, {}]", X, Y, Z);
	}
	const float* Vector3::data() const {
		float* data = new float[3];
		data[0] = (float)X;
		data[1] = (float)Y;
		data[2] = (float)Z;
		return data;
	}
	#ifdef StevEngine_PHYSICS
	Vector3::operator JPH::DVec3() const {
		return JPH::DVec3(X, Y, Z);
	}
	Vector3& Vector3::operator= (const JPH::DVec3& other) {
		X = other.GetX();
		Y = other.GetY();
		Z = other.GetZ();
		return *this;
	}
	Vector3::Vector3(const JPH::DVec3& other) {
		X = other.GetX();
		Y = other.GetY();
		Z = other.GetZ();
	}
	Vector3::operator JPH::Vec3() const {
		return JPH::Vec3(X, Y, Z);
	}
	Vector3& Vector3::operator= (const JPH::Vec3& other) {
		X = other.GetX();
		Y = other.GetY();
		Z = other.GetZ();
		return *this;
	}
	Vector3::Vector3(const JPH::Vec3& other) {
		X = other.GetX();
		Y = other.GetY();
		Z = other.GetZ();
	}
	#endif
	#ifdef StevEngine_MODELS
	Vector3::operator aiVector3D() const {
		return aiVector3D(X, Y, Z);
	}
	Vector3& Vector3::operator= (const aiVector3D& other) {
		X = other.x;
		Y = other.y;
		Z = other.z;
		return *this;
	}
	Vector3::Vector3(const aiVector3D& other) {
		X = other.x;
		Y = other.y;
		Z = other.z;
	}
	#endif
	//Static stuff
	const Vector3 Vector3::up = Vector3(0,1,0);
	const Vector3 Vector3::right = Vector3(1,0,0);
	const Vector3 Vector3::forward = Vector3(0,0,-1);
	const Vector3 Vector3::identity = Vector3(1.0);
	const Vector3 Vector3::zero = Vector3(0.0);
	double Vector3::Distance(const Vector3& a, const Vector3& b) {
		return sqrt(pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2) + pow(a.Z - b.Z, 2));
	}
	Vector3 Vector3::Cross(const Vector3& a, const Vector3& b) {
		return Vector3(
			a.Y * b.Z - a.Z * b.Y,
			-(a.X * b.Z - a.Z * b.X),
			a.X * b.Y - a.Y * b.X
		);
	}
	double Vector3::Dot(const Vector3& a, const Vector3& b) {
		return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
	}
	Vector3 Vector3::CombineScale(const Vector3& a, const Vector3& b) {
		Utilities::Vector3 s = a.Get();
		s.X *= b.X;
		s.Y *= b.Y;
		s.Z *= b.Z;
		return s;
	}
	Vector3 Vector3::Slerp(const Vector3& a, const Vector3& b, double t) {
		double dot = Vector3::Dot(a, b);
		double theta = acos(dot / (a.Magnitude() * b.Magnitude()));
		double sintheta = sin(theta);

		return (a * (sin((1-t)*theta) / sintheta)) + (b * (sin(t*theta) / sintheta));
	}
}

namespace YAML {
	Node convert<StevEngine::Utilities::Vector3>::encode(const StevEngine::Utilities::Vector3& rhs) {
		Node node;
		node.push_back(rhs.X);
		node.push_back(rhs.Y);
		node.push_back(rhs.Z);
		return node;
	}
	bool convert<StevEngine::Utilities::Vector3>::decode(const Node& node, StevEngine::Utilities::Vector3& rhs) {
		if(!node.IsSequence() || node.size() != 3) {
			return false;
		}
		rhs.X = node[0].as<double>();
		rhs.Y = node[1].as<double>();
		rhs.Z = node[2].as<double>();
		return true;
	}
}
