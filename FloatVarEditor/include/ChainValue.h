#pragma once

#include "TreeAssembler.h"
#include "FloatVar.h"

using ListIChains = std::vector<std::unique_ptr<IChainValue>>;

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
        mValid = true;

        if (value == "true" || value == "false") {
            mType = DataType::Bool;
            mValue.bol = value == "true";
        }
        else if (value[0] == '.') {
            mType = DataType::Float;
            mValue.flt = std::stof(value);
        }
        else if (isdigit(value[0])) { // if here would be -, then well, no offense
            if (value.find('.') == value.npos) {
                // int
                mType = DataType::Int;
                mValue.in = std::stoi(value);
            }
            else {
                mType = DataType::Float;
                mValue.flt = std::stof(value);
            }

        } else {
            mValid = false;
        }
    }
    inline TypeChain getType() const override {
        return TypeChain::number;
    }
    inline bool isValid() const noexcept {
        return mValid;
    }
    inline DataType getDataType() {
        return mType;
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
    bool         mValid;

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


class ChainValueOperation : public IChainValue {
public:
    ChainValueOperation(Token oper) {
        setOperation(oper);
    }
    ChainValueOperation(Token oper, const ListIChains& args) {
        setOperation(oper);
        mArguments = args;
    }

    enum class Operation : char {
        clear, read, write, pop, push,
    };

    inline TypeChain getType() const override {
        return TypeChain::function;
    }
    inline Operation getOperation() const noexcept {
        return mOperation;
    }
    template <typename T, typename... TArgs>
    inline std::enable_if_t<std::is_base_of_v<IChainValue, T>> pushArgument(TArgs&&... args) {
        mArguments.push_back(std::make_unique<T>(args));
    }
    inline ListIChains getArguments() const noexcept {
        return mArguments;
    }

private:

    void setOperation(Token token) {
        switch (token) {
        case Token::var_clear: mOperation = Operation::clear; break;
        case Token::var_read:  mOperation = Operation::read;  break;
        case Token::var_write: mOperation = Operation::write; break;
        case Token::var_pop:   mOperation = Operation::pop;   break;
        case Token::var_push:  mOperation = Operation::push;  break;
        default:
            break;
        }          
    }

    ListIChains mArguments;
    Operation   mOperation;
};


class ChainValueContainer : public IChainValue {
public:
    ChainValueContainer(const ElementNode& container) {
        mSelectedProperty = Property::none;
    }

    enum class Property {
        none, binary, datatype, name
    };

    inline TypeChain getType() const override {
        return TypeChain::container;
    }

    inline ListIChains getTrace() const noexcept {
        return mTrace;
    }
    inline Property getPropertySelection() const noexcept {
        return mSelectedProperty;
    }

private:
    void resolveIndexes(const ElementNode& container) {
        std::shared_ptr<ParentNode> guts;
        for (size_t i = 0; i < container.getIndexSize(); i++) {
            guts = container[i].getGutsNode();

        }
    }

    ListIChains mTrace;
    Property    mSelectedProperty;

};
