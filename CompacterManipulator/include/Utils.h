#pragma once
#include <iostream>
#include <conio.h>
#include <queue>
#include <array>
#include <sstream>
#include <functional>

#include "Hardware.h"

#include "Keywords.h"

inline _NODISCARD size_t count(const std::string& input, char ch) {
    size_t count = 0;
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == ch)
            count++;
    }
    return count;
}

inline _NODISCARD std::string shrinkContent(const std::string& input, int(*cond)(int)) {
    size_t firstChar = SIZE_MAX;
    size_t lastChar = 0;

    std::string content;

    for (size_t i = 0; i < input.size(); i++) {
        if (!cond(input[i])) {
            if (firstChar == SIZE_MAX)
                firstChar = i;
            lastChar = i;
        }
    }

    return input.substr(firstChar, lastChar - firstChar + 1);
}

inline _NODISCARD std::vector<std::string> parseArray(const std::string& input) {
    // !{element, element, element}
    // 
    // example: @{int, flot, str_} -> create an array with marked data type
    // 
    // or: @{3, 5, hello} -> write an array with marked value,
    // also it parse data type str_ until comma, and space isn't counting
    // so, if you want a full str_, then type it manually

    std::vector<std::string> arr;
    size_t exclamation = input.find(gArrayMarker);
    std::string buffer;

    if (exclamation == input.npos) goto except;
    buffer = input.substr(exclamation);

    exclamation = buffer.find_first_of('{');
    if (exclamation == buffer.npos) goto except;
    buffer = buffer.substr(exclamation + 1);

    exclamation = buffer.find_last_of('}');
    if (exclamation == input.npos) goto except;
    buffer = buffer.substr(0, exclamation);

    // we have this -> "..., ..., ..."

    arr.resize(count(buffer, ',') + 1);
    for (size_t i = 0; i < arr.size(); i++) {
        if (i + 1 != arr.size()) {
            exclamation = buffer.find(',');
            arr[i] = buffer.substr(0, exclamation);
            buffer.erase(0, arr[i].size() + 1);
            arr[i] = shrinkContent(arr[i], isspace);
        } else
            arr[i] = shrinkContent(buffer, isspace);
    }

    return arr;

except:
    std::cout << "except: scope aren't closed, try to make this sequence like: !{element, element, ...}\n";
    std::cout << buffer << '\n';
    throw false;
}


inline void createArrayFromScopes(const std::vector<std::string>& arr, const std::string& name, hardware_envi_lib::float_var& element) {
    std::string datatype;

    const auto cond = [&datatype](const hardware_envi_lib::datatype& dt) { return dt.value == datatype; };
    auto iter = hardware_envi_lib::c_DataTypes.begin();

    element = hardware_envi_lib::FloatVarFactory::createArrayVariable(name, arr.size());

    for (size_t i = 0; i < arr.size(); i++) {
        datatype = arr[i];
        iter = std::find_if(hardware_envi_lib::c_DataTypes.begin(), hardware_envi_lib::c_DataTypes.end(), cond);
        if (iter == hardware_envi_lib::c_DataTypes.end()) {
            std::cout << "except: not existing data type like \"" << datatype << "\" try to type \"help\" to get all data types what aviable\n";
            throw false;
        }

        if (iter->data_type == hardware_envi_lib::DataType::arr__)
            element[i] = hardware_envi_lib::FloatVarFactory::createArrayVariable("", 0);
        else
            element[i] = hardware_envi_lib::FloatVarFactory::createNumericVariable(iter->data_type, "", "");
    }
}

template<typename T>
inline typename std::enable_if<std::is_integral<T>::value&& std::is_unsigned<T>::value>::type inputArrayValues(const std::string& str, hardware_envi_lib::float_var& element) {
    std::cout << "log: unsigned: \"" << str << "\"\n";
    std::stringstream sstream;
    static uint64_t varBuffer;
    sstream << str;
    sstream >> *(T*)(&varBuffer);
    element = hardware_envi_lib::FloatVarFactory::createNumericVariable(element.data_type, element.name, &varBuffer);
}

