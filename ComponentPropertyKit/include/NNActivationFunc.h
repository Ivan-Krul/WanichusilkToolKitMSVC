#pragma once
#include <limits>

namespace comp_prop_kit_lib {
    namespace nn_activations {

        class ActivationFunc {
        public:
            virtual float activate(float x) {
                return x;
            }

            virtual float derivative(float x) {
                return 1;
            }
        };

        class StepActivFunc
            : public ActivationFunc {
        public:
            float activate(float x) override {
                return x > 0.f ? 1.f : 0.f;
            }

            float derivative(float x) override {
                return x == 0.f ? std::numeric_limits<float>::infinity() : 0.f;
            }
        };

        class LinearActivFunc
            : public ActivationFunc {
        public:
            float activate(float x) override {
                return x > 0.f ? 1.f : 0.f;
            }

            float derivative(float x) override {
                return 1.f;
            }
        };

        class SigmoidActivFunc
            : public ActivationFunc {
        public:
            float activate(float x) override {
                return 1.0f / (1.0f + exp(-x));
            }

            float derivative(float x) override {
                return x * (1.0f - x);
            }
        };

        class ReLUActivFunc
            : public ActivationFunc {
        public:
            float activate(float x) override {
                return x > 0.0f ? x : 0.0f;
            }

            float derivative(float x) override {
                return x > 0.0f ? 1.0f : 0.0f;
            }
        };

        class LeakyReLUActivFunc
            : public ActivationFunc {
        public:
            float activate(float x) override {
                return x > 0.0f ? x : 0.01f * x;
            }

            float derivative(float x) override {
                return x > 0.0f ? 1.0f : 0.01f;
            }
        };

        class Leaky2ReLUActivFunc
            : public ActivationFunc {
        public:
            float activate(float x) override {
                return x > 1.0f ? 1.0f + x * 0.01f : (x > 0.0f ? x : 0.01f * x);
            }

            float derivative(float x) override {
                return x > 1.0f ? 0.01f : (x > 0.0f ? 1.0f : 0.01f);
            }
        };

        class TanhActivFunc
            : public ActivationFunc {
        public:
            float activate(float x) override {
                return tanh(x);
            }

            float derivative(float x) override {
                return 1.0f - x * x;
            }
        };
    }
}
