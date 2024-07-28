#pragma once
#include <vector>
#include <iostream>

#include "Component.h"
#include "NNActivationFunc.h"
#include "NNLossFunc.h"

namespace comp_prop_kit_lib {
    typedef float NeuralValue;

    class BaseNeuralNetworkComponent
        : public Component {
    public:
        struct Neuron {
            NeuralValue value;
            std::vector<NeuralValue> weights;
            std::vector<NeuralValue> gradientW;
            NeuralValue bias;
            NeuralValue gradientB;
        };

        inline BaseNeuralNetworkComponent(ComponentTypeHash infoHash = typeid(BaseNeuralNetworkComponent).hash_code())
            : Component(infoHash) {
        }

        inline BaseNeuralNetworkComponent(const std::vector<std::vector<Neuron>>& mNeurons, ComponentTypeHash infoHash = typeid(BaseNeuralNetworkComponent).hash_code())
            : mNeurons(mNeurons), Component(infoHash) {
        }
        
        inline BaseNeuralNetworkComponent(BaseNeuralNetworkComponent&& base) noexcept
            : Component(base), mNeurons(std::move(base.mNeurons)) {
        }

        // setup
        void setup(std::vector<size_t> layerCount) noexcept;
        void randomise() noexcept;
        template<typename T> void setInput(std::vector<T> input) noexcept;

        // forward
        template<typename TActivFunc> void forward2Layers(size_t indexLeft, size_t indexRight);

        // output
        _NODISCARD std::vector<NeuralValue> getOutput() const noexcept;

        // loss
        template <typename TLossFunc>_NODISCARD float getLossResult(const std::vector<NeuralValue>& answer);

        // apply weight
        void mutate(float chance, float amplitude) noexcept;
        void applyGradient(NeuralValue learnCoef);

        std::vector<std::vector<Neuron>>& getTopology() noexcept {
            return mNeurons;
        }

    protected:
        template<typename TActivFunc> void forward(const std::vector<Neuron>& leftLayer, std::vector<Neuron>& rightLayer);
        template<typename TActivFunc> void forwardPart(const std::vector<Neuron>& leftLayer, std::vector<Neuron>& rightLayer, size_t rightBegin, size_t rightEnd);

        std::vector<std::vector<Neuron>> mNeurons;
    };

    template<typename T>
    inline void BaseNeuralNetworkComponent::setInput(std::vector<T> input) noexcept {
        if (mNeurons.empty())
            return;

        auto& front = mNeurons.front();
        for (size_t i = 0; i < front.size(); i++) {
            if (input.size() <= i)
                front[i].value = 0;
            else
                front[i].value = static_cast<NeuralValue>(input[i]);
        }
    }

    template<typename TActivFunc>
    inline void BaseNeuralNetworkComponent::forward2Layers(size_t indexLeft, size_t indexRight) {
        if (indexLeft >= mNeurons.size() || indexRight >= mNeurons.size()) {
            std::cerr << "out of range: one of the layer index is out of range\n";
            throw std::out_of_range("one of the layer index is out of range");
        }

        forward<TActivFunc>(mNeurons[indexLeft], mNeurons[indexRight]);
    }

    template<typename TActivFunc>
    inline void BaseNeuralNetworkComponent::forward(const std::vector<Neuron>& leftLayer, std::vector<Neuron>& rightLayer) {
        forwardPart<TActivFunc>(leftLayer, rightLayer, 0, rightLayer.size());
    }

    template<typename TActivFunc>
    inline void BaseNeuralNetworkComponent::forwardPart(const std::vector<Neuron>& leftLayer, std::vector<Neuron>& rightLayer, size_t rightBegin, size_t rightEnd) {
        static_assert(std::is_base_of<nn_activations::ActivationFunc, TActivFunc>::value, "assert: template isn't based of ActivationFunc");
        static TActivFunc activation;

        for (size_t r = 0; r < rightEnd - rightBegin; r++) {
            auto& rightNeuron = rightLayer[rightBegin + r];
            rightNeuron.value = 0.0f;
            for (size_t l = 0; l < leftLayer.size(); l++) {
                rightNeuron.value += leftLayer[l].value * rightNeuron.weights[l];
            }
            rightNeuron.value += rightNeuron.bias;
            rightNeuron.value = reinterpret_cast<nn_activations::ActivationFunc*>(&activation)->activate(rightNeuron.value);
        }
    }

    template<typename TLossFunc>
    inline float BaseNeuralNetworkComponent::getLossResult(const std::vector<NeuralValue>& answer) {
        static_assert(std::is_base_of<nn_losses::LossFunc, TLossFunc>::value, "assert: template isn't based of LossFunc");
        TLossFunc lossFunc;

        return static_cast<nn_losses::LossFunc>(lossFunc).loss(getOutput(), answer);
    }
}
