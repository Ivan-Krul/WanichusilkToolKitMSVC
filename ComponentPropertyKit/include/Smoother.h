#pragma once
#include <math.h>

namespace comp_prop_kit_lib {
    namespace smoother {
        typedef double SmootherValue;

        class Smoother {
        public:
            virtual _NODISCARD SmootherValue fitValue(SmootherValue value, SmootherValue min, SmootherValue max) {
                return value;
            }
        };

        class SmootherLinear
            : public Smoother {
        public:
            _NODISCARD SmootherValue fitValue(SmootherValue value, SmootherValue min, SmootherValue max) override;
        };

        class SmootherSmoothLinear
            : public Smoother {
        public:
            _NODISCARD SmootherValue fitValue(SmootherValue value, SmootherValue min, SmootherValue max) override;
        };

        class SmootherStep
            : public Smoother {
        public:
            _NODISCARD SmootherValue fitValue(SmootherValue value, SmootherValue min, SmootherValue max) override;
        };

        class SmootherTwoSteps
            : public Smoother {
        public:
            _NODISCARD SmootherValue fitValue(SmootherValue value, SmootherValue min, SmootherValue max) override;
        };

        class SmootherParabola
            : public Smoother {
        public:
            _NODISCARD SmootherValue fitValue(SmootherValue value, SmootherValue min, SmootherValue max) override;
        };

        class SmootherInverseParabola
            : public Smoother {
        public:
            _NODISCARD SmootherValue fitValue(SmootherValue value, SmootherValue min, SmootherValue max) override;
        };
    }
}

