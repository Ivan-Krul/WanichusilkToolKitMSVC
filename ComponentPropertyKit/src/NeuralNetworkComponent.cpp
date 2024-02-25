#include "NeuralNetworkComponent.h"
#include "RandomGeneratorComponent.h"

namespace comp_prop_kit_lib {
    void NeuralNetworkComponent::setMaxThreadCount(size_t threads) {
        mThreadsMaxCount = std::min(threads, static_cast<size_t>(std::thread::hardware_concurrency()));
    }
}
