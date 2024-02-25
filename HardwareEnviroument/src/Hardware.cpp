#include "../include/Hardware.h"

namespace hardware_envi_lib {
	void Hardware::_writeInFileSize(std::ofstream& fs, size_t size) {
		fs.write(reinterpret_cast<char*>(&size), sizeof(short));
	}

	void Hardware::_writeInFileString(std::ofstream& fs, const std::string str) {
		auto size = str.size();
		fs.write(reinterpret_cast<char*>(&size), sizeof(short));
		for (const auto& s : str)
			fs.write(&s, 1);
	}

	size_t Hardware::_readFromFileSize(std::ifstream& fs) {
		auto size = size_t();

		fs.read(reinterpret_cast<char*>(&size), sizeof(short));

		return size;
	}

	std::string Hardware::_readFromFileString(std::ifstream& fs) {
		auto size = size_t();
		auto str = std::string();

		fs.read(reinterpret_cast<char*>(&size), sizeof(short));
		str.resize(size);

		for (auto& c : str)
			fs.read(&c, 1);

		return str;
	}

	void Hardware::_readFromFileValue(std::ifstream& fs, float_var& fv) {
		auto bufnum = size_t();
		fs.read(reinterpret_cast<char*>(&bufnum), 1);
		fv.data_type = _findFromDataTypes(static_cast<char>(bufnum)).data_type;

		if (fv.data_type != DataType::arr__) {
			fv.value.binary = _readFromFileString(fs);
		} else {
			fv.value.arr.resize(_readFromFileSize(fs));
			for (auto& el : fv.value.arr) {
				_readFromFileValue(fs, el);
			}
		}
	}

	void Hardware::_writeInFileValue(std::ofstream& fs, const float_var& fv) {
		auto bufnum = _findFromDataTypes(fv.data_type).binary_value;
		fs.write(&bufnum, 1);
		if (fv.data_type != DataType::arr__) {
			_writeInFileString(fs, fv.value.binary);
		} else {
			_writeInFileSize(fs, fv.value.arr.size());
			for (auto& elem : fv.value.arr) {
				_writeInFileValue(fs, elem);
			}
		}
	}

	datatype Hardware::_findFromDataTypes(const DataType data_type) {
		auto comp = [=](datatype datatp) { return datatp.data_type == data_type;  };
		return *std::find_if(c_DataTypes.begin(), c_DataTypes.end(), comp);
	}

	datatype Hardware::_findFromDataTypes(const char bin) {
		auto comp = [=](datatype datatp) { return datatp.binary_value == bin; };
		return *std::find_if(c_DataTypes.begin(), c_DataTypes.end(), comp);
	}

	/*
	Definition:
		&Str(string)
		{
			string.size() > s
			->
				string[s]
			<-
		}
	Structure:
		Compacter.size() > c
		->
			c_DataTypes[Compacter[c].data_type].binary_value
			&Str(Compacter[c].name)
			if c_DataTypes[Compacter[c].data_type].binary_value != DataType::arr -> |
				&Str(Compacter[c].value.binary)
			else -> |
				Compacter[c].value.arr.size() > a
				->
					c_DataTypes[Compacter[c].value.arr[a]].binary_value
					&Str(Compacter[c].value.binary)
				<-

		<-
	*/

	void hardware_envi_lib::Hardware::write(const std::string directory) {
		auto writer = std::ofstream(directory, std::ios::binary | std::ios::out);

		_writeInFileSize(writer, m_Compacter.size());

		for (const auto& c : m_Compacter) {
			_writeInFileString(writer, c.name);
			_writeInFileValue(writer, c);
		}
		writer.close();
	}

	void hardware_envi_lib::Hardware::read(const std::string directory) {
		if (!canBeFileOpen(directory))
			return;
		auto reader = std::ifstream(directory, std::ios::binary | std::ios::in);
		auto size = _readFromFileSize(reader);
		auto fv = float_var();

		m_Compacter.clear();

		for (size_t c = 0; c < size; c++) {
			fv.name = _readFromFileString(reader);

			_readFromFileValue(reader, fv);

			m_Compacter.push(fv);
		}
		reader.close();
	}

	bool Hardware::canBeFileOpen(const std::string directory) {
		auto reader = std::ifstream(directory);
		auto open = reader.is_open();

		reader.close();

		return open;
	}
	Compacter& Hardware::getCompacter() {
		return m_Compacter;
	}

	void Hardware::writeAbortLine(const std::string where, const std::string message) {
		auto writer = std::ofstream(where + "_abort_log.log");

		writer.seekp(0, std::ios_base::end);
		writer << "ERROR: " << where << ": " << message << '\n';
		writer.close();
	}

	Hardware Hardware::s_Instance;
}
