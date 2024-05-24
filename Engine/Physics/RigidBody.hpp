#pragma once
#include <Core/GameObject.hpp>
#include <Core/Utilities.hpp>
#include <Physics/Colliders.hpp>

//Jolt imports
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/MutableCompoundShape.h>
#include <Jolt/Physics/Body/BodyManager.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

namespace StevEngine::Physics {
	//Physics component
	class RigidBody : public Component {
		//Information
		public:
			//Basic values
			JPH::Body* GetBody() { return body; }
		private:
			JPH::EMotionType motionType;
			JPH::ObjectLayer layer;
			std::vector<Collider*> colliders;
			JPH::Body* body;
			JPH::Ref<JPH::Shape> shape;
			float mass;
		//Functions
		public:
			//Basic component functions
			void Start();
			void Update(double deltaTime);
			void Draw() {}
			void Destroy();
			//Constructor
			RigidBody(JPH::EMotionType motionType, JPH::ObjectLayer layer, float mass = 1);
			//Other functions
			void UpdateColliders();
	};
}