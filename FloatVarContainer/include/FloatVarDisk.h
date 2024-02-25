#pragma once
#include "FloatVarBaseExpander.h"

#include <fstream>
#include <unordered_map>

namespace float_var_container_lib {
    class FloatVarDisk :
        public FloatVarBaseExpander {
    public:
        FloatVarDisk() = default;
        FloatVarDisk(const FloatVar& value);
        FloatVarDisk(FloatVar&& value);
        FloatVarDisk(const FloatVarBaseExpander& value);
        FloatVarDisk(FloatVarBaseExpander&& value);
        virtual ~FloatVarDisk() {}


        enum class InteractStatus {
            success = 0,
            not_found,
            fail_stream,
            undefined_landmark,
            too_small
        };

        enum class FileLandmark {
            space = 0,
            begin,
            name,
            data_type,
            binary,
            end
        };


        static const uint8_t cLengthBytes;
        static const std::unordered_map<char, FileLandmark> sLandmarkLookup;
        static const std::unordered_map<FileLandmark, char> sCharLookup;


        InteractStatus readFile(const std::string& path, size_t start = 0);
        void writeFile(const std::string& path, size_t start = 0);

        size_t getByteShifted() const noexcept;


        bool isClassAffordable() override;

    protected:
        InteractStatus skipSpaces(std::istream& in);

        FloatVar readWholeFloatVar(std::istream& in);
        void readPartionBuffer(std::istream& in, char* buffer, size_t size);

        void writeWholeFloatVar(std::ostream& out, const FloatVar& value);
        void writePartionBuffer(std::ostream& out, const char* buffer, size_t size);

        size_t mShifted;

    };
}

