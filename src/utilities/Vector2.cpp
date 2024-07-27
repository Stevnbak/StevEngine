#include "Vector2.hpp"
#include "Vector3.hpp"

#include <main/Log.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <format>
#include <sstream>

namespace StevEngine {
	namespace Utilities {
        //Constructors
        Vector2::Vector2(double x, double y) {
            X = x;
			Y = y;
        }
        Vector2::Vector2(const Vector2& from) {
            X = from.X;
            Y = from.Y;
        }
        Vector2::Vector2(std::string str) {
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
        }
        Vector2::Vector2() {
            X = 0;
			Y = 0;
        }
        //Functions
        Vector2 Vector2::Get() const {
            return *this;
        }
        double Vector2::Magnitude() const {
            return sqrt(exp2(X) + exp2(Y));
        }
        Vector2& Vector2::Normalize() {
            double mag = Magnitude();
			if (mag != 0) Divide(mag);
            return *this;
        }
        Vector2 Vector2::Normalized() const {
            return Get().Normalize();
        }
        Vector2& Vector2::Mult(double value) {
            X *= value;
			Y *= value;
			return *this;
        }
        Vector2& Vector2::Divide(double value) {
            X /= value;
			Y /= value;
			return *this;
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
        Vector2::operator Vector3() {
			return Vector3(X, Y, 0);
        }
        Vector2::operator std::string() {
            return std::format("[{};{}]", X, Y);
        }
        //Static stuff
        Vector2 Vector2::up = Vector2(0,1);
        Vector2 Vector2::right = Vector2(1,0);
		double Vector2::Distance(Vector2 a, Vector2 b) {
            return sqrt(exp2(a.X - b.X) + exp2(a.Y - b.Y));
        }
        double Vector2::Dot(Vector2 a, Vector2 b) {
			return (a.X * b.X) + (a.Y * b.Y);
        }
    }
}