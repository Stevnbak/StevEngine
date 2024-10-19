#include "Quaternion.hpp"

#include "main/Log.hpp"
#include "utilities/Vector4.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>

namespace StevEngine::Utilities {
	//Constructors
	Quaternion::Quaternion(double w, double x, double y, double z)
		: W(w), X(x), Y(y), Z(z) {}
	Quaternion::Quaternion(double w, Vector3 vector)
		: W(w), X(vector.X), Y(vector.Y), Z(vector.Z) {}
	Quaternion::Quaternion(const Quaternion& from)
		: W(from.W), X(from.X), Y(from.Y), Z(from.Z) {}
	Quaternion::Quaternion()
		: W(1), X(0), Y(0), Z(0) {}
	//Directions
	Vector3 Quaternion::Forward() const {
		return Get() * Vector3::forward;
	}
	Vector3 Quaternion::Right() const {
		return Get() * Vector3::right;
	}
	Vector3 Quaternion::Up() const {
		return Get() * Vector3::up;
	}
	Quaternion& Quaternion::Conjugate() {
		X = -X;
		Y = -Y;
		Z = -Z;
		return *this;
	}
	Quaternion& Quaternion::Inverse() {
		double absoluteValue = Magnitude();
		absoluteValue *= absoluteValue;
		absoluteValue = 1 / absoluteValue;
		Quaternion conjugateValue = Get().Conjugate();

		double w = conjugateValue.W * absoluteValue;
		Vector3 vector = Vector3(conjugateValue.X, conjugateValue.Y, conjugateValue.Z) * absoluteValue;

		W = w;
		X = vector.X;
		Y = vector.Y;
		Z = vector.Z;
		return *this;
	}
	//Functions
	Quaternion Quaternion::Get() const {
		return *this;
	}
	double Quaternion::Magnitude() const {
		return sqrt(exp2(W) +exp2(X) + exp2(Y) + exp2(Z));
	}
	Quaternion& Quaternion::Normalize() {
		double mag = Magnitude();
		if (mag != 0) {
			W /= mag;
			X /= mag;
			Y /= mag;
			Z /= mag;
		}
		return *this;
	}
	Quaternion Quaternion::Normalized() const {
		return Get().Normalize();
	}
	std::tuple<double, Vector3> Quaternion::GetAngleAxis() const {
		Quaternion n = Get();
		if (W > 1) n.Normalize();
		double s = 1 - n.W * n.W;
		double x,y,z;
		if (s < 0.001) {
			x = 0;
			y = 0;
			z = 1;
		} else {
			double t = sqrt(s);
			x = n.X / t;
			y = n.Y / t;
			z = n.Z / t;
		}
		double angle = 2 * acos(n.W);
		return std::tuple<double, Vector3>(angle, Vector3(x, y, z));
	}
	//Operators
	Quaternion Quaternion::operator + (const Quaternion& other) const {
		return Quaternion(W + other.W, Vector3(X,Y,Z) + Vector3(other.X, other.Y, other.Z));
	}
	Quaternion Quaternion::operator - (const Quaternion& other) const {
		return Quaternion(W - other.W, Vector3(X,Y,Z) - Vector3(other.X, other.Y, other.Z));
	}
	Quaternion Quaternion::operator * (const Quaternion& other) const {
		return Quaternion(
			W * other.W - X * other.X - Y * other.Y - Z * other.Z,
			W * other.X + X * other.W + Y * other.Z - Z * other.Y,
			W * other.Y - X * other.Z + Y * other.W + Z * other.X,
			W * other.Z + X * other.Y - Y * other.X + Z * other.W
		);
	}
	Quaternion Quaternion::operator * (const double& other) const {
		return Quaternion(
			W * other,
			X * other,
			Y * other,
			Z * other
		);
	}
	Quaternion Quaternion::operator / (const double& other) const {
		return Quaternion(
			W / other,
			X / other,
			Y / other,
			Z / other
		);
	}
	Quaternion& Quaternion::operator *= (const Quaternion& other) {
		double w = W * other.W - X * other.X - Y * other.Y - Z * other.Z;
		double x = W * other.X + X * other.W + Y * other.Z - Z * other.Y;
		double y = W * other.Y - X * other.Z + Y * other.W + Z * other.X;
		double z = W * other.Z + X * other.Y - Y * other.X + Z * other.W;

		W = w;
		X = x;
		Y = y;
		Z = z;

		return *this;
	}
	bool Quaternion::operator == (const Quaternion& other) const {
		return (W == other.W) &&(X == other.X) && (Y == other.Y) && (Z == other.Z);
	}
	//Vector operators
	Vector3 Quaternion::operator*(const Vector3& v) const {
		return v + Vector3::Cross(Vector3(X, Y, Z), (v * W + Vector3::Cross(Vector3(X, Y, Z), v))) * 2;
	}
	//Conversions
	Quaternion::operator std::string() const {
		return std::format("[{}, {}, {}, {}]", W, X, Y, Z);
	}
	Quaternion::operator Vector4() const {
		return Vector4(W, X, Y, Z);
	}
	#ifdef StevEngine_PHYSICS
	Quaternion::operator JPH::Quat() const {
		return JPH::Quat(X, Y, Z, W);
	}
	Quaternion& Quaternion::operator= (const JPH::Quat& other) {
		W = other.GetW();
		X = other.GetX();
		Y = other.GetY();
		Z = other.GetZ();
		return *this;
	}
	Quaternion::Quaternion(const JPH::Quat& other) {
		W = other.GetW();
		X = other.GetX();
		Y = other.GetY();
		Z = other.GetZ();
	}
	#endif
	//Static methods
	Quaternion Quaternion::FromAngleAxis(double angle, Vector3 axis) {
		axis.Normalize();
		double s = sin(angle / 2);
		double x = axis.X * s;
		double y = axis.Y * s;
		double z = axis.Z * s;
		double w = cos(angle / 2);
		return Quaternion(w, x, y, z);
	}
	double Quaternion::Angle(const Quaternion& a, const Quaternion& b) {
		return acos(Quaternion::Dot(a.Normalized(), b.Normalized())) * 2.0;
	}
	Quaternion Quaternion::Lerp(const Quaternion& a, const Quaternion& b, double t) {
		return (a * (1 - t) + b * t).Normalized();
	}
	Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, double t) {
		double d = Dot(a,b);
		Quaternion a2 = d >= 0 ? a : Inverse(a);
		return a2 * (1-t) + b * t;
	}
	Quaternion Quaternion::FromToRotation(Vector3 fromDirection, Vector3 toDirection) {
		fromDirection.Normalize();
		toDirection.Normalize();
		Vector3 n = Vector3::Cross(fromDirection, toDirection).Normalize();
		double cosA = Vector3::Dot(fromDirection, toDirection);
		double cosA2 = sqrt((1+cosA)/2);
		double sinA2 = sqrt((1-cosA)/2);
		return Quaternion(
			cosA2,
			n.X * sinA2,
			n.Y * sinA2,
			n.Z * sinA2
		);
	}
	Quaternion Quaternion::LookRotation(Vector3 forward, const Vector3& globalup) {
		forward.Normalize();
		Vector3 right = Vector3::Cross(globalup.Normalized(), forward);
		Vector3 up = Vector3::Cross(forward, right);

		double w = sqrt(1.0 + right.X + up.Y + forward.Z) * 0.5;
		float w4_recip = 1.0f / (4.0 * w);
		return Quaternion(
			w,
			(forward.Y - up.Z) * w4_recip,
			(right.Z - forward.X) * w4_recip,
			(up.Y - right.Y) * w4_recip
		);
	}
	Quaternion Quaternion::Inverse(const Quaternion& q) {
		return q.Get().Inverse();
	}
	Quaternion Quaternion::Conjugate(const Quaternion& q) {
		return q.Get().Conjugate();
	}
	double Quaternion::Dot(const Quaternion& a, const Quaternion& b) {
		return a.W * b.W + a.X * b.X + a.Y * b.Y + a.Z * b.Z;
	}
	double Quaternion::DegreesToRadians(double degrees) {
		return degrees * (M_PI / 180.0);
	}
	double Quaternion::RadiansToDegrees(double radians) {
		return radians * (180.0 / M_PI);
	}
}

namespace YAML {
	Node convert<StevEngine::Utilities::Quaternion>::encode(const StevEngine::Utilities::Quaternion& rhs) {
		Node node;
		node.push_back(rhs.W);
		node.push_back(rhs.X);
		node.push_back(rhs.Y);
		node.push_back(rhs.Z);
		return node;
	}
	bool convert<StevEngine::Utilities::Quaternion>::decode(const Node& node, StevEngine::Utilities::Quaternion& rhs) {
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
