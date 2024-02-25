#pragma once
#include <iostream>
#include <conio.h>
#include <queue>
#include <array>
#include <sstream>

#include "Hardware.h"

#include "Keywords.h"

#include "Utils.h"

static auto& gHardware = hardware_envi_lib::Hardware::getInstance();
static auto& gCompacter = gHardware.getCompacter();
static bool gNeedExit = false;

void openFile();
void saveFile();
void createVariable();
void outputVariable();
void writeVariable();
void listFile();
void clearScreen();
void deleteVariable();
void mergeFile();
void divideIntoFiles();
void clear();
void help();
void handleExit();
void handleNone();

inline void chooseAction(Keyword keyword) {
    switch (keyword) {
    case Keyword::OPEN:
        openFile();
        break;
    case Keyword::SAVE:
        saveFile();
        break;
    case Keyword::CREATE:
        createVariable();
        break;
    case Keyword::OUTPUT:
        outputVariable();
        break;    
    case Keyword::WRITE:
        writeVariable();
        break;
    case Keyword::CLEAR:
        clear();
        break;
    case Keyword::DELETE:
        deleteVariable();
        break;
    case Keyword::MERGE:
        mergeFile();
        break;
    case Keyword::DIVIDE:
        divideIntoFiles();
        break;
    case Keyword::HELP:
        help();
        break;
    case Keyword::LIST:
        listFile();
        break;
    case Keyword::CLEAR_SCREEN:
        clearScreen();
        break;
    case Keyword::EXIT:
        handleExit();
        break;
    default:
        handleNone();
        break;
    }
}

inline void openFile() {
    std::string input;

    clear();

    if (!gCompacter.empty())
        return;

#ifdef _WIN32
    system("dir");
#elif __unix__ || __linux__
    system("ls -l");
#endif // _WIN32

    std::cout << "Type a filepath you prefer to open (type '$' to cancel): ";
    std::getline(std::cin >> std::ws, input);

    if (input.find('$') != std::string::npos) {
        std::cout << "return: canceled\n";
        return;
    }

    if (!gHardware.canBeFileOpen(input)) {
        std::cout << "return: not found\n";
        return;
    }

    try {
        gHardware.read(input);
    } catch (std::exception& e) {
        std::cout << "except: " << e.what() << '\n';
        return;
    }

    std::cout << "return: ok\n";
}

inline void saveFile() {
    std::string input;

#ifdef _WIN32
    system("dir");
#elif __unix__ || __linux__
    system("ls -l");
#endif // _WIN32

    std::cout << "Type a filepath you prefer to save (type '" << gBannedFilepathChar << "' to cancel): ";
    std::getline(std::cin >> std::ws, input);

    if (input.find(gBannedFilepathChar) != std::string::npos) {
        std::cout << "return: canceled\n";
        return;
    }

    if (gHardware.canBeFileOpen(input)) {
        std::string agreement;
        std::cout << "Another filepath are exist too. Overwrite? (type \"" << gAgreement << "\" to accept): ";
        std::cin >> agreement;
        if (agreement != gAgreement) {
            std::cout << "return: canceled\n";
            return;
        }
    }

    try {
        gHardware.write(input);
    } catch (std::exception& e) {
        std::cout << "except: " << e.what() << '\n';
        return;
    }

    std::cout << "return: ok\n";
}

inline hardware_envi_lib::float_var& getThroughArray(const std::string& input) {
    auto variableName = input.substr(0, input.find('['));
    std::queue<size_t> elements;
    try {
        elements = parseScopes(input.substr(variableName.size()));
    } catch (...) {
        throw false;
    }

    try {
        auto& fv = enterToArray(elements.size(), elements, gCompacter[variableName]);
        return fv;
    } catch (...) {
        throw false;
    }
}

inline hardware_envi_lib::float_var& getVariable(std::string phrase) {
    std::string input;

    std::cout << "What variable you want " << phrase << " (type '" << gBannedCompacterName << "' to cancel, type name of existing variable and number under [] to mark what variable should " << phrase << " in array): ";
    std::getline(std::cin >> std::ws, input);

    if (input.find(gBannedCompacterName) != std::string::npos) {
        std::cout << "return: canceled\n";
        throw false;
    }

    if (input.find('[') != std::string::npos) {
        try {
            return getThroughArray(input);
        } catch (...) {
            throw false;
        }
    }

    try {
        return gCompacter[input];
    } catch (std::exception& e) {
        std::cout << "except: " << e.what() << '\n';
        throw false;
    }
}

