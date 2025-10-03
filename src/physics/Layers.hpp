#pragma once
#ifdef StevEngine_PHYSICS
#include <unordered_map>

#include "Jolt.h"
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>


namespace StevEngine::Physics {
	typedef JPH::ObjectLayer LayerID;

	namespace BroadPhaseLayers
	{
		/** @brief Layer for static/non-moving objects */
		static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
		/** @brief Layer for dynamic/moving objects */
		static constexpr JPH::BroadPhaseLayer MOVING(1);
	};

	/**
	 * @brief Physics collision layer
	 *
	 * Defines collision filtering and broad phase settings for physics objects.
	 * Controls which objects can collide with each other.
	 */
	class Layer {
		friend class LayerManager;

		public:
			const bool isStatic;							///< Whether objects are static
			const LayerID id;	  							///< Unique layer ID
			const JPH::BroadPhaseLayer BroadPhaseLayer;  	///< Broad phase settings

			/**
			 * @brief Copy physics layer
			 * @param copy Layer to copy
			 */
			Layer(const Layer& copy);

		private:
			/**
			 * @brief Create new physics layer
			 * @param id Layer identifier
			 * @param isStatic Whether objects are static
			 */
			Layer(LayerID id, bool isStatic = false);
	};

	/**
	 * @brief Physics collision layer manager
	 *
	 * Defines set of layers and which collisions are allowed.
	 */
	class LayerManager {
		public:
			enum DefaultLayers {
				STATIC = (LayerID)0,
				DEFAULT = (LayerID)1,
			};

			LayerManager();

			/**
			 * @brief Create a new physics layer
			 * @param isStatic Whether objects are static
			 * @return Layer ID
			 */
			LayerID CreateLayer(bool isStatic = true);

			/**
			 * @brief Get layer by ID
			 * @param id Layer ID
			 * @return Layer
			 */
			const Layer& GetLayer(LayerID id);

		private:
			std::unordered_map<LayerID, Layer> layers;

			LayerID currentId;  ///< Next layer ID to assign
	};

	// Jolt management

	/**
	 * @brief Broad phase layer interface for Jolt
	 *
	 * Implements Jolt physics engine interface for broad phase collision detection.
	 * Manages layer relationships for initial collision filtering.
	 */
	class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface {
		public:
			/**
			 * @brief Create broad phase interface
			 */
			BPLayerInterfaceImpl();

			/**
			 * @brief Get number of broad phase layers
			 * @return Number of layers
			 */
			virtual unsigned int GetNumBroadPhaseLayers() const override;

			/**
			 * @brief Get broad phase layer for object layer
			 * @param inLayer Object layer to check
			 * @return Corresponding broad phase layer
			 */
			virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;
	};

	/**
	 * @brief Filter for broad phase layer collisions
	 *
	 * Controls which broad phase layers can interact with each other.
	 */
	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
		public:
			/**
			 * @brief Check if layers should collide
			 * @param inLayer1 First object layer
			 * @param inLayer2 Second broad phase layer
			 * @return true if layers should check for collisions
			 */
			virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
	};

	/**
	 * @brief Filter for object layer collisions
	 *
	 * Controls which object layers can interact with each other.
	 */
	class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
	{
		public:
			/**
			 * @brief Check if layers should collide
			 * @param inObject1 First object layer
			 * @param inObject2 Second object layer
			 * @return true if layers should check for collisions
			 */
			virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override;
	};
}

template<> struct std::hash<StevEngine::Physics::Layer> {
	/**
	 * @brief Hash function for Physics layer
	 * @param k Layer to hash
	 * @return Hash value
	 */
	std::size_t operator()(const StevEngine::Physics::Layer& k) const {
		return std::hash<StevEngine::Physics::LayerID>()(k.id);
	}
};
#endif