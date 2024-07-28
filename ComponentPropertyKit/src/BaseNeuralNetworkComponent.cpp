#include "BaseNeuralNetworkComponent.h"

#include "RandomGeneratorComponent.h"

namespace comp_prop_kit_lib {
    void BaseNeuralNetworkComponent::setup(std::vector<size_t> layerCount) noexcept {
        mNeurons.resize(layerCount.size());
        for (size_t l = 0; l < mNeurons.size(); l++) {
            mNeurons[l].resize(layerCount[l]);
            if (l) {
                for (size_t n = 0; n < mNeurons[l].size(); n++) {
                    mNeurons[l][n].weights.resize(layerCount[l - 1]);
                    mNeurons[l][n].gradientW.resize(layerCount[l - 1]);
                }
            }
        }
    }

    void BaseNeuralNetworkComponent::randomise() noexcept {
        RandomGeneratorComponent rgc;
        rgc.init();

        for (auto& layer : mNeurons) {
            for (auto& neuron : layer) {
                neuron.weights = rgc.nextRange<NeuralValue>(neuron.weights.size(), -2.0f, 2.0f);
                neuron.bias = rgc.next<NeuralValue>(-2.0f, 2.0f);
            }
        }
    }

    std::vector<NeuralValue> BaseNeuralNetworkComponent::getOutput() const noexcept {
        std::vector<NeuralValue> res;
        res.resize(mNeurons.back().size());

        for (size_t o = 0; o < mNeurons.back().size(); o++) {
            res[o] = mNeurons.back()[o].value;
        }

        return res;
    }

    void BaseNeuralNetworkComponent::mutate(float chance, float amplitude) noexcept {
        RandomGeneratorComponent rgc;
        rgc.init();

        for (auto& layer : mNeurons) {
            for (auto& neuron : layer) {
                for (auto& weight : neuron.weights) {
                    // 0   0.3           1
                    // |----|------------|
                    //  yes     no
                    if (chance < rgc.next<NeuralValue>(1)) {
                        weight += amplitude * rgc.next(-1.0f, 1.0f);
                    }
                }
            }
        }

    }

    void BaseNeuralNetworkComponent::applyGradient(NeuralValue learnCoef) {
        for (auto& layer : mNeurons) {
            for (auto& neuron : layer) {
                neuron.bias -= neuron.gradientB * learnCoef;
                for (size_t w = 0; w < neuron.weights.size(); w++) {
                    neuron.weights[w] -= neuron.gradientW[w] * learnCoef;
                }
            }
        }
    }
}
