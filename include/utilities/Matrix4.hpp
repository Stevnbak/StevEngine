#pragma once
#include "utilities/Quaternion.hpp"
#include "utilities/Vector4.hpp"

namespace StevEngine::Utilities {
	class Matrix4 {
		public:
			//Constructors
			Matrix4(double data[4][4]);
			Matrix4(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4);
			Matrix4(const Matrix4& matrix);
			Matrix4(double v);
			Matrix4();
			//Functions
			Vector4 GetRow(int i) const;
			Vector4 GetColumn(int i) const;
			double GetDeterminant() const;
			Matrix4& Inverse();
			static Matrix4 Inverse(const Matrix4& matrix);
			Matrix4& Transpose();
			static Matrix4 Transpose(const Matrix4& matrix);
			//Operators
			Matrix4& operator= (const Matrix4& other);
			Matrix4  operator	*   (const Matrix4& matrix) const;
			Vector3  operator	*   (const Vector3& point) const;
			Matrix4  operator	*   (const double& value) const;
			Matrix4  operator	/   (const double& value) const;
			bool	 operator   ==  (const Matrix4& other) const;
			//Conversions
			explicit operator std::string() const;
			const float* data() const;
			//Static creators
			static Matrix4 FromTranslation(const Vector3& position);
			static Matrix4 FromRotation(const Quaternion& rotation);
			static Matrix4 FromScale(const Vector3& scale);
			static Matrix4 FromTranslationRotationScale(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
			static Matrix4 FromOrthographic(float width, float height, float clipNear, float clipFar);
			static Matrix4 FromOrthographic(float left, float right, float bottom, float top, float clipNear, float clipFar);
			static Matrix4 FromPerspective(float fovx, float aspect, float clipNear, float clipFar);
			//Static matrices
			static const Matrix4 identity, zero;
		private:
			double raw[4][4];
	};
}