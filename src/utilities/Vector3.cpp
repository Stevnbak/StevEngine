#include "Vector3.hpp"
#include "Vector2.hpp"

#include <main/Log.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <format>
#include <sstream>

namespace StevEngine {
	namespace Utilities {
        //Constructors
        Vector3::Vector3(double x, double y, double z) {
            X = x;
			Y = y;
			Z = z;
        }
        Vector3::Vector3(const Vector3& from) {
            X = from.X;
            Y = from.Y;
            Z = from.Z;
        }
        Vector3::Vector3(std::string str) {
            if(!str.starts_with("[") || !str.ends_with("]") || str.find(";") == 0) {
				Log::Error("3D Vector string not valid.", true);
				return;
			}
			//Create stream
			std::istringstream ss(str.substr(1, str.length() - 2));
			std::string s;
			//Get values
			std::getline(ss, s, ';');
			X = stod(s);
			std::getline(ss, s, ';');
			Y = stod(s);
			std::getline(ss, s, ';');
			Z = stod(s);
        }
        Vector3::Vector3() {
            X = 0;
			Y = 0;
			Z = 0;
        }
        //Functions
        Vector3 Vector3::Get() const {
            return *this;
        }
        double Vector3::Magnitude() const {
            return sqrt(exp2(X) + exp2(Y) + exp2(Z));
        }
        Vector3& Vector3::Normalize() {
            double mag = Magnitude();
			if (mag != 0) Divide(mag);
            return *this;
        }
        Vector3 Vector3::Normalized() const {
            return Get().Normalize();
        }
        Vector3& Vector3::Mult(double value) {
            X *= value;
			Y *= value;
			Z *= value;
			return *this;
        }
        Vector3& Vector3::Divide(double value) {
            X /= value;
			Y /= value;
			Z /= value;
			return *this;
        }
        //Operators
        Vector3 Vector3::operator + (const Vector3& other) const {
			return Vector3(X + other.X, Y + other.Y, Z + other.Z);
        }
        Vector3 Vector3::operator - (const Vector3& other) const {
			return Vector3(X - other.X, Y - other.Y, Z - other.Z);
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
        Vector3::operator Vector2() {
			return Vector2(X, Y);
        }
        Vector3::operator std::string() {
            return std::format("[{};{};{}]", X, Y, Z);
        }
        #ifdef StevEngine_PHYSICS
        Vector3::operator JPH::DVec3() {
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
        Vector3::operator JPH::Vec3() {
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
        //Static stuff
        Vector3 Vector3::up = Vector3(0,1,0);
        Vector3 Vector3::right = Vector3(1,0,0);
        Vector3 Vector3::forward = Vector3(0,0,1);
		double Vector3::Distance(Vector3 a, Vector3 b) {
            return sqrt(exp2(a.X - b.X) + exp2(a.Y - b.Y) + exp2(a.Z - b.Z));
        }
        Vector3 Vector3::Cross(Vector3 a, Vector3 b) {
			return Vector3(
                a.Y * b.Z - a.Z * b.Y, 
                -(a.X * b.Z - a.Z * b.X), 
                a.X * b.Y - a.Y * b.X
            );
        }
        double Vector3::Dot(Vector3 a, Vector3 b) {
			return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
        }
        Vector3 Vector3::CombineScale(Vector3 a, Vector3 b) {
            Utilities::Vector3 s = a.Get();
			s.X *= b.X;
			s.Y *= b.Y;
			s.Z *= b.Z;
            return s;
        }
    }
}