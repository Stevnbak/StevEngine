#pragma once
#include "utilities/Quaternion.hpp"
#include "utilities/Vector4.hpp"

namespace StevEngine::Utilities {
	/**
	 * @brief 4x4 matrix for 3D transformations
	 *
	 * Implements common matrix operations for 3D transformations including
	 * translation, rotation, scaling, and projection matrices.
	 */
	class Matrix4 {
		public:
			/**
			 * @brief Create matrix from raw 4x4 array
			 * @param data 4x4 array of values
			 */
			Matrix4(double data[4][4]);

			/**
			 * @brief Create matrix from row vectors
			 * @param row1 First row vector
			 * @param row2 Second row vector
			 * @param row3 Third row vector
			 * @param row4 Fourth row vector
			 */
			Matrix4(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4);

			/**
			 * @brief Copy constructor
			 * @param matrix Matrix to copy
			 */
			Matrix4(const Matrix4& matrix);

			/**
			 * @brief Create matrix filled with value
			 * @param v Value to fill with
			 */
			Matrix4(double v);

			/** @brief Create identity matrix */
			Matrix4();

			/**
			 * @brief Get row vector
			 * @param i Row index (0-3)
			 * @return Row as Vector4
			 */
			Vector4 GetRow(int i) const;

			/**
			 * @brief Get column vector
			 * @param i Column index (0-3)
			 * @return Column as Vector4
			 */
			Vector4 GetColumn(int i) const;

			/**
			 * @brief Calculate matrix determinant
			 * @return Determinant value
			 */
			double GetDeterminant() const;

			/**
			 * @brief Invert this matrix
			 * @return Reference to this
			 */
			Matrix4& Inverse();

			/**
			 * @brief Create inverse of matrix
			 * @param matrix Matrix to invert
			 * @return Inverted matrix
			 */
			static Matrix4 Inverse(const Matrix4& matrix);

			/**
			 * @brief Transpose this matrix
			 * @return Reference to this
			 */
			Matrix4& Transpose();

			/**
			 * @brief Create transpose of matrix
			 * @param matrix Matrix to transpose
			 * @return Transposed matrix
			 */
			static Matrix4 Transpose(const Matrix4& matrix);

			//Operators
			Matrix4& operator= (const Matrix4& other);			///< Assignment
			Matrix4  operator* (const Matrix4& matrix) const;	 ///< Matrix multiplication
			Vector3  operator* (const Vector3& point) const;	  ///< Point transformation
			Matrix4  operator* (const double& value) const;	   ///< Scalar multiplication
			Matrix4  operator/ (const double& value) const;	   ///< Scalar division
			bool	operator== (const Matrix4& other) const;	  ///< Equality comparison

			//Conversions
			explicit operator std::string() const;  ///< Convert to string
			const float* data() const;			 ///< Get raw float array

			//Static creators
			/**
			 * @brief Create translation matrix
			 * @param position Translation vector
			 * @return Translation matrix
			 */
			static Matrix4 FromTranslation(const Vector3& position);

			/**
			 * @brief Create rotation matrix
			 * @param rotation Rotation quaternion
			 * @return Rotation matrix
			 */
			static Matrix4 FromRotation(const Quaternion& rotation);

			/**
			 * @brief Create scale matrix
			 * @param scale Scale vector
			 * @return Scale matrix
			 */
			static Matrix4 FromScale(const Vector3& scale);

			/**
			 * @brief Create combined TRS matrix
			 * @param position Translation
			 * @param rotation Rotation
			 * @param scale Scale
			 * @return Combined transformation matrix
			 */
			static Matrix4 FromTranslationRotationScale(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

			/**
			 * @brief Create orthographic projection
			 * @param width View width
			 * @param height View height
			 * @param clipNear Near clip plane
			 * @param clipFar Far clip plane
			 * @return Orthographic projection matrix
			 */
			static Matrix4 FromOrthographic(float width, float height, float clipNear, float clipFar);

			/**
			 * @brief Create orthographic projection with bounds
			 * @param left Left bound
			 * @param right Right bound
			 * @param bottom Bottom bound
			 * @param top Top bound
			 * @param clipNear Near clip plane
			 * @param clipFar Far clip plane
			 * @return Orthographic projection matrix
			 */
			static Matrix4 FromOrthographic(float left, float right, float bottom, float top, float clipNear, float clipFar);

			/**
			 * @brief Create perspective projection
			 * @param fovx Horizontal field of view in degrees
			 * @param aspect Aspect ratio (width/height)
			 * @param clipNear Near clip plane
			 * @param clipFar Far clip plane
			 * @return Perspective projection matrix
			 */
			static Matrix4 FromPerspective(float fovx, float aspect, float clipNear, float clipFar);

			//Static matrices
			static const Matrix4 identity;  ///< Identity matrix
			static const Matrix4 zero;	  ///< Zero matrix

		private:
			double raw[4][4];  ///< Raw matrix data
	};
}