inline void inputValueOfCreatedVariable(const std::string& variableName, hardware_envi_lib::DataType dataType, hardware_envi_lib::float_var& element) {
    uint64_t varBuffer = 0;
    std::string input;

    switch (dataType) {
    case hardware_envi_lib::DataType::arr__: inputArray(variableName, element); return;
    case hardware_envi_lib::DataType::bool_:
        std::cout << "Type a value of the variable (true, false): ";
        std::cin >> input;
        varBuffer = input == "true"; break;
    case hardware_envi_lib::DataType::char_:
        std::cout << "Do you want to input by number? (type \"" << gAgreement << "\" to accept): ";
        std::cin >> input;
        std::cout << "Type a value of the variable(char or (-128 - 127)): ";
        if (input == gAgreement)
            std::cin >> varBuffer;
        else {
            std::cin >> input;
            varBuffer = input[0];
        } break;
    case hardware_envi_lib::DataType::shrt_:
        std::cout << "Type a value of the variable(-32'768 - 32'767): ";
        std::cin >> varBuffer; break;
    case hardware_envi_lib::DataType::int__:
        std::cout << "Type a value of the variable(-2'147'483'648 - 2'147'483'647): ";
        std::cin >> varBuffer; break;
    case hardware_envi_lib::DataType::uint_:
        std::cout << "Type a value of the variable(0 - 4'294'967'296): ";
        std::cin >> varBuffer; break;
    case hardware_envi_lib::DataType::flot_:
        std::cout << "Type a value of the variable(float point): ";
        std::cin >> *(float*)(&varBuffer); break;
    case hardware_envi_lib::DataType::size_:
        std::cout << "Type a value of the variable(0 - 18'446'744'073'709'551'616): ";
        std::cin >> varBuffer; break;
    case hardware_envi_lib::DataType::dobl_:
        std::cout << "Type a value of the variable(float point): ";
        std::cin >> *(double*)(&varBuffer); break;
    case hardware_envi_lib::DataType::str__:
        std::cout << "Type a value of the variable(string): ";
        std::getline(std::cin >> std::ws, input);
        element = hardware_envi_lib::FloatVarFactory::createStringVariable(variableName, input.c_str());
        std::cout << "return: ok\n"; return;
    }

    element = hardware_envi_lib::FloatVarFactory::createNumericVariable(dataType, variableName, &varBuffer);

    std::cout << "return: ok\n";
    return;
}

inline void finaliseVariableCreation(const std::string& variableName, hardware_envi_lib::float_var& element) {
    std::string inputDataType;
    std::string input;

    std::cout << "Type a data type: ";

    std::getline(std::cin >> std::ws, inputDataType);

    if (inputDataType.find(gArrayMarker) != inputDataType.npos) {
        try {
            auto arr = parseArray(inputDataType);
            createArrayFromScopes(arr, variableName, element);
        } catch (std::exception& exc) {
            std::cout << "except: " << exc.what() << '\n';
            return;
        }
        std::cout << "return: ok\n";
        return;
    }

    const auto cond = [=](const hardware_envi_lib::datatype& dt) { return dt.value == inputDataType; };
    const auto iter = std::find_if(hardware_envi_lib::c_DataTypes.begin(), hardware_envi_lib::c_DataTypes.end(), cond);
    if (iter == hardware_envi_lib::c_DataTypes.end()) {
        std::cout << "except: not existing data type like \"" << inputDataType << "\" try to type \"help\" to get all data types what aviable\n";
        return;
    }

    inputValueOfCreatedVariable(variableName, iter->data_type, element);
}

inline void createVariable() {
    std::string input;

    std::cout << "What name of a new variable"
        << "(type '" << gBannedCompacterName << "' to cancel, type name of existing variable and number under [] to mark what new variable should create in array): ";
    std::getline(std::cin >> std::ws, input);

    if (input.find(gBannedCompacterName) != std::string::npos) {
        std::cout << "return: canceled\n";
        return;
    }

    std::string variableName = input.substr(0, input.find('['));

    const bool isExist = gCompacter.indexOfVariable(variableName) != SIZE_MAX;
    const bool isVarInArr = input.find('[') != std::string::npos && isExist;

    if (isExist && !isVarInArr) {
        std::cout << "return: exist\n";
        return;
    }
    if (!isExist && input.find('[') == std::string::npos) {
        gCompacter.push_back(hardware_envi_lib::float_var());
        finaliseVariableCreation(variableName, gCompacter[""]);
        return;
    }
    if (isExist && isVarInArr) {
        try {
            auto& fv = getThroughArray(input);
            finaliseVariableCreation("", fv);
        } catch (...) {
            return;
        }
        return;
    }

    std::cout << "except: none of these conditions weren't triggered\n";
}

inline void writeVariable() {
    
    try {
        auto& fv = getVariable("write");
        inputValueOfCreatedVariable(fv.name, fv.data_type, fv);
    } catch (...) {
        return;
    }
}

