#include "Matrix4.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Serializable.hpp"
#include "utilities/Vector2.hpp"
#include "utilities/Vector3.hpp"
#include "utilities/Vector4.hpp"
#include <cmath>
#include <string>

namespace StevEngine::Utilities {
	//Constructors
	Matrix4::Matrix4(double data[4][4]) {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				raw[i][j] = data[i][j];
			}
		}
	};
	Matrix4::Matrix4(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4) {
		for(int i = 0; i < 4; i++) {
			const Vector4& row = (i == 0 ? row1 : (i == 1 ? row2 : (i == 2 ? row3 : row4)));
			raw[i][0] = row.W;
			raw[i][1] = row.X;
			raw[i][2] = row.Y;
			raw[i][3] = row.Z;
		}
	}
	Matrix4::Matrix4(const Matrix4& matrix) {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				raw[i][j] = matrix.raw[i][j];
			}
		}
	}
	Matrix4::Matrix4(double v) {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				raw[i][j] = v;
			}
		}
	}
	Matrix4::Matrix4() : Matrix4(identity) {}
	//Functions
	Vector4 Matrix4::GetRow(int i) const {
		return Vector4(raw[i][0], raw[i][1], raw[i][2], raw[i][3]);
	}
	Vector4 Matrix4::GetColumn(int i) const {
		return Vector4(raw[0][i], raw[1][i], raw[2][i], raw[3][i]);
	}
	double GetDeterminantMatrix2(Vector2 row1, Vector2 row2) {
		return row1.X * row2.Y - row1.Y * row2.X;
	}
	double GetDeterminantMatrix3(Vector3 row1, Vector3 row2, Vector3 row3) {
		double a = row1.X;
		double b = row1.Y;
		double c = row1.Z;

		double da = GetDeterminantMatrix2(Vector2(row2.Y, row2.Z), Vector2(row3.Y, row3.Z));
		double db = GetDeterminantMatrix2(Vector2(row2.X, row2.Z), Vector2(row3.X, row3.Z));
		double dc = GetDeterminantMatrix2(Vector2(row2.X, row2.Y), Vector2(row2.X, row2.Y));

		return (a * da) - (b * db) + (c * dc);
	}
	double Matrix4::GetDeterminant() const {
		double a = raw[0][0];
		double b = raw[0][1];
		double c = raw[0][2];
		double d = raw[0][3];

		double da = GetDeterminantMatrix3(
			Vector3(raw[1][1], raw[1][2], raw[1][3]),
			Vector3(raw[2][1], raw[2][2], raw[2][3]),
			Vector3(raw[3][1], raw[3][2], raw[3][3])
		);
		double db = GetDeterminantMatrix3(
			Vector3(raw[0][1], raw[0][2], raw[0][3]),
			Vector3(raw[2][1], raw[2][2], raw[2][3]),
			Vector3(raw[3][1], raw[3][2], raw[3][3])
		);
		double dc = GetDeterminantMatrix3(
			Vector3(raw[0][1], raw[0][2], raw[0][3]),
			Vector3(raw[1][1], raw[1][2], raw[1][3]),
			Vector3(raw[3][1], raw[3][2], raw[3][3])
		);
		double dd = GetDeterminantMatrix3(
			Vector3(raw[0][1], raw[0][2], raw[0][3]),
			Vector3(raw[1][1], raw[1][2], raw[1][3]),
			Vector3(raw[2][1], raw[2][2], raw[2][3])
		);

		return (a * da) - (b * db) + (c * dc) - (d * dd);
	}
	Matrix4& Matrix4::Inverse() {
		Matrix4 result = Inverse(*this);
		//Set current raw
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				raw[i][j] = result.raw[i][j];
			}
		}
		return *this;
	}
	Matrix4 Matrix4::Inverse(const Matrix4& matrix) {
		Matrix4 result;
		//Matrix of Minors
		for(int i = 0; i < 4; i++) {
			int row1 = i + 1;
			int row2 = i + 2;
			int row3 = i + 3;
			if(row1 > 3) row1 -= 4;
			if(row2 > 3) row2 -= 4;
			if(row3 > 3) row3 -= 4;
			for(int j = 0; j < 4; j++) {
				int col1 = i + 1;
				int col2 = i + 2;
				int col3 = i + 3;
				if(col1 > 3) col1 -= 4;
				if(col2 > 3) col2 -= 4;
				if(col3 > 3) col3 -= 4;

				result.raw[i][j] = GetDeterminantMatrix3(
					Vector3(matrix.raw[row1][col1], matrix.raw[row1][col2], matrix.raw[row1][col3]),
					Vector3(matrix.raw[row2][col1], matrix.raw[row2][col2], matrix.raw[row2][col3]),
					Vector3(matrix.raw[row3][col1], matrix.raw[row3][col2], matrix.raw[row3][col3])
				);
			}
		}
		//Matrix of Cofactors
		for(int i = 0; i < 4; i++) {
			bool evenRow = (i % 2 == 0);
			for(int j = 0; j < 4; j++) {
				bool evenColumn = (j % 2 == 0);
				if(evenRow && !evenColumn) {
					result.raw[i][j] = -result.raw[i][j];
				} else if(!evenRow && evenColumn) {
					result.raw[i][j] = -result.raw[i][j];
				}
			}
		}
		//Adjugate/Adjoint
		result.Transpose();
		//Multiply by 1/Determinant
		return result * (1 / matrix.GetDeterminant());
	}
	Matrix4 Matrix4::Transpose(const Matrix4& matrix) {
		return Matrix4(matrix.GetColumn(0), matrix.GetColumn(1), matrix.GetColumn(2), matrix.GetColumn(3));
	}
	Matrix4& Matrix4::Transpose() {
		//Get rows
		Vector4 rows[4] = {
			GetRow(0),
			GetRow(1),
			GetRow(2),
			GetRow(3)
		};
		//Set columns
		for(int i = 0; i < 4; i++) {
			raw[0][i] = rows[i].W;
			raw[1][i] = rows[i].X;
			raw[2][i] = rows[i].Y;
			raw[3][i] = rows[i].Z;
		}
		return *this;
	}
	//Operators
	Matrix4& Matrix4::operator= (const Matrix4& other) {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				raw[i][j] = other.raw[i][j];
			}
		}
		return *this;
	}
	Matrix4  Matrix4::operator	*   (const Matrix4& other) const {
		Matrix4 result;
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				result.raw[i][j] = Vector4::Dot(this->GetRow(i), other.GetColumn(j));
			}
		}
		return result;
	}
	Vector3  Matrix4::operator	*   (const Vector3& point) const {
		return Vector3(
			(point.X * raw[0][0]) + (point.Y * raw[0][1]) + (point.Z * raw[0][2]) + raw[0][3],
			(point.X * raw[1][0]) + (point.Y * raw[1][1]) + (point.Z * raw[1][2]) + raw[1][3],
			(point.X * raw[2][0]) + (point.Y * raw[2][1]) + (point.Z * raw[2][2]) + raw[2][3]
		);
	}
	Matrix4  Matrix4::operator	*   (const double& value) const {
		Matrix4 result;
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				result.raw[i][j] *= value;
			}
		}
		return result;
	}
	Matrix4  Matrix4::operator	/   (const double& value) const {
		Matrix4 result;
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				result.raw[i][j] *= value;
			}
		}
		return result;
	}
	bool	 Matrix4::operator   ==  (const Matrix4& other) const {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				if(raw[i][j] != other.raw[i][j]) return false;
			}
		}
		return true;
	}
	//Conversions
	Matrix4::operator std::string() const {
		std::string str;
		for(int i = 0; i < 4; i++) {
			str += "[";
			for(int j = 0; j < 4; j++) {
				str += std::to_string(raw[i][j]) + (j == 3 ? "" : ", ");
			}
			str += std::string("]") + (i == 3 ? "" : ", ");
		}
		return "[" + str + "]";
	}
	const float* Matrix4::data() const {
		float* out = new float[4*4];
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				out[i + j * 4] = (float)raw[i][j];
			}
		}
		return out;
	}
	//Static creators
	Matrix4 Matrix4::FromTranslation(const Vector3& position) {
		return Matrix4(
			{1,0,0,position.X},
			{0,1,0,position.Y},
			{0,0,1,position.Z},
			{0,0,0,1}
		);
	}
	Matrix4 Matrix4::FromRotation(const Quaternion& q) {
		return Matrix4(
			{(2*((q.W*q.W) + (q.X*q.X)) - 1), 	(2*((q.X*q.Y) - (q.W*q.Z))), 		(2*((q.X*q.Z) + (q.W*q.Y))), 		0},
			{(2*((q.X*q.Y) + (q.W*q.Z))), 		(2*((q.W*q.W) + (q.Y*q.Y)) - 1), 	(2*((q.Y*q.Z) - (q.W*q.X))), 		0},
			{(2*((q.X*q.Z) - (q.W*q.Y))), 		(2*((q.Y*q.Z) + (q.W*q.X))), 		(2*((q.W*q.W) + (q.Z*q.Z)) - 1), 	0},
			{0,									0,									0,									1}
		);
	}
	Matrix4 Matrix4::FromScale(const Vector3& scale) {
		return Matrix4(
			{scale.X,0,0,0},
			{0,scale.Y,0,0},
			{0,0,scale.Z,0},
			{0,0,0,1}
		);
	}
	Matrix4 Matrix4::FromTranslationRotationScale(const Vector3& position, const Quaternion& rotation, const Vector3& scale) {
		//Translation
		Utilities::Matrix4 translationMatrix = Utilities::Matrix4::FromTranslation(position);
		//Rotate
		Utilities::Matrix4 rotationMatrix = Utilities::Matrix4::FromRotation(rotation);
		//Scale
		Utilities::Matrix4 scaleMatrix = Utilities::Matrix4::FromScale(scale);
		//Combined
		return (translationMatrix * (rotationMatrix * scaleMatrix));
	}
	Matrix4 Matrix4::FromOrthographic(float width, float height, float clipNear, float clipFar) {
		return Matrix4({
			{2.0/width, 0, 0, 0},
			{0, 2.0/width, 0, 0},
			{0, 0, (-2)/(clipFar - clipNear), -((clipNear + clipFar)/(clipFar - clipNear))},
			{0, 0, 0, 1},
		});
	}
	Matrix4 Matrix4::FromOrthographic(float left, float right, float bottom, float top, float clipNear, float clipFar) {
		return Matrix4({
			{2.0/(right - left), 0, 0, -((right + left)/(right - left))},
			{0, 2.0/(top - bottom), 0, -((top + bottom)/(top - bottom))},
			{0, 0, (-2)/(clipFar - clipNear), -((clipNear + clipFar)/(clipFar - clipNear))},
			{0, 0, 0, 1},
		});
	}
	Matrix4 Matrix4::FromPerspective(float fovx, float aspect, float clipNear, float clipFar) {
		float tanFovX = std::tan(Quaternion::DegreesToRadians(fovx) * 0.5);
		float tanFovY = std::tan(Quaternion::DegreesToRadians(fovx / aspect) * 0.5);

		return Matrix4({
			{1.0/tanFovX, 0, 0, 0},
			{0, 1.0/tanFovY, 0, 0},
			{0, 0, (-(clipNear + clipFar))/(clipFar - clipNear), (-2 * clipNear * clipFar)/(clipFar - clipNear)},
			{0, 0, -1, 0},
		});
	}
	//Static matrices
	const Matrix4 Matrix4::identity = Matrix4(
		Vector4(1,0,0,0),
		Vector4(0,1,0,0),
		Vector4(0,0,1,0),
		Vector4(0,0,0,1)
	);
	const Matrix4 Matrix4::zero = Matrix4(0.0);
}

namespace StevEngine {
	//Read from text stream
	template <> Utilities::Matrix4 TextStream::Read<Utilities::Matrix4>() {
		double raw[4][4];
		char s;
		for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++) *this >> raw[j][i];
		return Utilities::Matrix4(raw);
	}
	//Write to text stream
	template <> void TextStream::Write<Utilities::Matrix4>(const Utilities::Matrix4& data) {
		auto raw = data.data();
		for(int i = 0; i < 4 * 4; i++) *this << raw[i];
	}
	//Read from text stream
	template <> Utilities::Matrix4 BinaryStream::Read<Utilities::Matrix4>() {
		double raw[4][4];
		for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++) *this >> raw[j][i];
		return Utilities::Matrix4(raw);
	}
	//Write to text stream
	template <> void BinaryStream::Write<Utilities::Matrix4>(const Utilities::Matrix4& data) {
		auto raw = data.data();
		for(int i = 0; i < 4 * 4; i++) *this << raw[i];
	}
}
