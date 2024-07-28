#pragma once
#include "PropertyList.h"
#include <time.h>
#include <math.h>
#include <vector>

namespace comp_prop_kit_lib {
    typedef intptr_t signedsize_t;

    class RandomGeneratorComponent :
        public Component {
    public:

        inline RandomGeneratorComponent()
            : Component(typeid(RandomGeneratorComponent).hash_code()) {
            mValue = 0;
            mShifter = 0;
            mMultiplier = 0;
            mAdder = 0;
        }

        inline void init() override {
            mValue = (signedsize_t)time(NULL);
            mShifter = 6;
            mMultiplier = 53242359711;
            mAdder = 15835721;
        }

        inline void setOwnValues(signedsize_t value, signedsize_t multiplier, signedsize_t adder, signedsize_t shifter) noexcept {
            mValue = value;
            mMultiplier = multiplier;
            mAdder = adder;
            mShifter = shifter;
        }

        inline void setSeed(signedsize_t seed) noexcept {
            mValue = seed;
        }

        template<typename T>
        inline _NODISCARD typename std::enable_if<std::is_integral<T>::value, T>::type next() noexcept {
            mValue = ((mValue * mMultiplier) << mShifter) + mAdder;
            return mValue;
        }

        template<typename T>
        inline _NODISCARD typename std::enable_if<!std::is_integral<T>::value, T>::type next() noexcept {
            mValue = ((mValue * mMultiplier)) + mAdder;
            return static_cast<T>(mValue);
        }

        template<typename T>
        inline _NODISCARD T next(T to) noexcept {
            return chop(next<T>(), to);
        }

        template<typename T>
        inline _NODISCARD T next(T from, T to) noexcept {
            return chop(next<T>(), from, to);
        }

        template <typename T>
        inline _NODISCARD std::vector<T> nextRange(size_t size) noexcept {
            static std::vector<T> range;
            range.resize(size);

            for (auto& element : range)
                element = next<T>();

            return range;
        }

        template <typename T>
        inline _NODISCARD std::vector<T> nextRange(size_t size, T to) noexcept {
            static std::vector<T> range;
            range.resize(size);

            for (auto& element : range)
                element = next(to);

            return range;
        }

        template <typename T>
        inline _NODISCARD std::vector<T> nextRange(size_t size, T from, T to) noexcept {
            static std::vector<T> range;
            range.resize(size);

            for (auto& element : range)
                element = next(from, to);

            return range;
        }

        template <typename T>
        _NODISCARD typename std::enable_if<std::is_integral<T>::value, T>::type nextPrime() noexcept;
        template <typename T>
        _NODISCARD typename std::enable_if<std::is_integral<T>::value, T>::type nextPrime(T to) noexcept;
        template <typename T>
        _NODISCARD typename std::enable_if<std::is_integral<T>::value, T>::type nextPrime(T from, T to) noexcept;

    private:
        template <typename T>
        inline _NODISCARD T chop(T val, T to) {
            return fmod(val, to);
        }

        template <typename T>
        inline _NODISCARD T chop(T val, T from, T to) {
            static T min_val = static_cast<T>(std::numeric_limits<signedsize_t>::min());
            static T max_val = static_cast<T>(std::numeric_limits<signedsize_t>::max());
            T value = ((val - min_val) / (max_val - min_val));
            T res = value * (to - from) + from;
            return res;
        }

        signedsize_t mShifter;
        signedsize_t mMultiplier;
        signedsize_t mAdder;
        signedsize_t mValue;
    };

    template<typename T>
    inline typename std::enable_if<std::is_integral<T>::value, T>::type RandomGeneratorComponent::nextPrime() noexcept {
        auto res = next<T>();
        auto score = 0;
        while (score == res) {
            score = 0;
            for (int i = 2; i < res; i++) {
                if (res % i != 0)
                    score++;
            }
            if (score != res)
                res++;

            if(res < 2)
                res = next<T>();
        }

        return res;
    }

    template<typename T>
    inline typename std::enable_if<std::is_integral<T>::value, T>::type RandomGeneratorComponent::nextPrime(T to) noexcept {
        auto res = next(to);
        auto score = 0;
        while (score == res) {
            score = 0;
            for (int i = 2; i < res; i++) {
                if (res % i != 0)
                    score++;
            }
            if (score != res)
                res = chop(++res, to);
            if(res < 2)
                res = next(to);
        }
        return res;
    }

    template<typename T>
    inline typename std::enable_if<std::is_integral<T>::value, T>::type RandomGeneratorComponent::nextPrime(T from, T to) noexcept {
        auto res = next(from, to);
        auto score = 0;
        while (score == res) {
            score = 0;
            for (int i = 2; i < res; i++) {
                if (res % i != 0)
                    score++;
            }
            if (score != res)
                res = chop(++res, to, from);

            if (res < 2)
                res = next(from, to);
        }

        return res;
    }
}

typedef comp_prop_kit_lib::RandomGeneratorComponent RandComponent;
