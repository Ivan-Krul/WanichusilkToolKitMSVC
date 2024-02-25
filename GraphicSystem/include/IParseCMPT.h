#pragma once
#include <string>
#include <iostream>

#include "Hardware.h"

namespace graphic_sys_lib {

	class ParseHardwareHelper {
	protected:
		hardware_envi_lib::Compacter& parseCmptFile(const std::string& pathToCMPT) {
			auto& hardware = hardware_envi_lib::Hardware::getInstance();
			if (!hardware.canBeFileOpen(pathToCMPT)) {
				std::cerr << "runtime: file " << pathToCMPT << " wasn't found\n";
				throw std::runtime_error("wasn't found");
			}

			hardware.read(pathToCMPT);

			return hardware.getCompacter();
		}

		using HWCompacter = hardware_envi_lib::Compacter;
		using HWDataType = hardware_envi_lib::DataType;

		static constexpr HWDataType cDTArr = HWDataType::arr__;
		static constexpr HWDataType cDTBool = HWDataType::bool_;
		static constexpr HWDataType cDTChar = HWDataType::char_;
		static constexpr HWDataType cDTDobl = HWDataType::dobl_;
		static constexpr HWDataType cDTFlot = HWDataType::flot_;
		static constexpr HWDataType cDTInt = HWDataType::int__;
		static constexpr HWDataType cDTShrt = HWDataType::shrt_;
		static constexpr HWDataType cDTSize = HWDataType::size_;
		static constexpr HWDataType cDTStr = HWDataType::str__;
		static constexpr HWDataType cDTUInt = HWDataType::uint_;
	};

	struct IParseCMPT {
		virtual void parseFile(const std::string& pathToCMPT) = 0;
		virtual _NODISCARD bool isParsableFile(const std::string& pathToCMPT) = 0;
	};

	struct IParseFloatVar {
		virtual void parseFloatVar(const hardware_envi_lib::float_var floatVar) = 0;
		virtual _NODISCARD bool isParsableFloatVar(const hardware_envi_lib::float_var floatVar) = 0;
	};
}
