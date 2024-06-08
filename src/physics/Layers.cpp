#include "Layers.hpp"

namespace StevEngine::Physics {
    JPH::ObjectLayer Layer::currentId = 0;
    std::map<uint, Layer*> Layer::layers = std::map<uint, Layer*>();
    std::map<std::string, Layer*> Layer::layersByName = std::map<std::string, Layer*>();

    // Layer class

    Layer::Layer(std::string name, bool isStatic) : name(name), isStatic(isStatic), id(currentId++), BroadPhaseLayer(isStatic ? BroadPhaseLayers::NON_MOVING : BroadPhaseLayers::MOVING) {
        layers.insert(std::make_pair(id, this));
        layersByName.insert(std::make_pair(name, GetLayerById(id)));
    }
    Layer* Layer::GetLayerByName(std::string name) {
        return layersByName.at(name);
    }
    Layer* Layer::GetLayerById(JPH::ObjectLayer id) {
        return layers.at(id);
    }

    //Jolt implementations:
    BPLayerInterfaceImpl::BPLayerInterfaceImpl() {}
    uint BPLayerInterfaceImpl::GetNumBroadPhaseLayers() const { 
        return 2; 
    }
    JPH::BroadPhaseLayer BPLayerInterfaceImpl::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const {
        return Layer::GetLayerById(inLayer)->BroadPhaseLayer;
    }
	bool ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const
    {
        if(Layer::GetLayerById(inLayer1)->isStatic) {
            return inLayer2 == BroadPhaseLayers::MOVING;
        } else {
            return true;
        }
    }
    bool ObjectLayerPairFilterImpl::ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const
    {
        if(Layer::GetLayerById(inObject1)->isStatic) {
            return !Layer::GetLayerById(inObject2)->isStatic;
        } else {
            return true;
        }
    }
}
