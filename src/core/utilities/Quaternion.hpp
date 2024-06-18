#pragma once
#include <Jolt/Jolt.h>
#include <string>
#include <tuple>

#include "Vector3.hpp"

namespace StevEngine {
	namespace Utilities {
        class Quaternion {
			public:
                //Values
				double W, X, Y, Z;
                //Constructors
				Quaternion(double w, double x, double y, double z);
				Quaternion(double w, Vector3 vector);
				Quaternion(const Quaternion& from);
				Quaternion(std::string str);
				Quaternion();
                //Directions
				Vector3 Forward();
				Vector3 Right();
				Vector3 Up();
                Quaternion& Conjugate();
                Quaternion& Inverse();
				//Functions
				Quaternion Get() const;
				double Magnitude() const;
				Quaternion& Normalize();
				Quaternion Normalized() const;
				std::tuple<double, Vector3> GetAngleAxis() const;
				//Operators
				Quaternion  operator    +   (const Quaternion& other) const;
				Quaternion  operator    -   (const Quaternion& other) const;
				Quaternion  operator    *   (const Quaternion& other) const;
				Quaternion  operator    *   (const double& other) const;
				Quaternion  operator    /   (const double& other) const;
				Quaternion& operator    *=  (const Quaternion& other);
				bool        operator    ==  (const Quaternion& other) const;
                //Vector operators
                Vector3 operator*(const Vector3& other) const;
				//Conversions
				operator JPH::Quat();
				explicit operator std::string();
				Quaternion& operator= (const JPH::Quat& other);
				Quaternion(const JPH::Quat& other);
                //Static methods
                static Quaternion FromAngleAxis(double angle, Vector3 axis);
                static double Angle(Quaternion a, Quaternion b);
                static Quaternion Lerp(Quaternion a, Quaternion b, double t);
                static Quaternion Slerp(Quaternion a, Quaternion b, double t);
                static Quaternion FromToRotation(Vector3 fromDirection, Vector3 toDirection);
                static Quaternion LookRotation(Vector3 forward, Vector3 upwards = Vector3::up);
				static Quaternion Conjugate(Quaternion q);
				static Quaternion Inverse(Quaternion q);
				static double Dot(Quaternion a, Quaternion b);
				static double DegreesToRadians(double degrees);
				static double RadiansToDegrees(double radians);
		};
    }
}