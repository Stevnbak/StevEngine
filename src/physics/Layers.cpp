#include "main/SceneManager.hpp"
#ifdef StevEngine_PHYSICS
#include "Layers.hpp"

namespace StevEngine::Physics {
	// Layer class

	Layer::Layer(LayerID id, bool isStatic) : isStatic(isStatic), id(id), BroadPhaseLayer(isStatic ? BroadPhaseLayers::NON_MOVING : BroadPhaseLayers::MOVING) {

	}

	Layer::Layer(const Layer& copy) : isStatic(copy.isStatic), id(copy.id), BroadPhaseLayer(copy.BroadPhaseLayer) {}

	// Layer Manager

	LayerManager::LayerManager() {
		layers.emplace(STATIC, Layer(STATIC, true));
		layers.emplace(DEFAULT, Layer(DEFAULT, false));
		currentId = DEFAULT + 1;
	}

	LayerID LayerManager::CreateLayer(bool isStatic) {
		layers.emplace(currentId, Layer(currentId, isStatic));
		return currentId++;
	}

	const Layer& LayerManager::GetLayer(LayerID id) {
		return layers.at(id);
	}

	//Jolt implementations:
	BPLayerInterfaceImpl::BPLayerInterfaceImpl() {}
	unsigned int BPLayerInterfaceImpl::GetNumBroadPhaseLayers() const {
		return 2;
	}
	JPH::BroadPhaseLayer BPLayerInterfaceImpl::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const {
		return sceneManager.GetActiveScene().GetLayers().GetLayer(inLayer).BroadPhaseLayer;
	}
	bool ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const
	{
		if(sceneManager.GetActiveScene().GetLayers().GetLayer(inLayer1).isStatic) {
			return inLayer2 == BroadPhaseLayers::MOVING;
		} else {
			return true;
		}
	}
	bool ObjectLayerPairFilterImpl::ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const
	{
		if(sceneManager.GetActiveScene().GetLayers().GetLayer(inObject1).isStatic) {
			return !sceneManager.GetActiveScene().GetLayers().GetLayer(inObject2).isStatic;
		} else {
			return true;
		}
	}
}
#endif