inline void outputVariable() {
    try {
        auto& fv = getVariable("output");
        outputFloatVar(fv);
    } catch (...) {
        return;
    }
}

inline void listFile() {
    std::cout << "variables: " << gCompacter.size() << '\n';

    for (auto fv : gCompacter) {
        std::cout << '\t' << fv.getDataTypeProperty() << " \"" << fv.name << "\"\n";
    }

    std::cout << "return: ok\n";
}

inline void clearScreen() {
#ifdef _WIN32
    system("cls");
#elif __unix__ || __linux__
    system("clear");
#endif // _WIN32
}

inline void deleteSingleVariable(bool isInRoot, hardware_envi_lib::float_var& fl_v) {
    std::string agreement;
    std::cout << "There are some data in compacter. Exit anyway? (type \"" << gAgreement << "\" to accept): ";
    std::cin >> agreement;
    if (agreement != gAgreement) {
        std::cout << "return: canceled\n";
        return;
    }

    if (isInRoot)
        gCompacter.erase(fl_v.name);
    else
        fl_v = hardware_envi_lib::float_var();
}

inline void deleteVariable() {
    std::string input;

    std::cout << "What variable you want to delete (type '" << gBannedCompacterName << "' to cancel, type name of existing variable and number under [] to mark what new variable should create in array): ";
    std::getline(std::cin >> std::ws, input);

    if (input.find(gBannedCompacterName) != std::string::npos) {
        std::cout << "return: canceled\n";
        return;
    }

    hardware_envi_lib::float_var fv;


    if (input.find('[') != std::string::npos) {
        try {
            auto& fv = getThroughArray(input);
            deleteSingleVariable(false, fv);
            return;
        } catch (...) {
            return;
        }

    }

    try {
        fv = gCompacter[input];
        deleteSingleVariable(true, fv);
    } catch (std::exception& e) {
        std::cout << "except: " << e.what() << '\n';
        return;
    }

    std::cout << "return: ok\n";
}

inline void mergeFile() {
    std::string input;

#ifdef _WIN32
    system("dir");
#elif __unix__ || __linux__
    system("ls -l");
#endif // _WIN32

    std::cout << "Type a filepath you prefer to open (type '$' to cancel): ";
    std::getline(std::cin >> std::ws, input);

    if (input.find('$') != std::string::npos) {
        std::cout << "return: canceled\n";
        return;
    }

    if (!gHardware.canBeFileOpen(input)) {
        std::cout << "return: not found\n";
        return;
    }

    auto compCopy = gCompacter;
    gCompacter.clear();
    try {
        gHardware.read(input);
    } catch (std::exception& e) {
        std::cout << "except: " << e.what() << '\n';
        return;
    }

    std::swap(compCopy, gCompacter);

    try {
        auto& fv = getVariable("merge");
        std::string name = fv.name;
        fv = convertCompacterToFloatVar(compCopy);
        fv.name = name;
    } catch (...) {
        return;
    }
    std::cout << "return: ok\n";
}

inline void divideIntoFiles() {
    auto fv = hardware_envi_lib::float_var();
    try {
        auto& fv2 = getVariable("divide");
        fv = fv2;
        if (fv2.name != "")
            gCompacter.erase(fv2.name);
        else
            fv2 = hardware_envi_lib::float_var();
    } catch (...) {
        return;
    }

    auto compCopy = convertFloatVarToCompacter(fv);

    std::swap(compCopy, gCompacter);
    
    std::string input;

#ifdef _WIN32
    system("dir");
#elif __unix__ || __linux__
    system("ls -l");
#endif // _WIN32

    std::cout << "Type a filepath you prefer to save (type '" << gBannedFilepathChar << "' to cancel): ";
    std::getline(std::cin >> std::ws, input);

    if (input.find(gBannedFilepathChar) != std::string::npos) {
        std::cout << "return: canceled\n";
        std::swap(compCopy, gCompacter);
        return;
    }

    if (gHardware.canBeFileOpen(input)) {
        std::string agreement;
        std::cout << "Another filepath are exist too. Overwrite? (type \"" << gAgreement << "\" to accept): ";
        std::cin >> agreement;
        if (agreement != gAgreement) {
            std::cout << "return: canceled\n";
            std::swap(compCopy, gCompacter);
            return;
        }
    }

    try {
        gHardware.write(input);
    } catch (std::exception& e) {
        std::cout << "except: " << e.what() << '\n';
        std::swap(compCopy, gCompacter);
        return;
    }

    std::cout << "return: ok\n";

    std::swap(compCopy, gCompacter);
    
}

inline void clear() {
    if (!gCompacter.empty()) {
        std::string agreement;
        std::cout << "There are some data in compacter. Clear anyway? (type \"" << gAgreement << "\" to accept): ";
        std::cin >> agreement;
        if (agreement != gAgreement) {
            std::cout << "return: canceled\n";
            return;
        }
    }

    gCompacter.clear();

    std::cout << "return: ok\n";
}

