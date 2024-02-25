#pragma once
#include <string>
#include <vector>

namespace hardware_envi_lib {
	enum class DataType {
		str__,
		int__,
		uint_,
		bool_,
		char_,
		flot_,
		dobl_,
		shrt_,
		size_,

		arr__
	};

	struct float_var {
		template<typename T>
		typename std::enable_if<std::is_pointer<T>::value, void>::type init(const DataType data_type, const std::string& name, T ptr) {
			this->data_type = data_type;
			if (sizeOfVariable() != 0xff) {
				value.binary.resize(sizeOfVariable());
				for (uint8_t i = 0; i < value.binary.size(); i++)
					value.binary[i] = *((char*)ptr + i);
			} else
				value.binary = (const char*)ptr;
			this->name = name;
		}

		inline float_var() {
			name = "";
			data_type = DataType::str__;
		}
		inline float_var(const DataType data_type, const std::string name) {
			this->name = name;
			this->data_type = data_type;
		}
		inline float_var(const DataType data_type) {
			this->data_type = data_type;
			this->name = "";
			if (sizeOfVariable() != 0xff)
				value.binary.resize(sizeOfVariable());
		}
		//float_var(const DataType data_type, const std::string name, const char* p_ref);
		//float_var(const DataType data_type, const std::string name, const void* p_ref);
		template<typename T>
		float_var(const DataType data_type, const std::string& name, T ptr, typename std::enable_if<std::is_pointer<T>::value, void>::type* = nullptr);
		template<typename T>
		float_var(const DataType data_type, const std::string& name, T value, typename std::enable_if<!std::is_pointer<T>::value, void>::type* = nullptr);
		float_var(const std::string& name, size_t size);

		// to elements of array
		//float_var(const DataType data_type, const char* p_ref);
		//float_var(const DataType data_type, const void* p_ref);
		template<typename T>
		float_var(const DataType data_type, T ptr, typename std::enable_if<std::is_pointer<T>::value, void>::type* = nullptr);
		template<typename T>
		float_var(const DataType data_type, T value, typename std::enable_if<!std::is_pointer<T>::value, void>::type* = nullptr);
		float_var(size_t size);

		uint8_t sizeOfVariable();

		std::string name;
		DataType data_type;

		struct {
			std::string binary;
			std::vector<float_var> arr;
		} value;

		template <typename T>
		typename std::enable_if<!std::is_pointer<T>::value, T>::type convertToSavety() const;
		template <typename T>
		typename std::enable_if<!std::is_pointer<T>::value, T>::type convertTo() const;

		float_var& operator[](size_t index);
		float_var at(size_t index) const;

		inline size_t size() const noexcept {
			return value.arr.size();
		}

		inline std::string& operator()() {
			return value.binary;
		}

		inline std::string operator()() const {
			return value.binary;
		}

		template <typename T>
		typename std::enable_if<!std::is_pointer<T>::value, void>::type set(T val);

		void set(const std::string val);

		std::string getProperties() const;
		std::string getDataTypeProperty() const;
	};

	template<typename T>
	inline float_var::float_var(const DataType data_type, const std::string& name, T ptr, typename std::enable_if<std::is_pointer<T>::value, void>::type*) {
		init(data_type, name, ptr);
	}

	template<typename T>
	inline float_var::float_var(const DataType data_type, const std::string& name, T value, typename std::enable_if<!std::is_pointer<T>::value, void>::type*) {
		init(data_type, name, &value);
	}

	template<typename T>
	inline float_var::float_var(const DataType data_type, T value, typename std::enable_if<!std::is_pointer<T>::value, void>::type*) {
		init(data_type, "", &value);
	}

	template<typename T>
	inline float_var::float_var(const DataType data_type, T value, typename std::enable_if<std::is_pointer<T>::value, void>::type*) {
		init(data_type, "", value);
	}

	template<typename T>
	inline typename std::enable_if<!std::is_pointer<T>::value, T>::type float_var::convertToSavety() const {
		if (value.binary.size() != sizeof(T) || data_type == DataType::arr__) return T();
		else if (typeid(T) == typeid(bool) && data_type == DataType::bool_) return *reinterpret_cast<T*>(const_cast<char*>(value.binary.c_str()));
		else if (typeid(T) == typeid(char) && data_type == DataType::char_) return *reinterpret_cast<T*>(const_cast<char*>(value.binary.c_str()));
		else if (typeid(T) == typeid(short) && data_type == DataType::shrt_) return *reinterpret_cast<T*>(const_cast<char*>(value.binary.c_str()));
		else if (typeid(T) == typeid(int) && data_type == DataType::int__) return *reinterpret_cast<T*>(const_cast<char*>(value.binary.c_str()));
		else if (typeid(T) == typeid(unsigned) && data_type == DataType::uint_) return *reinterpret_cast<T*>(const_cast<char*>(value.binary.c_str()));
		else if (typeid(T) == typeid(float) && data_type == DataType::flot_) return *reinterpret_cast<T*>(const_cast<char*>(value.binary.c_str()));
		else if (typeid(T) == typeid(uint64_t) && data_type == DataType::size_) return *reinterpret_cast<T*>(const_cast<char*>(value.binary.c_str()));
		else if (typeid(T) == typeid(double) && data_type == DataType::dobl_) return *reinterpret_cast<T*>(const_cast<char*>(value.binary.c_str()));
		else return T();
	}

	template<typename T>
	inline typename std::enable_if<!std::is_pointer<T>::value, T>::type float_var::convertTo() const {
		if (value.binary.size() != sizeof(T) || data_type == DataType::arr__)
			return T();
		return *reinterpret_cast<T*>(const_cast<char*>(value.binary.c_str()));
	}

	template<typename T>
	inline typename std::enable_if<!std::is_pointer<T>::value, void>::type hardware_envi_lib::float_var::set(T val) {
		for (uint8_t i = 0; i < value.binary.size(); i++)
			value.binary[i] = *((char*)(&val) + i);
	}
}
