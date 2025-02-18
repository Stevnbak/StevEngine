#pragma once
#ifdef StevEngine_PHYSICS

#include <map>
#include <string>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace StevEngine::Physics {
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
		public:
			const bool isStatic;			///< Whether objects are static
			const std::string name;		 ///< Layer name
			const JPH::ObjectLayer id;	  ///< Unique layer ID
			const JPH::BroadPhaseLayer BroadPhaseLayer;  ///< Broad phase settings

			/**
			 * @brief Create new physics layer
			 * @param name Layer identifier
			 * @param isStatic Whether objects are static
			 */
			Layer(std::string name, bool isStatic = false);

		private:
			static JPH::ObjectLayer currentId;  ///< Next layer ID to assign
			static std::map<unsigned int, Layer*> layers;  ///< All layers by ID
			static std::map<std::string, Layer*> layersByName;  ///< All layers by name

		public:
			/**
			 * @brief Get layer by name
			 * @param name Layer identifier
			 * @return Layer pointer
			 */
			static Layer* GetLayerByName(std::string name);

			/**
			 * @brief Get layer by ID
			 * @param id Layer ID
			 * @return Layer pointer
			 */
			static Layer* GetLayerById(JPH::ObjectLayer id);
	};

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
#endif