#include "FloatVarDisk.h"

namespace float_var_container_lib {
	FloatVarDisk::FloatVarDisk(const FloatVar& value) {
		mInstance = value;
	}

	FloatVarDisk::FloatVarDisk(FloatVar&& value) {
		mInstance = value;
		memcpy(value.data_type, gDefaultDataType, 5);
		value.binary.clear();
		value.list.clear();
		value.name.clear();
	}

	FloatVarDisk::FloatVarDisk(const FloatVarBaseExpander& value) {
		mInstance = *value.getFloatVar();
	}

	FloatVarDisk::FloatVarDisk(FloatVarBaseExpander&& value) {
		mInstance = *value.getFloatVar();
		value.clear();
	}

	FloatVarDisk::InteractStatus FloatVarDisk::readFile(const std::string& path, size_t start) {
		std::ifstream fin;
		InteractStatus status = InteractStatus::success;

		fin.open(path);

		if (!fin.is_open())
			return InteractStatus::not_found;

		if (fin.fail()) {
			status = InteractStatus::fail_stream;
			goto gotoreturn;
		}

		fin.seekg(start, std::ios::beg);
		if (fin.eof()) {
			status = InteractStatus::too_small;
			goto gotoreturn;
		}

		if ((status = skipSpaces(fin)) != InteractStatus::success)
			goto gotoreturn;

		try {
			auto instance = readWholeFloatVar(fin);
			mInstance = instance;
		} catch (InteractStatus st) {
			status = st;
			goto gotoreturn;
		}

	gotoreturn:
		fin.close();
		return status;
	}

	void FloatVarDisk::writeFile(const std::string& path, size_t start) {
		std::ofstream fout;
		InteractStatus status = InteractStatus::success;

		fout.open(path);

		fout.seekp(0, std::ios::end);
		size_t filesize = fout.tellp();

		if (start > filesize) {
			fout.seekp(0, std::ios::end);

			for (size_t i = 0; i < (start - filesize); i++)
				fout.write("\0", 1);
		} else
			fout.seekp(start, std::ios::beg);

		writeWholeFloatVar(fout, mInstance);

		fout.close();
	}

	size_t FloatVarDisk::getByteShifted() const noexcept {
		return mShifted;
	}

	FloatVar FloatVarDisk::readWholeFloatVar(std::istream& in) {
		bool end = false;
		char readBuffer;

		auto landmark = sLandmarkLookup.end();
		size_t bufferSize = 0;

		FloatVar value;

		while (!end) {
			if (in.eof())
				throw InteractStatus::too_small;

			readPartionBuffer(in, &readBuffer, 1);

			landmark = sLandmarkLookup.find(readBuffer);

			if (landmark == sLandmarkLookup.end())
				throw InteractStatus::undefined_landmark;

			switch (landmark->second) {
			case FloatVarDisk::FileLandmark::begin:
				try {
					value.list.push_back(readWholeFloatVar(in));
				} catch (InteractStatus st) {
					throw st;
				}
				break;

			case FloatVarDisk::FileLandmark::binary:
				readPartionBuffer(in, (char*)&bufferSize, cLengthBytes);
				value.binary.resize(bufferSize);
				readPartionBuffer(in, (char*)value.binary.data(), bufferSize);
				break;

			case FloatVarDisk::FileLandmark::data_type:
				readPartionBuffer(in, value.data_type, 4);
				break;

			case FloatVarDisk::FileLandmark::name:
				readPartionBuffer(in, (char*)&bufferSize, cLengthBytes);
				value.name.resize(bufferSize);
				readPartionBuffer(in, (char*)value.name.data(), bufferSize);
				break;

			case FloatVarDisk::FileLandmark::end:
				end = true;
				break;

			case FloatVarDisk::FileLandmark::space:
			default:
				break;
			}
		}

		return value;
	}

	void FloatVarDisk::writeWholeFloatVar(std::ostream& out, const FloatVar& value) {
		writePartionBuffer(out, &sCharLookup.find(FloatVarDisk::FileLandmark::begin)->second, 1);

		size_t buf;

		if (!value.binary.empty()) {
			writePartionBuffer(out, &sCharLookup.find(FloatVarDisk::FileLandmark::binary)->second, 1);
			buf = value.binary.size();
			writePartionBuffer(out, (char*)&buf, cLengthBytes);
			writePartionBuffer(out, value.binary.data(), value.binary.size());
		}

		if (!value.name.empty()) {
			writePartionBuffer(out, &sCharLookup.find(FloatVarDisk::FileLandmark::name)->second, 1);
			buf = value.name.size();
			writePartionBuffer(out, (char*)&buf, cLengthBytes);
			writePartionBuffer(out, value.name.data(), value.name.size());
		}

		if (*(const int*)value.data_type != *(const int*)gDefaultDataType) {
			writePartionBuffer(out, &sCharLookup.find(FloatVarDisk::FileLandmark::data_type)->second, 1);
			writePartionBuffer(out, value.data_type, 4);
		}

		for (auto& element : value.list) {
			writeWholeFloatVar(out, element);
		}

		writePartionBuffer(out, &sCharLookup.find(FloatVarDisk::FileLandmark::end)->second, 1);
	}

	void FloatVarDisk::writePartionBuffer(std::ostream& out, const char* buffer, size_t size) {
		out.write(buffer, size);
		mShifted += size;
	}

	FloatVarDisk::InteractStatus FloatVarDisk::skipSpaces(std::istream& in) {
		bool start = false;
		char readBuffer;

		while (!start) {
			if (in.eof())
				return InteractStatus::too_small;

			readPartionBuffer(in, &readBuffer, 1);

			if (sLandmarkLookup.at(readBuffer) == FileLandmark::begin)
				start = true;
		}
		return InteractStatus::success;
	}

	bool FloatVarDisk::isClassAffordable() {
		return true;
	}

	void FloatVarDisk::readPartionBuffer(std::istream& in, char* buffer, size_t size) {
		in.read(buffer, size);
		mShifted += size;
	}

	const decltype(FloatVarDisk::sLandmarkLookup) FloatVarDisk::sLandmarkLookup = {
		{'\0', FloatVarDisk::FileLandmark::space},
		{'b',  FloatVarDisk::FileLandmark::begin},
		{'n',  FloatVarDisk::FileLandmark::name},
		{'d',  FloatVarDisk::FileLandmark::data_type},
		{'i',  FloatVarDisk::FileLandmark::binary},
		{'e',  FloatVarDisk::FileLandmark::end}
	};

	const decltype(FloatVarDisk::sCharLookup) FloatVarDisk::sCharLookup = {
		{FloatVarDisk::FileLandmark::space    , '\0'},
		{FloatVarDisk::FileLandmark::begin    , 'b'	},
		{FloatVarDisk::FileLandmark::name     , 'n'	},
		{FloatVarDisk::FileLandmark::data_type, 'd'	},
		{FloatVarDisk::FileLandmark::binary   , 'i'	},
		{FloatVarDisk::FileLandmark::end      , 'e'	}
	};

	const decltype(FloatVarDisk::cLengthBytes) FloatVarDisk::cLengthBytes = 2;
}
