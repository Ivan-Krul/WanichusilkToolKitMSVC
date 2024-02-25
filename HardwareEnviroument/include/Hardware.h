// Target: Save and load from/to file and put all in Compactor, single object

#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "Compacter.h"

#define CONFIG_PATH "config.cmpt"

namespace hardware_envi_lib {
	struct datatype {
		char value[5];
		char binary_value;
		DataType data_type;
	};

	const std::vector<datatype> c_DataTypes =
	{
		{"str_", 's', DataType::str__},
		{"int_", 'i', DataType::int__},
		{"uint", 'u', DataType::uint_},
		{"char", 'c', DataType::char_},
		{"bool", 'b', DataType::bool_},
		{"flot", 'f', DataType::flot_},
		{"dobl", 'd', DataType::dobl_},
		{"shrt", 'r', DataType::shrt_},
		{"size", 'z', DataType::size_},
		{"arr_", 'a', DataType::arr__}
	};

	struct IHardware {
		virtual void write(const std::string directory) = 0;
		virtual void read(const std::string directory) = 0;
		virtual void writeAbortLine(const std::string where, const std::string message) = 0;
	};

	class Hardware : public IHardware {
	public:
		void write(const std::string directory);
		void read(const std::string directory);
		bool canBeFileOpen(const std::string directory);
		Compacter& getCompacter();
		void writeAbortLine(const std::string where, const std::string message);

		// Singleton pattern
		static Hardware& getInstance() {
			return s_Instance;
		}
	private:
		Compacter m_Compacter;

		void _writeInFileSize(std::ofstream& fs, size_t size);
		void _writeInFileString(std::ofstream& fs, const std::string str);
		void _writeInFileValue(std::ofstream& fs, const float_var& fv);
		datatype _findFromDataTypes(const DataType data_type);
		datatype _findFromDataTypes(const char bin);

		size_t _readFromFileSize(std::ifstream& fs);
		std::string _readFromFileString(std::ifstream& fs);
		void _readFromFileValue(std::ifstream& fs, float_var& fv);



		// Singleton pattern
		static Hardware s_Instance;
		Hardware() = default;
	};

	// Hardware Hardware::s_Instance;
}
