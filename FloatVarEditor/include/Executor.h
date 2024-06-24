#pragma once

#include "FloatVar.h"
#include "ChainValue.h"
#include "LogWriter.h"
#include "ChainContainerTarget.h"

#include <bitset>

class Executor : public LogWriter {
    using FloatVar       = float_var_container_lib::FloatVar;

    enum PropertyBit {
        null = 0,
        set_container = 1,
        set_binary = 2,
        set_name = 4,
        set_datatype = 8,
        set_list_size = 16,
        set_list_element = 32,

        get_container = 1,
        get_binary = 2,
        get_name = 3,
        get_datatype = 4,
        get_list_size = 5,
        get_list_element = 6,

        bits_count = 6
    };
public:
    Executor() noexcept : LogWriter("Executor") {}

    Executor& getChains(const ListIChains& chains) noexcept;
    void      run();

private:

    void runChain();

    void runBinaryOperand();
    void assignContainer(ChainContainerTarget & target, ChainContainerTarget & value, const std::bitset<bits_count>& flagsToAssign);
    void assignFromString(ChainContainerTarget& target, const std::string& value, const std::bitset<bits_count>& flagsToAssign);
    void assignToString(std::string& target, ChainContainerTarget& value, const std::bitset<bits_count>& flagsToAssign);


    void runFunction();

    FloatVar    mMain;
    ListIChains mChains;
};