template<typename T>
inline typename std::enable_if<std::is_integral<T>::value && (!std::is_unsigned<T>::value) && (!std::is_same<T, char>::value)>::type inputArrayValues(const std::string& str, hardware_envi_lib::float_var& element) {

    std::cout << "log: signed: \"" << str << "\"\n";
    std::stringstream sstream;
    static int64_t varBuffer;
    sstream << str;
    sstream >> *(T*)(&varBuffer);
    element = hardware_envi_lib::FloatVarFactory::createNumericVariable(element.data_type, element.name, &varBuffer);
}

template<typename T>
inline typename std::enable_if<!std::is_integral<T>::value>::type inputArrayValues(const std::string& str, hardware_envi_lib::float_var& element) {
    std::cout << "log: flot: \"" << str << "\"\n";
    std::stringstream sstream;
    static double varBuffer;
    sstream << str;
    sstream >> *(T*)(&varBuffer);
    element = hardware_envi_lib::FloatVarFactory::createNumericVariable(element.data_type, element.name, &varBuffer);
}

template<typename T>
inline typename std::enable_if<std::is_same<T, char>::value>::type inputArrayValues(const std::string& str, hardware_envi_lib::float_var& element) {
    std::cout << "log: char: \"" << str << "\"\n";
    std::stringstream sstream;
    static char varBuffer;
    sstream << str;
    if (str.size() != 1)
        sstream >> *(short*)(&varBuffer);
    else {
        sstream >> varBuffer;
    }
    element = hardware_envi_lib::FloatVarFactory::createNumericVariable(element.data_type, element.name, &varBuffer);
}

inline void inputArray(const std::string& variableName, hardware_envi_lib::float_var& element) {
    uint64_t varBuffer = 0;
    std::string input;
    std::stringstream sstream;

    std::cout << "Type a choice to fill the array(type \"arr\" to input array like values): ";
    std::cin >> input;
    if (input == "arr") {
        std::cout << "type a list of values(@{value, value, ...}): ";
        std::getline(std::cin >> std::ws, input);
        try {
            auto arr = parseArray(input);
            if (element.data_type != hardware_envi_lib::DataType::arr__) {
                std::cout << "except: the element isn't array\n";
                return;
            }

            for (size_t i = 0; i < arr.size(); i++) {
                if (element[i].data_type == hardware_envi_lib::DataType::str__)
                    element[i]() = arr[i];
                else {
                    if (element[i].data_type == hardware_envi_lib::DataType::dobl_)
                        inputArrayValues<double>(arr[i], element[i]);
                    else if (element[i].data_type == hardware_envi_lib::DataType::flot_)
                        inputArrayValues<float>(arr[i], element[i]);
                    else if (element[i].data_type == hardware_envi_lib::DataType::uint_ || element[i].data_type == hardware_envi_lib::DataType::size_)
                        inputArrayValues<uint64_t>(arr[i], element[i]);
                    else if (element[i].data_type == hardware_envi_lib::DataType::char_)
                        inputArrayValues<char>(arr[i], element[i]);
                    else
                        inputArrayValues<int64_t>(arr[i], element[i]);
                }
            }
        } catch (...) {
            return;
        }
    } else {
        std::cout << "Type a size of the array(number): ";
        std::cin >> varBuffer;
        element = hardware_envi_lib::FloatVarFactory::createArrayVariable(variableName, varBuffer);
    }

    std::cout << "return: ok\n";
}


inline hardware_envi_lib::float_var& enterToArray(const size_t start_queue_size, std::queue<size_t>& queue, hardware_envi_lib::float_var& fl_v) {
    size_t index = queue.front();
    queue.pop();

    if (queue.empty())
        return fl_v[index];

    if (fl_v.data_type != hardware_envi_lib::DataType::arr__) {
        std::cout << "except: element in scope #" << start_queue_size - queue.size() << " isn't array\n";
        throw false;
    }
    if (index >= fl_v.size()) {
        std::cout << "except: index is out of range in scope #" << start_queue_size - queue.size() << '\n';
        throw false;
    }

    try {
        return enterToArray(start_queue_size, queue, fl_v[index]);
    } catch (...) {
        throw false;
    }
}

