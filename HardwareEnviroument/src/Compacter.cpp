#include "../include/Compacter.h"

namespace hardware_envi_lib {
	std::list<float_var>::const_iterator Compacter::_cgetFromIndex(const size_t index) const {
		size_t i = 0;
		for (auto iter = cbegin(); iter != cend(); iter++) {
			if (i == index)
				return iter;
			i++;
		}
		return std::list<float_var>::const_iterator();
	}
	std::list<float_var>::iterator Compacter::_getFromIndex(const size_t index) {
		size_t i = 0;
		for (auto iter = begin(); iter != end(); iter++) {
			if (i == index)
				return iter;
			i++;
		}
		return std::list<float_var>::iterator();
	}
	std::list<float_var>::const_iterator Compacter::_cgetVar(const std::string str_index) const {
		auto comp = [=](const float_var& fv) {return fv.name == str_index; };
		return std::find_if(m_Variables.cbegin(), m_Variables.cend(), comp);
	}
	std::list<float_var>::iterator Compacter::_getVar(const std::string str_index) {
		auto comp = [=](const float_var& fv) {return fv.name == str_index; };
		return std::find_if(m_Variables.begin(), m_Variables.end(), comp);
	}

	bool Compacter::empty() const {
		return m_Variables.empty();
	}

	void hardware_envi_lib::Compacter::push(const DataType data_type, const std::string& name, const char* p_ref) {
		auto fl_var = float_var(data_type, name, p_ref);
		m_Variables.push_back(fl_var);
	}

	void Compacter::push(const DataType data_type, const std::string& name, const void* p_ref) {
		auto fl_var = float_var(data_type, name, p_ref);
		m_Variables.push_back(fl_var);
	}

	void Compacter::push(const std::string& name, size_t size) {
		auto fl_var = float_var(name, size);
		m_Variables.push_back(fl_var);
	}

	void Compacter::push(const float_var fl_var) {
		m_Variables.push_back(fl_var);
	}

	size_t Compacter::size() const noexcept {
		return m_Variables.size();
	}

	std::list<float_var>::const_iterator Compacter::cbegin() const noexcept {
		return m_Variables.cbegin();
	}

	std::list<float_var>::const_iterator Compacter::cend() const noexcept {
		return m_Variables.cend();
	}

	std::list<float_var>::iterator Compacter::begin() noexcept {
		return m_Variables.begin();
	}

	std::list<float_var>::iterator Compacter::end() noexcept {
		return m_Variables.end();
	}

	std::list<float_var>::const_reverse_iterator Compacter::crbegin() const noexcept {
		return m_Variables.crbegin();
	}

	std::list<float_var>::const_reverse_iterator Compacter::crend() const noexcept {
		return m_Variables.crend();
	}

	std::list<float_var>::reverse_iterator Compacter::rbegin() noexcept {
		return m_Variables.rbegin();
	}

	std::list<float_var>::reverse_iterator Compacter::rend() noexcept {
		return m_Variables.rend();
	}

	void Compacter::clear() {
		m_Variables.clear();
	}

	const std::string Compacter::nameOfVariable(size_t index) const {
		for (const auto& v : m_Variables) {
			if (index == 0)
				return v.name;
			index--;
		}
		return std::string();
	}

	size_t Compacter::indexOfVariable(const std::string& str_index) const {

		size_t i = 0;
		for (auto iter = cbegin(); iter != cend(); iter++) {
			if (iter->name == str_index)
				return i;
			i++;
		}
		return SIZE_MAX;
	}

	float_var& Compacter::operator[](const std::string& str_index) {
		return *_getVar(str_index);
	}

	float_var& Compacter::operator[](const size_t index) {
		return *_getFromIndex(index);
	}

	const float_var Compacter::at(const std::string& str_index) const {
		return *_cgetVar(str_index);
	}

	const float_var Compacter::at(const size_t index) const {
		return *_cgetFromIndex(index);
	}

	const DataType Compacter::getDataType(const std::string& str_index) const {
		return _cgetVar(str_index)->data_type;
	}

	void Compacter::erase(const std::string& str_index) {
		auto comp = [=](const float_var& fv) { return fv.name == str_index; };
		auto iter = std::find_if(m_Variables.begin(), m_Variables.end(), comp);
		m_Variables.erase(iter);
	}

	void Compacter::erase(const size_t index) {
		m_Variables.erase(_getFromIndex(index));
	}

	void Compacter::erase(std::list<float_var>::const_iterator iter) {
		m_Variables.erase(iter);
	}
}
