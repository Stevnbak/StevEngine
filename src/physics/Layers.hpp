#pragma once
#ifdef StevEngine_PHYSICS

#include <map>
#include <string>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace StevEngine::Physics {
	namespace BroadPhaseLayers
	{
		static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
		static constexpr JPH::BroadPhaseLayer MOVING(1);
	};

	//Layers
	class Layer {
		public:
			const bool isStatic;
			const std::string name;
			const JPH::ObjectLayer id;
			const JPH::BroadPhaseLayer BroadPhaseLayer;
			Layer(std::string name, bool isStatic = false);
		private:
			static JPH::ObjectLayer currentId;
			static std::map<unsigned int, Layer*> layers;
			static std::map<std::string, Layer*> layersByName;
		public:
			// Get layers
			static Layer* GetLayerByName(std::string name);
			static Layer* GetLayerById(JPH::ObjectLayer id);
	};

	class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface {
		public:
			BPLayerInterfaceImpl();
			virtual unsigned int GetNumBroadPhaseLayers() const override;
			virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;
	};
	/// Class that determines if an object layer can collide with a broadphase layer
	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
		public:
			virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
	};
	/// Class that determines if two object layers can collide
	class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
	{
		public:
			virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override;
	};
}
#endif
