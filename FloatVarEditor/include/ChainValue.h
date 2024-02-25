#pragma once

#include "TreeAssembler.h"
#include "FloatVar.h"

struct IChainValue {
    enum class TypeChain : char {
        function,
        string,
        number,
        container
    };

    virtual TypeChain getType() const noexcept = 0;
};


class ChainValueString : public IChainValue {
public:
    inline ChainValueString(const std::string& value) noexcept {
        mValue = value;
    }
    inline TypeChain getType() const override {
        return TypeChain::string;
    }
    inline std::string getValue() const noexcept {
        return mValue;
    }

private:

    std::string mValue;
};


class ChainValueNumber : public IChainValue {
public:
    enum class DataType : char {
        Bool, Char, Short, Int, Unsign, Size, Float, Double
    };

    ChainValueNumber(const std::string& value, const DataType predefinedType) {
        mType = predefinedType;

        switch (mType) {
        case DataType::Bool:   mValue.bol = value == "true";    break;
        case DataType::Char:   mValue.chr = std::stoi(value);   break;
        case DataType::Short:  mValue.shr = std::stoi(value);   break;
        case DataType::Int:    mValue.in  = std::stoi(value);   break;
        case DataType::Unsign: mValue.un  = std::stoul(value);  break;
        case DataType::Size:   mValue.ull = std::stoull(value); break;
        case DataType::Float:  mValue.flt = std::stof(value);   break;
        case DataType::Double: mValue.dbl = std::stod(value);   break;
        }
    }
    ChainValueNumber(const std::string& value) {
        if (value == "true" || value == "false") {
            mType = DataType::Bool;
            mValue.bol = value == "true";
        }
        else if (value[0] == '.') {
            mType = DataType::Float;
            mValue.flt = std::stof(value);
        }
        else if (isdigit(value[0])) { // if here would be -, then well, no offense
            

        } else {
            // ???
        }
    }
    inline TypeChain getType() const override {
        return TypeChain::number;
    }
    template <typename T>
    inline std::enable_if_t<std::is_arithmetic_v<T>, T> getValue() const noexcept {
        switch (sizeof(T)) {
        case 1: return mValue.chr;
        case 2: return mValue.shr;
        case 4: return mValue.in;
        case 8: return mValue.ull;
        }
    }

private:

    DataType     mType;

    union NumericValueUnion {
        bool     bol;
        char     chr;
        short    shr;
        int      in;
        unsigned un;
        uint64_t ull;
        float    flt;
        double   dbl;
    }            mValue;
};


class ChainValueFunction : public IChainValue {
public:
    using ListIChains = std::vector<std::unique_ptr<IChainValue>>;

    enum class Operation : char {
        clear, read, write, pop, push,
    };

    inline TypeChain getType() const override {
        return TypeChain::function;
    }
    inline Operation getOperation() const noexcept {
        return mOperation;
    }

private:

    ListIChains mArguments;
    Operation   mOperation;
};

