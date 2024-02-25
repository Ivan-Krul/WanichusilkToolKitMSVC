#pragma once
#include <vector>

namespace comp_prop_kit_lib {
    namespace nn_losses {

        class LossFunc {
        public:
            virtual float loss(const std::vector<float>& result, const std::vector<float>& answer) noexcept {
                float loss = 0;
                for (size_t i = 0; i < std::min(result.size(), answer.size()); i++) {
                    loss += abs(result[i] - answer[i]);
                }
                return loss;
            }

            virtual float derivative(float var) noexcept {
                return 1;
            }
        };

        class ParabolaLossFunc
            : public LossFunc {
        public:
            float loss(const std::vector<float>& result, const std::vector<float>& answer) noexcept override {
                float loss = 0;
                for (size_t i = 0; i < std::min(result.size(), answer.size()); i++) {
                    loss += result[i] * result[i] - answer[i] * answer[i];
                }
                return loss;
            }
        };
    }
}
