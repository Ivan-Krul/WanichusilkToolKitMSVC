#include "FloatVar.h"

namespace hardware_envi_lib {
	std::string float_var::getProperties() const {
		auto str = " " + name + ": ";

		switch (data_type) {
		case DataType::dobl_:
			return "dobl" + str + std::to_string(convertTo<double>());
		case DataType::size_:
			return "size" + str + std::to_string(convertTo<uint64_t>());
		case DataType::flot_:
			return "flot" + str + std::to_string(convertTo<float>());
		case DataType::uint_:
			return "uint" + str + std::to_string(convertTo<unsigned>());
		case DataType::int__:
			return "int_" + str + std::to_string(convertTo<int>());
		case DataType::shrt_:
			return "shrt" + str + std::to_string(convertTo<short>());
		case DataType::char_:
			return "char" + str + convertTo<char>();
		case DataType::bool_:
			return "bool" + str + std::to_string(convertTo<bool>());
		case DataType::arr__:
			return "arr_" + str + "[ " + std::to_string(value.arr.size()) + " ]";
		case DataType::str__:
			return "str_" + str + value.binary;
		default:
			return "what" + str + value.binary;
		}
	}
	std::string float_var::getDataTypeProperty() const {
		switch (data_type) {
		case DataType::dobl_:
			return "dobl";
		case DataType::size_:
			return "size";
		case DataType::flot_:
			return "flot";
		case DataType::uint_:
			return "uint";
		case DataType::int__:
			return "int_";
		case DataType::shrt_:
			return "shrt";
		case DataType::char_:
			return "char";
		case DataType::bool_:
			return "bool";
		case DataType::arr__:
			return "arr_";
		case DataType::str__:
			return "str_";
		default:
			return "void";
		}
	}
}