inline void help() {
    std::array<std::vector<std::string>, (int)Keyword::last - (int)Keyword::first + 1> keywords;
    
    for (auto& keyword : gKeywords) {
        if ((int)keyword.second < (int)Keyword::first)
            continue;
        keywords[(int)keyword.second - (int)Keyword::first].push_back(keyword.first);
    }

    std::cout << "\t--- Help ---\n";
    std::cout << "Welcome to the help section!\n\n";
    std::cout << "There you can manipulate with \"compacters\", or files what their extension name is .cmpt\n";
    std::cout << "It's a cli interface, so, no GUI here\n";
    std::cout << "Commands also have aliases or synonyms\n";
    std::cout << '\n';
    std::cout << "\t\tCommands\n";
    
    std::cout <<"["<< helpWithAssembleAliases(keywords, Keyword::EXIT) << "]\t- exit from the program. It asks for erasing data from the session\n\n";
    std::cout <<"["<< helpWithAssembleAliases(keywords, Keyword::OPEN) << "]\t- open file and load to the workspace. The file extension should be .cmpt, but it's not necessary. It asks for erasing data from the session\n\n";
    std::cout <<"["<< helpWithAssembleAliases(keywords, Keyword::SAVE) << "]\t- save file from workspace. The file extension should be .cmpt, but it's not necessary. It asks for overwriting file\n\n";
    std::cout <<"["<< helpWithAssembleAliases(keywords, Keyword::CREATE) << "]\t- create a variable in workspace with named variable, or location in existing variable if you want to create a new variable in array\n\n";
    std::cout << "[" << helpWithAssembleAliases(keywords, Keyword::OUTPUT) << "]\t- output a variable and it's content in workspace with named variable, or location in existing variable if you want to create a new variable in array\n\n";
    std::cout << "[" << helpWithAssembleAliases(keywords, Keyword::WRITE) << "]\t- rewrite a variable's content in workspace with named variable, or location in existing variable if you want to create a new variable in array\n\n";
    std::cout << "[" << helpWithAssembleAliases(keywords, Keyword::DELETE) << "]\t- delete a variable in workspace with named variable, or location in existing variable if you want to create a new variable in array\n\n";
    std::cout << "[" << helpWithAssembleAliases(keywords, Keyword::LIST) << "]\t- output all variables what contain in workspace\n\n";
    std::cout << "[" << helpWithAssembleAliases(keywords, Keyword::CLEAR) << "]\t- clear workspace completely. It asks for erasing data from the session\n\n";
    std::cout << "[" << helpWithAssembleAliases(keywords, Keyword::CLEAR_SCREEN) << "]\t- clear terminal output\n\n";
    std::cout << "[" << helpWithAssembleAliases(keywords, Keyword::HELP) << "]\t- showind all instruments and their description(like as you can see)\n\n";

    std::cout << '\n';
    std::cout << "There also a variable\n";
    std::cout << '\n';
    std::cout << "\t\tVariable Types\n";

    std::cout << "bool - variable keeps statement only in true or false. It has a capacity of 1 byte\n";
    std::cout << "char - variable keeps symbol. It has a capacity of 1 byte. It also can be writen as number\n";
    std::cout << "shrt - variable keeps signed integer. It has a capacity of 2 bytes\n";
    std::cout << "int_ - variable keeps signed integer. It has a capacity of 4 bytes\n";
    std::cout << "uint - variable keeps unsigned integer. It has a capacity of 4 bytes\n";
    std::cout << "flot - variable keeps number with a float point. It has a capacity of 4 bytes\n";
    std::cout << "size - variable keeps unsigned integer. It has a capacity of 8 bytes\n";
    std::cout << "dobl - variable keeps number with a float point. It has a capacity of 8 bytes\n";
    std::cout << "str_ - variable keeps string. It has a limit of size up to 2^16 symbols\n";
    std::cout << "arr_ - variable keeps array of other variables, even itself. It has a limit of size up to 2^16 elements if it isn't containing another arrays."
        << " Also you can create arrays using @{var_type, var_type, ...}, and also write variables like @{value, value, ...}\n";
    std::cout << '\n';
}

inline void handleExit() {
    if (!gCompacter.empty()) {
        std::string agreement;
        std::cout << "There are some data in compacter. Exit anyway? (type \"" << gAgreement << "\" to accept): ";
        std::cin >> agreement;
        if (agreement != gAgreement) {
            std::cout << "return: canceled\n";
            return;
        }
    }

    gNeedExit = true;
    std::cout << "return: ok\n";
}

inline void handleNone() {
    std::cout << "except: invalid command, try to type \"help\" to get all commands\n";
}
