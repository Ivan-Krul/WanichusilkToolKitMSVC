#include "ChainValue.h"

ChainValueNumber::ChainValueNumber(const std::string& value, const DataType predefinedType) {
    mType = predefinedType;
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

ChainValueNumber::ChainValueNumber(const std::string& value) {
    mValid = true;
    mValue = { 0 };
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
    auto l_node = node.getRightOperandNode();
    if (l_node->getTokenDescriptor().tok == Token::dotdot)
        mLeftOperand = std::make_shared<ChainValueContainer>(std::static_pointer_cast<BinarOperatorNode>(l_node));
    else if (l_node->getTokenDescriptor().tok == Token::var_here)
        mLeftOperand = std::make_shared<ChainValueContainer>(std::static_pointer_cast<ElementNode>(l_node));
    else
        throw std::exception((std::string(__FUNCTION__) + ": invalud object to assign").c_str());

    auto r_node = node.getRightOperandNode();
    switch (r_node->getTokenDescriptor().tok) {
    case Token::var_here: // container
        mRightOperand = std::make_shared<ChainValueContainer>(*(std::static_pointer_cast<ElementNode>(r_node).get()));
        break;
    case Token::dotdot:   // container + property
        mRightOperand = std::make_shared<ChainValueContainer>(*(std::static_pointer_cast<BinarOperatorNode>(r_node).get()));
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
