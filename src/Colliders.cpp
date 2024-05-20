#include <stdexcept>
#include <algorithm>
#include "Colliders.hpp"

using namespace StevEngine::Utilities;

namespace StevEngine::Physics {

	Range3d Collider::CalculateRange() {
		double xMin = (position.X + gameObject->position.X) - (0.5 * scale.X * gameObject->scale.X);
		double xMax = (position.X + gameObject->position.X) + (0.5 * scale.X * gameObject->scale.X);
		double yMin = (position.Y + gameObject->position.Y) - (0.5 * scale.Y * gameObject->scale.Y);
		double yMax = (position.Y + gameObject->position.Y) + (0.5 * scale.Y * gameObject->scale.Y);
		double zMin = (position.Z + gameObject->position.Z) - (0.5 * scale.Z * gameObject->scale.Z);
		double zMax = (position.Z + gameObject->position.Z) + (0.5 * scale.Z * gameObject->scale.Z);
		return Utilities::Range3d(xMin, xMax, yMin, yMax, zMin, zMax);
	}

	bool Collider::IsInRange(Collider* other) {
		Range3d ownRange = CalculateRange();
		Range3d otherRange = other->CalculateRange();
		return ownRange.IsIntersecting(otherRange);
	}

	void Collider::Start() {
		
	}

	//Cube collider
	bool CubeCollider::IsPointInCollider(Vector3d point) {
		Range3d range = CalculateRange();
		if (range.Low.X <= point.X <= range.High.X) {
			if (range.Low.Y <= point.Y <= range.High.Y) {
				if (range.Low.Z <= point.Z <= range.High.Z) {
					return true;
				}
			}
		}
		return false;
	}

	bool CubeCollider::IsOverlapping(Collider* other) {
		if (!IsInRange(other)) return false;
		int detail = 50;
		//Calculate & check points
		double xLength = (scale.X * gameObject->scale.X) / detail;
		double yLength = (scale.Y * gameObject->scale.Y) / detail;
		double zLength = (scale.Z * gameObject->scale.Z) / detail;
		for (int x = -detail / 2; x <= detail / 2; x++) {
			double xPos = (position.X + gameObject->position.X) + (xLength * x);
			for (int y = -detail / 2; y <= detail / 2; y++) {
				double yPos = (position.Y + gameObject->position.Y) + (xLength * y);
				for (int z = -detail / 2; z <= detail / 2; z++) {
					double zPos = (position.Z + gameObject->position.Z) + (xLength * z);
					//Check point
					if (other->IsPointInCollider(Vector3d(xPos, yPos, zPos))) {
						return true;
					}
				}
			}
		}
		return false;
	}

	Vector3d CubeCollider::CollisionPoint(Collider* other) {
		if (!IsOverlapping(other)) throw std::logic_error("No overlap between colliders");
		//Ranges
		Range3d ownRange = CalculateRange();
		Range3d otherRange = other->CalculateRange();
		//Find range of intersection
		double maxX = std::min(ownRange.High.X, otherRange.High.X);
		double minX = std::max(ownRange.Low.X, otherRange.Low.X);
		double maxY = std::min(ownRange.High.Y, otherRange.High.Y);
		double minY = std::max(ownRange.Low.Y, otherRange.Low.Y);
		double maxZ = std::min(ownRange.High.Z, otherRange.High.Z);
		double minZ = std::max(ownRange.Low.Z, otherRange.Low.Z);
		Range3d intersection = Range3d(minX, maxX, minY, maxY, minZ, maxZ);
		//Return center point
		return intersection.Center();
	}
}