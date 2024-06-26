#pragma once
#include <Jolt/Jolt.h>
#include <string>

namespace StevEngine {
	namespace Utilities {
        class Vector2;

        class Vector3 {
			public:
                //Values
				double X, Y, Z;
                //Constructors
				Vector3(double x, double y, double z);
				Vector3(const Vector3& from);
				Vector3(std::string str);
				Vector3();
                //Functions
				Vector3 Get() const;
				double Magnitude() const;
				Vector3& Normalize();
				Vector3 Normalized() const;
				Vector3& Mult(double value);
				Vector3& Divide(double value);
				//Operators
				Vector3  operator   +   (const Vector3& other) const;
				Vector3  operator   -   (const Vector3& other) const;
				Vector3& operator   +=  (const Vector3& other);
				Vector3& operator   -=  (const Vector3& other);
				Vector3  operator   *   (const double& other) const;
				Vector3  operator   /   (const double& other) const;
				bool     operator   ==  (const Vector3& other) const;
                //Conversions
				explicit operator Vector2();
				explicit operator std::string();
                operator JPH::Vec3();
				Vector3& operator= (const JPH::Vec3& other);
				Vector3(const JPH::Vec3& other);
                //Static stuff
                static Vector3 up, right, forward;
				static double Distance(Vector3 a, Vector3 b);
				static Vector3 Cross(Vector3 a, Vector3 b);
				static double Dot(Vector3 a, Vector3 b);
				static Vector3 CombineScale(Vector3 a, Vector3 b);
		};
    }
}