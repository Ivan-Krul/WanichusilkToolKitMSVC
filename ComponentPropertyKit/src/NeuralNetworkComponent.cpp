#include "NeuralNetworkComponent.h"
#include "RandomGeneratorComponent.h"

namespace comp_prop_kit_lib {
    void NeuralNetworkComponent::setMaxThreadCount(size_t threads) {
        mThreadsMaxCount = std::min(threads, static_cast<size_t>(std::thread::hardware_concurrency()));
    }
}

template<>
comp_prop_kit_lib::NeuralNetworkComponent& comp_prop_kit_lib::PropertyList::pushComponent<comp_prop_kit_lib::NeuralNetworkComponent>();
template<>
comp_prop_kit_lib::NeuralNetworkComponent& comp_prop_kit_lib::PropertyList::getComponent<comp_prop_kit_lib::NeuralNetworkComponent>();
template<>
comp_prop_kit_lib::NeuralNetworkComponent comp_prop_kit_lib::PropertyList::getComponent<comp_prop_kit_lib::NeuralNetworkComponent>() const;
template<>
void comp_prop_kit_lib::PropertyList::popComponent<comp_prop_kit_lib::NeuralNetworkComponent>();
