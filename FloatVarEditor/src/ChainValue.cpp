#include "ChainValue.h"

ChainValueNumber::ChainValueNumber(const std::string& value, const DataType predefinedType) :
    mValue{ 0 } {
    mType = predefinedType;
    mResolved = true;
    mValid = true;
    mValue = { 0 };

    switch (mType) {
    case DataType::Bool:   mValue.bol = value == "true";    break;
    case DataType::Char:   mValue.chr = std::stoi(value);   break;
    case DataType::Short:  mValue.shr = std::stoi(value);   break;
    case DataType::Int:    mValue.in = std::stoi(value);   break;
    case DataType::Unsign: mValue.un = std::stoul(value);  break;
    case DataType::Size:   mValue.ull = std::stoull(value); break;
    case DataType::Float:  mValue.flt = std::stof(value);   break;
    case DataType::Double: mValue.dbl = std::stod(value);   break;
    }
}

ChainValueNumber::ChainValueNumber(const std::string& value) :
    mValue{ 0 } {
    mValid = true;
    mResolved = true;
    mType = DataType::Bool;

    if (value == "true" || value == "false") {
        mType = DataType::Bool;
        mValue.bol = value == "true";
    } else if (value[0] == '.') {
        mType = DataType::Float;
        mValue.flt = std::stof(value);
    } else if (isdigit(value[0])) { // if here would be -, then well, no offense
        if (value.find('.') == value.npos) {
            // int
            mType = DataType::Int;
            mValue.in = std::stoi(value);
        } else {
            mType = DataType::Float;
            mValue.flt = std::stof(value);
        }

    } else {
        mValid = false;
    }
}

ChainValueNumber::ChainValueNumber(const std::shared_ptr<IChainValue>& chain, const DataType type) :
    mValue{ 0 } {
        mValid = true;
        mType = type;
        mResolved = false;
        mUnresolvedChain = chain;
}

ChainValueNumber::ChainValueNumber(const std::shared_ptr<BinarOperatorNode>& chain) :
    mValue{ 0 } {
    mValid = true;
    mResolved = false;
    mType = DataType::Bool;

    switch (chain->getRightOperandNode()->getTokenDescriptor().tok) {
    case Token::convert_toBool: mType = DataType::Bool; break;
    case Token::convert_toChar: mType = DataType::Char; break;
    case Token::convert_toShort: mType = DataType::Short; break;
    case Token::convert_toInt: mType = DataType::Int; break;
    case Token::convert_toUnsignedInt: mType = DataType::Unsign; break;
    case Token::convert_toFloat: mType = DataType::Float; break;
    case Token::convert_toSize: mType = DataType::Size; break;
    case Token::convert_toDouble: mType = DataType::Double; break;
    }

    mUnresolvedChain = std::make_shared<ChainValueContainer>(chain->getLeftOperandNode());
}


void ChainValueContainer::resolveIndexes(const ElementNode& container) {
    std::shared_ptr<ParentNode> guts;

    for (size_t i = 0; i < container.getIndexSize(); i++) {
        guts = container[i].getGutsNode();

        switch (guts->getTokenDescriptor().tok) {
        case Token::var_here:
            mTrace.push_back(std::make_shared<ChainValueContainer>(*(std::static_pointer_cast<ElementNode>(guts).get())));
            break;
        case Token::string:
            mTrace.push_back(std::make_shared<ChainValueString>(guts->getTokenDescriptor().val));
            break;
        case Token::number:
            mTrace.push_back(std::make_shared<ChainValueNumber>(guts->getTokenDescriptor().val));
            break;
        }
    }
}

void ChainValueBinarOperand::setOperands(const BinarOperatorNode& node) {
    mLeftOperand = std::make_shared<ChainValueContainer>(node.getLeftOperandNode());

    auto r_node = node.getRightOperandNode();
    switch (r_node->getTokenDescriptor().tok) {
    case Token::var_here: // container
    case Token::dotdot:   // container + property
        mRightOperand = std::make_shared<ChainValueContainer>(r_node);
        break;
    case Token::number:   // number
        mRightOperand = std::make_shared<ChainValueNumber>(r_node->getTokenDescriptor().val);
        break;
    case Token::string:   // string
        mRightOperand = std::make_shared<ChainValueString>(r_node->getTokenDescriptor().val);
        break;
    case Token::atSign:   // math formula
        throw std::exception("TODO: math");
    }
}
