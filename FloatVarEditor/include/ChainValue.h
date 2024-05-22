#pragma once

#include "TreeAssembler.h"
#include "FloatVar.h"

struct IChainValue;

using ListIChains = std::vector<std::shared_ptr<IChainValue>>;

struct IChainValue {
    enum class TypeChain : char {
        function,
        string,
        number,
        container,
        binary_operator,
    };

    virtual TypeChain getType() const noexcept = 0;
};


class ChainValueString : public IChainValue {
public:
    inline ChainValueString(const std::string& value) noexcept {
        mValue = value;
    }
    inline TypeChain getType() const noexcept override {
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

    ChainValueNumber(const std::string& value, const DataType predefinedType);
    ChainValueNumber(const std::string& value);
    inline TypeChain getType() const noexcept override {
        return TypeChain::number;
    }
    inline bool isValid() const noexcept {
        return mValid;
    }
    inline DataType getDataType() const {
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
        clear, read, write, pop, push, math
    };

    inline TypeChain getType() const noexcept override {
        return TypeChain::function;
    }
    inline Operation getOperation() const noexcept {
        return mOperation;
    }
    template <typename T, typename... TArgs>
    inline std::enable_if_t<std::is_base_of_v<IChainValue, T>> pushArgument(TArgs&&... args) {
        mArguments.push_back(std::make_shared<T>(args));
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
    enum class Property {
        none, binary, datatype, name, size
    };

    ChainValueContainer(const BinarOperatorNode& dotdotNode) {
        mSelectedProperty = Property::none;
        resolveIndexes(*std::static_pointer_cast<ElementNode, ParentNode>(dotdotNode.getRightOperandNode()));
        setProperty(dotdotNode.getLeftOperandNode()->getTokenDescriptor().tok);
    }

    ChainValueContainer(const ElementNode& container, Property prop) {
        mSelectedProperty = prop;
        resolveIndexes(container);
    }

    ChainValueContainer(const ElementNode& container) {
        mSelectedProperty = Property::none;
        resolveIndexes(container);
    }

    inline TypeChain getType() const noexcept override {
        return TypeChain::container;
    }

    inline ListIChains getTrace() const noexcept {
        return mTrace;
    }
    inline Property getPropertySelection() const noexcept {
        return mSelectedProperty;
    }

private:
    void resolveIndexes(const ElementNode& container);

    void setProperty(const Token tok) {
        switch (tok) {
        case Token::var_name:      mSelectedProperty = Property::name;     break;
        case Token::var_binary:    mSelectedProperty = Property::binary;   break;
        case Token::var_type:      mSelectedProperty = Property::datatype; break;
        case Token::var_list_size: mSelectedProperty = Property::size;     break;
        default:
            break;
        }
    }

    ListIChains mTrace;
    Property    mSelectedProperty;

};

class ChainValueBinarOperand : public IChainValue {
public:
    enum class Operation {
        equal, add, subtract, multiply, divide, modulo
    };

    ChainValueBinarOperand(const BinarOperatorNode& node) noexcept {
        switch (node.getTokenDescriptor().tok) {
        case Token::equal:   mOperationType = Operation::equal;    break;
        case Token::plus:    mOperationType = Operation::add;      break;
        case Token::minus:   mOperationType = Operation::subtract; break;
        case Token::mult:    mOperationType = Operation::multiply; break;
        case Token::divide:  mOperationType = Operation::divide;   break;
        case Token::procent: mOperationType = Operation::modulo;   break;
        }

        setOperands(node);
    }

    Operation getOperationType() const noexcept {
        return mOperationType;
    }

    inline TypeChain getType() const noexcept override {
        return TypeChain::binary_operator;
    }

private:
    void setOperands(const BinarOperatorNode& node);

    Operation                    mOperationType;
    std::shared_ptr<IChainValue> mRightOperand;
    std::shared_ptr<IChainValue> mLeftOperand;
};
