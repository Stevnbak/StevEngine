#include "main/Log.hpp"
#ifdef StevEngine_PHYSICS
#include "CharacterBody.hpp"
#include "main/GameObject.hpp"
#include "physics/Colliders.hpp"
#include "physics/PhysicsSystem.hpp"
#include "utilities/Quaternion.hpp"
#include "utilities/Stream.hpp"
#include "utilities/Vector3.hpp"

#include "Jolt/Physics/Collision/Shape/StaticCompoundShape.h"

#define CHARACTERBODY_TYPE "CharacterBody"

namespace StevEngine::Physics {

	JPH::TempAllocatorMalloc tempAllocator;

	JPH::CharacterVirtualSettings convertSettings(CharacterSettings input) {
		JPH::CharacterVirtualSettings settings;
		settings.mMass = input.mass;
		settings.mMaxStrength = input.maxStrength;
		settings.mPredictiveContactDistance = input.predictiveContactDistance;
		settings.mMaxCollisionIterations = input.maxCollisionIterations;
		settings.mMaxConstraintIterations = input.maxConstraintIterations;
		settings.mMinTimeRemaining = input.minTimeRemaining;
		settings.mCollisionTolerance = input.collisionTolerance;
		settings.mCharacterPadding = input.characterPadding;
		settings.mMaxNumHits = input.maxNumHits;
		settings.mHitReductionCosMaxAngle = input.hitReductionCosMaxAngle;
		settings.mPenetrationRecoverySpeed = input.penetrationRecoverySpeed;
		return settings;
	}

	JPH::CharacterVirtualSettings tempSettings;
	CharacterBody::CharacterBody(CharacterSettings input, LayerID layer)
	  : Component(),
		settings(convertSettings(input)),
		layer(layer)
	{
		jphCharacter = new JPH::CharacterVirtual(&this->settings, Utilities::Vector3(0), Utilities::Quaternion(), &physics.GetJoltSystem());
	}

	bool CharacterBody::RefreshShape() {
		GameObject& parent = GetParent();
		Utilities::Vector3 pos = parent.GetWorldPosition();
		Utilities::Quaternion rot = parent.GetWorldRotation();
		//Find all colliders:
		std::vector<Collider*> colliders;
		colliders = parent.GetAllComponents<Collider>();
		for (int i = 0; i < parent.GetChildCount(); i++) {
			std::vector<Collider*> cc = parent.GetChild(i).GetAllComponents<Collider>();
			colliders.insert(colliders.end(), cc.begin(), cc.end());
		}
		//Create new shape
		JPH::StaticCompoundShapeSettings shapeSettings = JPH::StaticCompoundShapeSettings();
		for(Collider* col : colliders) {
			if(col->GetShape())
				shapeSettings.AddShape((col->GetParent().GetWorldPosition() + col->GetPosition() - pos), (col->GetParent().GetWorldRotation() + col->GetRotation() - rot), col->GetShape());
		}
		//Create final shape
		JPH::ShapeSettings::ShapeResult result = shapeSettings.Create();
		if(result.IsValid()) {
			return SetShape(result.Get(), Utilities::Vector3(0));
		}
		Log::Error(result.GetError().c_str(), true);
		return false;
	}

	bool CharacterBody::SetShape(JPH::Ref<JPH::Shape> newShape, Utilities::Vector3 shapeOffset) {
		JPH::PhysicsSystem& system = physics.GetJoltSystem();
		//Try and set new shape
		jphCharacter->SetShapeOffset(shapeOffset);
		if(jphCharacter->SetShape(newShape, 0.01f, system.GetDefaultBroadPhaseLayerFilter(layer), system.GetDefaultLayerFilter(layer), bodyFilter, shapeFilter, tempAllocator)) {
			if(shape) shape->Release(); //Release old shape
			shape = newShape;
			settings.mShape = newShape;
			settings.mShapeOffset = shapeOffset;
			return true;
		} else { //Failed to set new shape, falling back to previous one
			settings.mShape = shape;
			jphCharacter->SetShapeOffset(settings.mShapeOffset);
			return false;
		}
	}

	CharacterSettings fromStream(Utilities::Stream& stream) {
		CharacterSettings settings;
		stream
			>> settings.mass
			>> settings.maxStrength
			>> settings.predictiveContactDistance
			>> settings.maxCollisionIterations
			>> settings.maxConstraintIterations
			>> settings.minTimeRemaining
			>> settings.collisionTolerance
			>> settings.characterPadding
			>> settings.maxNumHits
			>> settings.hitReductionCosMaxAngle
			>> settings.penetrationRecoverySpeed
		;

		return settings;
	}

	CharacterBody::CharacterBody(Utilities::Stream& stream) : CharacterBody(fromStream(stream)) {}

	Utilities::Stream CharacterBody::Export(Utilities::StreamType type) const {
		Utilities::Stream stream(type);
		stream
			<< settings.mMass
			<< settings.mMaxStrength
			<< settings.mPredictiveContactDistance
			<< settings.mMaxCollisionIterations
			<< settings.mMaxConstraintIterations
			<< settings.mMinTimeRemaining
			<< settings.mCollisionTolerance
			<< settings.mCharacterPadding
			<< settings.mMaxNumHits
			<< settings.mHitReductionCosMaxAngle
			<< settings.mPenetrationRecoverySpeed
		;

		return stream;
	}

	void CharacterBody::Start() {
		RefreshShape();

		GameObject& parent = GetParent();
		jphCharacter->SetRotation(parent.GetWorldRotation());
		jphCharacter->SetPosition(parent.GetWorldPosition());

		GetParent().Subscribe<TransformUpdateEvent>([this](const TransformUpdateEvent& e) {
			GameObject& parent = GetParent();
			if(e.rotation) jphCharacter->SetRotation(parent.GetWorldRotation());
			if(e.position) jphCharacter->SetPosition(parent.GetWorldPosition());
		});
	}

	void CharacterBody::Deactivate() {

	}

	void CharacterBody::Update(double deltaTime) {
		JPH::PhysicsSystem& system = physics.GetJoltSystem();
		velocity += system.GetGravity() * deltaTime;
		jphCharacter->SetLinearVelocity(velocity);
		jphCharacter->Update(deltaTime, system.GetGravity(), system.GetDefaultBroadPhaseLayerFilter(layer), system.GetDefaultLayerFilter(layer), bodyFilter, shapeFilter, tempAllocator);
		velocity = jphCharacter->GetLinearVelocity();
		GetParent().SetPosition(jphCharacter->GetPosition(), false);
		GetParent().SetRotation(jphCharacter->GetRotation(), false);
	}

	void CharacterBody::ExtendedUpdate(double deltaTime, JPH::CharacterVirtual::ExtendedUpdateSettings updateSettings) {
		JPH::PhysicsSystem& system = physics.GetJoltSystem();
		jphCharacter->SetLinearVelocity(velocity);
		jphCharacter->ExtendedUpdate(deltaTime, system.GetGravity(), updateSettings, system.GetDefaultBroadPhaseLayerFilter(layer), system.GetDefaultLayerFilter(layer), bodyFilter, shapeFilter, tempAllocator);
		velocity = jphCharacter->GetLinearVelocity();
		GetParent().SetPosition(jphCharacter->GetPosition(), false);
		GetParent().SetRotation(jphCharacter->GetRotation(), false);
	}

	CharacterBody::~CharacterBody() {
		shape->Release();
		delete jphCharacter;
	}
}
#endif