// Target: Save variables in different datatypes in one and convert in needly with indexation

#pragma once
#include <string>
#include <list>
#include <type_traits>

#include "FloatVar.h"

namespace hardware_envi_lib {
	class Compacter {
	public:
		void push(const DataType data_type, const std::string& name, const char* p_ref);
		void push(const DataType data_type, const std::string& name, const void* p_ref);
		template<typename T>
		typename std::enable_if<!std::is_pointer<T>::value, void>::type push(const DataType data_type, const std::string& name, T value);
		void push(const std::string& name, size_t size);
		void push(const float_var fl_var);

		size_t size() const noexcept;

		std::list<float_var>::const_iterator cbegin() const noexcept;
		std::list<float_var>::const_iterator cend() const noexcept;

		std::list<float_var>::iterator begin() noexcept;
		std::list<float_var>::iterator end() noexcept;

		std::list<float_var>::const_reverse_iterator crbegin() const noexcept;
		std::list<float_var>::const_reverse_iterator crend() const noexcept;

		std::list<float_var>::reverse_iterator rbegin() noexcept;
		std::list<float_var>::reverse_iterator rend() noexcept;

		void clear();

		const std::string nameOfVariable(size_t index) const;
		size_t indexOfVariable(const std::string& str_index) const;

		float_var& operator[](const std::string& str_index);
		float_var& operator[](const size_t index);
		const float_var at(const std::string& str_index) const;
		const float_var at(const size_t index) const;

		const DataType getDataType(const std::string& str_index) const;

		bool empty() const;

		void erase(const std::string& str_index);
		void erase(const size_t index);
		void erase(std::list<float_var>::const_iterator iter);
	private:
		std::list<float_var> m_Variables;

		std::list<float_var>::const_iterator _cgetFromIndex(const size_t index) const;
		std::list<float_var>::iterator _getFromIndex(const size_t index);
		std::list<float_var>::const_iterator _cgetVar(const std::string str_index) const;
		std::list<float_var>::iterator _getVar(const std::string str_index);
	};

	template<typename T>
	inline typename std::enable_if<!std::is_pointer<T>::value, void>::type Compacter::push(const DataType data_type, const std::string& name, T value) {
		push(data_type, name, (const char*)&value);
	}
}
