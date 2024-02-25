#include "../include/FloatVar.h"

#include <stdexcept>
#include "FloatVar.h"

namespace hardware_envi_lib {
	//float_var::float_var(const DataType data_type, const std::string name, const char* p_ref) {
	//	this->data_type = data_type;
	//	if (sizeOfVariable() != 0xff) {
	//		value.binary.resize(sizeOfVariable());
	//		for (uint8_t i = 0; i < value.binary.size(); i++)
	//			value.binary[i] = *(p_ref + i);
	//	} else
	//		value.binary = p_ref;
	//	this->name = name;
	//}
	//
	//float_var::float_var(const DataType data_type, const std::string name, const void* p_ref) {
	//	this->data_type = data_type;
	//	
	//	value.binary.resize(sizeOfVariable());
	//	for (uint8_t i = 0; i < value.binary.size(); i++)
	//		value.binary[i] = *((char*)p_ref + i);
	//	
	//	this->name = name;
	//}

	float_var::float_var(const std::string& name, size_t size) {
		this->data_type = DataType::arr__;
		this->name = name;
		value.arr.resize(size);
	}

	//float_var::float_var(const DataType data_type, const char* p_ref)
	//	: float_var(data_type, "", p_ref) {
	//}
	//
	//float_var::float_var(const DataType data_type, const void* p_ref) 
	//: float_var(data_type, "", p_ref) {
	//}

	float_var::float_var(size_t size)
		: float_var("", size) {
	}

	uint8_t float_var::sizeOfVariable() {
		uint8_t bytes = 0;
		switch (data_type) {
		case DataType::dobl_:
		case DataType::size_:
			bytes += 4;
		case DataType::flot_:
		case DataType::uint_:
		case DataType::int__:
			bytes += 2;
		case DataType::shrt_:
			bytes += 1;
		case DataType::char_:
		case DataType::bool_:
			bytes += 1;
			return bytes;
		default:
			return 0xff;
		}
	}

	void float_var::set(const std::string val) {
		value.binary = val;
	}

	float_var& hardware_envi_lib::float_var::operator[](size_t index) {
		if (data_type != DataType::arr__)
			throw std::runtime_error("float_var isn't arr_ type");

		if (index >= value.arr.size())
			throw std::out_of_range("float_var is out of range");

		return value.arr[index];
	}

	float_var float_var::at(size_t index) const {
		if (data_type != DataType::arr__)
			throw std::runtime_error("float_var isn't arr_ type");

		if (index >= value.arr.size())
			throw std::out_of_range("float_var is out of range");

		return value.arr[index];
	}

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
		default:
			return "str_" + str + value.binary;
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
		default:
			return "str_";
		}
	}
}
