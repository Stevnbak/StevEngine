#pragma once

namespace StevEngine {
	namespace Utilities {
		class Vector3d {
			public:
				double X, Y, Z;
				double Magnitude();
				double Distance(Vector3d target);
				void Add(Vector3d vector);
				void Normalize();
				void Mult(double value);
				void Divide(double value);
		};
	}
}