inline std::queue<size_t> parseScopes(const std::string& input) {
    std::queue<size_t> elements;
    bool isInScope = false;
    size_t back = 0;

    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '[') {
            isInScope = true;
            back = 0;
        }
        while (input[++i] != ']' && isInScope) {
            if (i + 1 == input.size()) {
                std::cout << "except: scope aren't closed, try to make this sequence like: existingVariable[number][number]...\n";
                throw false;
            }

            if (input[i - 1] == '[')
                back = input[i] - '0';
            else
                back = back * 10 + (input[i] - '0');

            if (input[i + 1] == ']')
                elements.push(back);
        }
        isInScope = false;
    }

    return elements;
}


inline void outputSingleFloatVar(const hardware_envi_lib::float_var& fv) {
    switch (fv.data_type) {
    case hardware_envi_lib::DataType::str__:
        std::cout << '"' << fv() << '"';
        break;
    case hardware_envi_lib::DataType::bool_:
        std::cout << (fv.convertTo<bool>() ? "true" : "false");
        break;
    case hardware_envi_lib::DataType::char_:
        std::cout << '\'' << fv.convertTo<char>() << "\' (mean " << (int)fv.convertTo<char>() << ")";
        break;
    case hardware_envi_lib::DataType::shrt_:
        std::cout << fv.convertTo<short>();
        break;
    case hardware_envi_lib::DataType::int__:
        std::cout << fv.convertTo<int>();
        break;
    case hardware_envi_lib::DataType::uint_:
        std::cout << fv.convertTo<unsigned>();
        break;
    case hardware_envi_lib::DataType::flot_:
        std::cout << fv.convertTo<float>() << 'f';
        break;
    case hardware_envi_lib::DataType::size_:
        std::cout << fv.convertTo<uint64_t>();
        break;
    case hardware_envi_lib::DataType::dobl_:
        std::cout << fv.convertTo<double>() << 'f';
        break;
    }
}

inline void outputFloatVar(const hardware_envi_lib::float_var& fv, bool isContinue = false) {
    static size_t levelOfRecursion = 0;

    if (!isContinue)
        levelOfRecursion = 1;
    else
        levelOfRecursion++;

    //for (size_t i = 0; i < levelOfRecursion; i++) {
    //    std::cout << '\t';
    //}

    std::cout << '\t' << fv.getDataTypeProperty() << ' ';

    if (fv.name.empty())
        std::cout << "[] = ";
    else
        std::cout << "\"" << fv.name << "\" = ";

    if (fv.data_type == hardware_envi_lib::DataType::arr__) {
        std::cout << "size: " << fv.size() << "\n";
        for (size_t i = 0; i < levelOfRecursion; i++) {
            std::cout << '\t';
        }
        std::cout << "{\n";

        for (size_t i = 0; i < fv.size(); i++) {
            for (size_t i = 0; i < levelOfRecursion; i++) {
                std::cout << '\t';
            }
            std::cout << "\t[" << i << "]: ";
            outputFloatVar(fv.at(i), true);
            if(i + 1 != fv.size())
            std::cout << ",\n";
        }

        std::cout << "\n";
        for (size_t i = 0; i < levelOfRecursion; i++) {
            std::cout << '\t';
        }
        std::cout << "}";
    } else
        outputSingleFloatVar(fv);

    levelOfRecursion--;
}

inline std::string helpWithAssembleAliases(std::array<std::vector<std::string>, (int)Keyword::last - (int)Keyword::first + 1> keywords, Keyword key) {
    std::string res;
    int shift = (int)key - (int)Keyword::first;

    for (size_t i = 0; i < keywords[shift].size(); i++) {
        auto& el = keywords[shift][i];
        res += el + (el == keywords[shift].back() ? "" : ", ");
    }

    return res;
}

inline hardware_envi_lib::float_var convertCompacterToFloatVar(const hardware_envi_lib::Compacter& compacter) {
    hardware_envi_lib::float_var fl_v = hardware_envi_lib::FloatVarFactory::createArrayVariable("", compacter.size());

    for (size_t i = 0; i < fl_v.size(); i++) {
        fl_v[i] = compacter.at(i);
        fl_v[i].name = "";
    }

    return fl_v;
}

inline hardware_envi_lib::Compacter convertFloatVarToCompacter(hardware_envi_lib::float_var& floatVar) {
    hardware_envi_lib::Compacter compacter;
    
    for (size_t i = 0; i < floatVar.size(); i++) {
        compacter.push_back(floatVar[i]);
        compacter.back().name = std::to_string(i);
    }

    return compacter;
}

