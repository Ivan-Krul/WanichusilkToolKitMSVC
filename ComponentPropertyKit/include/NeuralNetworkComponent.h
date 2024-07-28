#pragma once
#include <vector>
#include <thread>
#include <chrono>

#include "BaseNeuralNetworkComponent.h"

namespace comp_prop_kit_lib {
    typedef float NeuralValue;

    class NeuralNetworkComponent :
        public BaseNeuralNetworkComponent {
    public:
        inline NeuralNetworkComponent()
            : BaseNeuralNetworkComponent(typeid(NeuralNetworkComponent).hash_code()) {
        }

        // setup
        void setMaxThreadCount(size_t threads);

        // benchmark for singlethread and multithread
        template<typename TActivFunc> inline _NODISCARD float makeSinglThreadBenchmark() { return makeBenchmark<TActivFunc>(false); }
        template<typename TActivFunc> inline _NODISCARD float makeMultiThreadBenchmark() { return makeBenchmark<TActivFunc>(true); }

        // forward
        template<typename TActivFunc> void forwardSinglThreadSequentional();
        template<typename TActivFunc> void forwardMulithreadSequentional();

        // loss function
        template <typename T, typename TActivFunc, typename TLossFunc>_NODISCARD float getLossResults(const std::vector<std::vector<T>>& inputs, const std::vector<std::vector<NeuralValue>>& answers);

        // change weights
        template<typename TActivFunc, typename TLossFunc> void setGradientUnefficientlyOne(const std::vector<NeuralValue>& answer);
        template<typename TActivFunc, typename TLossFunc> void setGradientUnefficientlyMore(const std::vector<std::vector<NeuralValue>>& inputs,const std::vector<std::vector<NeuralValue>>& answers);
        template<typename TActivFunc, typename TLossFunc> void setGradientEfficientlyOne(const std::vector<NeuralValue>& answer);

    private:
        template<typename TActivFunc> float makeBenchmark(bool isMultithread);

        template<typename TActivFunc> void forwardMultiThread(const std::vector<Neuron>& leftLayer, std::vector<Neuron>& rightLayer, size_t threadNumber);

        size_t mThreadsDone = 0;
        size_t mThreadsMaxCount = std::thread::hardware_concurrency();
    };

    template<typename TActivFunc>
    inline void NeuralNetworkComponent::forwardSinglThreadSequentional() {
        for (size_t i = 0; i < mNeurons.size() - 1; i++) {
            forward<TActivFunc>(mNeurons[i], mNeurons[i + 1]);
        }
    }

    template<typename TActivFunc>
    inline void NeuralNetworkComponent::forwardMulithreadSequentional() {
        std::vector<std::thread> mThreads;
        mThreads.resize(mThreadsMaxCount);
        for (size_t i = 0; i < mNeurons.size() - 1; i++) {
            
            mThreadsDone = 0;
            for (size_t t = 0; t < mThreadsMaxCount; t++) {
                auto func = [=]() {
                    forwardMultiThread<TActivFunc>(mNeurons[i], mNeurons[i + 1], t);
                };
                mThreads[t] = std::thread(func);
                mThreads[t].detach();
            }

            while (mThreadsDone != mThreadsMaxCount);
        }
            
    }

    template<typename T, typename TActivFunc, typename TLossFunc>
    inline float NeuralNetworkComponent::getLossResults(const std::vector<std::vector<T>>& inputs, const std::vector<std::vector<NeuralValue>>& answers) {
        float averageLoss = 0.0f;

        for (size_t i = 0; i < std::min(inputs.size(), answers.size()); i++) {
            setInput(inputs[i]);
            forwardSinglThreadSequentional<TActivFunc>();
            averageLoss += getLossResult<TLossFunc>(answers[i]);
        }

        return averageLoss / std::min(inputs.size(), answers.size());
    }

