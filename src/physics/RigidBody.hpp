#pragma once
#include <core/GameObject.hpp>
#include <core/Utilities.hpp>
#include <physics/Colliders.hpp>
#include <physics/Layers.hpp>

//Jolt imports
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/MutableCompoundShape.h>
#include <Jolt/Physics/Body/BodyManager.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

namespace StevEngine::Physics {
	//physics component
	class RigidBody : public Component {
		//Information
		public:
			static const bool unique = true;
			//Basic values
			JPH::Body* GetBody() { return body; }
		private:
			JPH::EMotionType motionType;
			Layer* layer;
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
			//Export
			void Export(tinyxml2::XMLElement* element);
			//Constructor
			RigidBody(JPH::EMotionType motionType, Layer* layer, float mass = 1);
			RigidBody(tinyxml2::XMLElement* element);
			//Other functions
			void UpdateColliders();
	};
}