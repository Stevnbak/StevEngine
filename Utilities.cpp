#include "Utilities.h"
#include <math.h>
namespace StevEngine {
	namespace Utilities {
		double Vector3d::Magnitude() {
			return sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
		}
		double Vector3d::Distance(Vector3d target) {
			return sqrt(pow(X - target.X, 2) + pow(Y - target.Y, 2) + pow(Z - target.Z, 2));
		}
		void Vector3d::Add(Vector3d vector) {
			X += vector.X;
			Y += vector.Y;
			Z += vector.Z;
		}
		void Vector3d::Mult(double value) {
			X *= value;
			Y *= value;
			Z *= value;
		}
		void Vector3d::Divide(double value) {
			X /= value;
			Y /= value;
			Z /= value;
		}
		void Vector3d::Normalize() {
			double mag = Magnitude();
			Divide(mag);
		}
	}
}