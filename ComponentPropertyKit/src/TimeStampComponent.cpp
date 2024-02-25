#include "TimeStampComponent.h"

namespace comp_prop_kit_lib {
    float TimeStampComponent::getRawDifference() const noexcept {
        return static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(mTimeEnd - mTimeBeg).count());
    }
}
