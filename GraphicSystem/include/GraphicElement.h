#pragma once
#include <string>

#include "raylib.h"

#include "Hardware.h"
#include "ComponentPropertyKit.h"

#include "IParseCMPT.h"

namespace graphic_sys_lib {
    class GraphicElement
        : protected IParseCMPT, protected IParseFloatVar, public ParseHardwareHelper {
    public:
        GraphicElement();
        virtual void render() const;

        virtual void parseFile(const std::string& pathToCMPT);
        virtual _NODISCARD bool isParsableFile(const std::string& pathToCMPT);
        virtual void parseFloatVar(const hardware_envi_lib::float_var floatVar);
        virtual _NODISCARD bool isParsableFloatVar(const hardware_envi_lib::float_var floatVar);
        

        inline _NODISCARD comp_prop_kit_lib::PropertyList& getProperties() noexcept {
            return mProperties;
        }

        inline _NODISCARD comp_prop_kit_lib::PropertyList cgetProperties() const noexcept {
            return mProperties;
        }

        ~GraphicElement() = default;

    protected:
        comp_prop_kit_lib::PropertyList mProperties;
        hardware_envi_lib::Compacter mConfig;
    };
}

