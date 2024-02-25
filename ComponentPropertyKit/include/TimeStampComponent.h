#pragma once
#include "Component.h"

#include <chrono>

namespace comp_prop_kit_lib {
    class TimeStampComponent
    : public Component{
    public:
        TimeStampComponent(PropertyList& parent, ComponentID id, ComponentTypeHash infoHash)
            : Component(parent, id, infoHash) {
        }

        inline void init() override {
            mTimeBeg = mTimeEnd = std::chrono::high_resolution_clock::now();
        }

        inline void stampBegin() noexcept {
            mTimeBeg = std::chrono::high_resolution_clock::now();
        }

        inline void stampEnd() noexcept {
            mTimeEnd = std::chrono::high_resolution_clock::now();
        }

        inline std::chrono::high_resolution_clock::time_point& getBegClock() noexcept {
            return mTimeBeg;
        }

        inline std::chrono::high_resolution_clock::time_point& getEndClock() noexcept {
            return mTimeEnd;
        }

        template<typename T>
        inline typename std::enable_if<std::is_same<T, std::chrono::nanoseconds>::type,float>::value getDifference() const noexcept {
            return getRawDifference();
        }
        template<typename T>
        inline typename std::enable_if<std::is_same<T, std::chrono::microseconds>::type, float>::value getDifference() const noexcept {
            return getRawDifference() / 1e3f;
        }
        template<typename T>
        inline typename std::enable_if<std::is_same<T, std::chrono::milliseconds>::type, float>::value getDifference() const noexcept {
            return getRawDifference() / 1e6f;
        }
        template<typename T>
        inline typename std::enable_if<std::is_same<T, std::chrono::seconds>::type, float>::value getDifference() const noexcept {
            return getRawDifference() / 1e9f;
        }
        template<typename T>
        inline typename std::enable_if<std::is_same<T, std::chrono::minutes>::type, float>::value getDifference() const noexcept {
            return getRawDifference() / 60e9f;
        }
        template<typename T>
        inline typename std::enable_if<std::is_same<T, std::chrono::hours>::type, float>::value getDifference() const noexcept {
            return getRawDifference() / 3600e9f;
        }

    private:
        float getRawDifference() const noexcept;

        std::chrono::high_resolution_clock::time_point mTimeBeg;
        std::chrono::high_resolution_clock::time_point mTimeEnd;
    };
}