    template<typename TActivFunc, typename TLossFunc>
    inline void NeuralNetworkComponent::setGradientUnefficientlyOne(const std::vector<NeuralValue>& answer) {
        const NeuralValue dh = 0.0001f;
        NeuralValue origLoss = getLossResult<TLossFunc>(answer);

        for (auto& layer : mNeurons) {
            for (auto& neuron : layer) {
                for (size_t w = 0; w < neuron.weights.size(); w++) {
                    neuron.weights[w] += dh;
                    forwardSinglThreadSequentional<TActivFunc>();
                    NeuralValue dLoss = getLossResult<TLossFunc>(answer) - origLoss;
                    neuron.weights[w] -= dh;
                    neuron.gradientW[w] = dLoss / dh;
                }

                neuron.bias += dh;
                forwardSinglThreadSequentional<TActivFunc>();
                NeuralValue dLoss = getLossResult<TLossFunc>(answer) - origLoss;
                neuron.bias -= dh;
                neuron.gradientB = dLoss / dh;
            }
        }
    }

    template<typename TActivFunc, typename TLossFunc>
    inline void NeuralNetworkComponent::setGradientUnefficientlyMore(const std::vector<std::vector<NeuralValue>>& inputs, const std::vector<std::vector<NeuralValue>>& answers) {
        for (size_t i = 0; i < std::min(inputs.size(), answers.size()); i++) {
            setInput(inputs[i]);
            setGradientUnefficientlyOne<TActivFunc, TLossFunc>(answers[i]);
        }
    }

    template<typename TActivFunc, typename TLossFunc>
    inline void NeuralNetworkComponent::setGradientEfficientlyOne(const std::vector<NeuralValue>& answer) {
        static_assert(std::is_base_of<nn_losses::LossFunc, TLossFunc>::value, "assert: template isn't based on LossFunc");
        static_assert(std::is_base_of<nn_activations::ActivationFunc, TActivFunc>::value, "assert: template isn't based on ActivationFunc");
        static TActivFunc activFunc;
        static TLossFunc lossFunc;
        
        nn_activations::ActivationFunc* activs = static_cast<nn_activations::ActivationFunc*>(&activFunc);

        forwardSinglThreadSequentional<TActivFunc>();
        auto loss = getLossResult<TLossFunc>(answer);

        // Backpropagation
        for (size_t l = mNeurons.size() - 1; l > 0; l--) {
            for (size_t n = 0; n < mNeurons[l].size(); n++) {
                Neuron& neuron = mNeurons[l][n];
                NeuralValue neuronGradient = 0.0;

                if (l == mNeurons.size() - 1) {
                    // Output layer
                    neuronGradient = mNeurons[l][n].value - answer[n];
                } else {
                    // Hidden layers
                    for (size_t n = 0; n < mNeurons[l + 1].size(); n++) {
                        neuronGradient += mNeurons[l + 1][n].gradientW[n] * mNeurons[l + 1][n].weights[n];
                    }
                    neuronGradient *= activs->derivative(neuron.value);
                }

                neuron.gradientB = neuronGradient;

                for (size_t p = 0; p < neuron.weights.size(); p++) {
                    neuron.gradientW[p] = neuronGradient * mNeurons[l - 1][p].value;
                }
            }
        }
    }

    template<typename TActivFunc>
    inline float NeuralNetworkComponent::makeBenchmark(bool isMultithread) {
        std::chrono::high_resolution_clock::time_point beg = std::chrono::high_resolution_clock::now();
        std::chrono::high_resolution_clock::time_point end;
        if(isMultithread) 
            forwardMulithreadSequentional<TActivFunc>();
        else
            forwardSinglThreadSequentional<TActivFunc>();

        end = std::chrono::high_resolution_clock::now();

        uint64_t cnt = std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count();
        float dur = cnt / 10e5; // milliseconds

        return 1000.f / dur;
    }

    template<typename TActivFunc>
    inline void NeuralNetworkComponent::forwardMultiThread(const std::vector<Neuron>& leftLayer, std::vector<Neuron>& rightLayer, size_t threadNumber) {
        size_t part = rightLayer.size() / mThreadsMaxCount;
        size_t sector = part * threadNumber;
        if(threadNumber + 1 == mThreadsMaxCount)
            part += rightLayer.size() % mThreadsMaxCount;

        forwardPart<TActivFunc>(leftLayer, rightLayer, sector, sector + part);
        mThreadsDone++;
    }
}

typedef comp_prop_kit_lib::NeuralNetworkComponent NeuNetComponent;