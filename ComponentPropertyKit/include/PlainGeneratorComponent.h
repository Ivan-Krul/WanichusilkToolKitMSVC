#pragma once
#include "Component.h"
#include "RandomGeneratorComponent.h"
#include "Smoother.h"

#include <memory>

namespace comp_prop_kit_lib {
    typedef double PlainValue;

    class PlainGeneratorComponent
        : public Component {
    public:
        struct Point {
            int x;
            int y;
            PlainValue value;
        };

        inline PlainGeneratorComponent()
            : Component(typeid(PlainGeneratorComponent).hash_code()), mRandomizer() {
            mSavedPoints = {};
            mSeed = (intptr_t)time(NULL);
        }

        inline void init() override {
            mSavedPoints = {};
            mRandomizer.init();
            mSeed = (intptr_t)time(NULL);
            mNeedSave = false;
        }

        inline void setSeed(intptr_t seed) {
            mSeed = seed;
            mRandomizer.setSeed(mSeed);
            mSavedPoints.clear();
        }

        void setSavingChunk(bool needSaving);

        template<typename T>
        _NODISCARD PlainValue generateSimplex(PlainValue X, PlainValue Y);

        template<typename T>
        _NODISCARD std::vector<PlainValue> generateSimplexChunk(PlainValue X, PlainValue Y, PlainValue shift, size_t width, size_t height);

        template<typename T>
        _NODISCARD std::vector<PlainValue> generatePerlinChunk(PlainValue startShift, PlainValue mulitplier, size_t iterations, size_t width, size_t height);

        inline _NODISCARD const std::vector<Point> getSavedPoints() const noexcept {
            return mSavedPoints;
        }

    private:
        PlainValue _NODISCARD findPoint(int globalX, int globalY);

        template<typename T>
        PlainValue _NODISCARD smooth(PlainValue X, PlainValue Y, PlainValue C1, PlainValue C2, PlainValue C3, PlainValue C4);

        RandomGeneratorComponent mRandomizer;
        std::vector<Point> mSavedPoints;
        intptr_t mSeed;

        bool mNeedSave;
    };

    template<class T>
    inline PlainValue PlainGeneratorComponent::generateSimplex(PlainValue X, PlainValue Y) {
        // finding out where are we in plain
        int globalX = floor(X);
        int globalY = floor(Y);

        X -= globalX;
        Y -= globalY;

        // finding out does point are generated already
        std::array<PlainValue, 4> corners = {
            findPoint(globalX, globalY),
            findPoint(globalX + 1, globalY),
            findPoint(globalX, globalY + 1),
            findPoint(globalX + 1, globalY + 1) };

        // smothing all values what we got using Bilinear interpolation
        return smooth<T>(X, Y, corners[0], corners[1], corners[2], corners[3]);
    }
    template<typename T>
    inline std::vector<PlainValue> PlainGeneratorComponent::generateSimplexChunk(PlainValue X, PlainValue Y, PlainValue shift, size_t width, size_t height) {
        // reserve for future array of numbers
        std::vector<PlainValue> dob;
        dob.reserve(width * height);

        // finding out where're we
        int globalX = floor(X);
        int globalY = floor(Y);

        X -= globalX;
        Y -= globalY;

        // creating a buffer of all corners
        std::vector<std::vector<PlainValue>> points;
        size_t usedPlainX = (int)ceil(shift * width) - globalX + 1;
        size_t usedPlainY = (int)ceil(shift * height) - globalY + 1;

        points.resize(usedPlainY);

        if (mNeedSave) {
            for (int y = 0; y < usedPlainY; y++) {
                points[y] = mRandomizer.nextRange(usedPlainX, 0.0, 1.0);
                for (int x = 0; x < usedPlainX; x++) {
                    mSavedPoints.push_back(Point{ x + globalX,y + globalY, points[y][x] });
                }
            }
        } else {
            for (int y = 0; y < usedPlainY; y++) {
                points[y] = mRandomizer.nextRange(usedPlainX, 0.0, 1.0);
            }
        }

        // iteration through corners
        for (size_t h = 0; h < height; h++) {
            for (size_t w = 0; w < width; w++) {
                PlainValue curX = X + (w * shift);
                PlainValue curY = Y + (h * shift);

                globalX = floor(curX);
                globalY = floor(curY);

                dob.push_back(
                    smooth<T>(
                        curX - globalX,
                        curY - globalY,
                        points[globalY][globalX],
                        points[globalY][globalX + 1],
                        points[globalY + 1][globalX],
                        points[globalY + 1][globalX + 1]));
            }
        }

        return dob;
    }

    template<typename T>
    inline std::vector<PlainValue> PlainGeneratorComponent::generatePerlinChunk(PlainValue startShift, PlainValue mulitplier, size_t iterations, size_t width, size_t height) {
        std::vector<PlainValue> res;
        res.resize(width * height);

        std::vector<PlainValue> plain;
        PlainValue min = std::numeric_limits<PlainValue>::max();
        PlainValue max = std::numeric_limits<PlainValue>::min();

        for (size_t i = 0; i < iterations; i++) {
            plain = generateSimplexChunk<T>(0, 0, startShift * pow(mulitplier, i), width, height);

            if (i + 1 == iterations) {
                for (size_t p = 0; p < plain.size(); p++) {
                    res[p] += plain[p] / pow(mulitplier, i);

                    min = std::min(min, res[p]);
                    max = std::max(max, res[p]);
                }
            } else {
                for (size_t p = 0; p < plain.size(); p++) {
                    res[p] += plain[p] / pow(mulitplier, i);
                }
            }
        }

        PlainValue max_min = max - min;
        for (auto& el : res) {
            el = (el - min) / max_min;
        }

        return res;
    }

    template<typename T>
    inline PlainValue comp_prop_kit_lib::PlainGeneratorComponent::smooth(PlainValue X, PlainValue Y, PlainValue C1, PlainValue C2, PlainValue C3, PlainValue C4) {
        // check for valid typename
        static_assert(std::is_base_of<smoother::Smoother, T>::value, "Template don't have a parent Smoother");
        static T smoother;

        return smoother.fitValue(Y, smoother.fitValue(X, C1, C2), smoother.fitValue(X, C3, C4));
    }
}

typedef comp_prop_kit_lib::PlainGeneratorComponent PlainGenComponent;
