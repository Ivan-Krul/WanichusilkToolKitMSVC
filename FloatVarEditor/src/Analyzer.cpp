#include "Analyzer.h"

Analyzer& Analyzer::acceptRootNode(RootNode root) noexcept {
    mRoot = root;
    clearLogs();
    return *this;
}

Analyzer& Analyzer::analyze() {
    auto root_array = mRoot.getArray();

    for (mLine = 0; mLine < root_array.size(); mLine++) {
        try {
            mTempRoot = root_array[mLine];
            analyzeLine();
        }
        catch (const std::exception& exc) {
            writeErr("something went wrong : " + std::string(exc.what()), true);
        }
        if (mTempChain.empty())
            writeErr("it contains no key values", mLine);
        else
            mChains.push_back(mTempChain);
    }
    return *this;
}

std::vector<ListIChains> Analyzer::getListChain() noexcept {
    return mChains;
}

void Analyzer::analyzeLine() {
    switch (mTempRoot->getTokenDescriptor().tok) {
    case Token::dotdot:
        mTempChain.push_back(std::make_shared<ChainValueOperation>(std::static_pointer_cast<BinarOperatorNode>(mTempRoot)->getRightOperandNode()->getTokenDescriptor().tok));
        break;
    case Token::equal:
        analyzeEqual();
        break;
    default:
        break;
    }
}

void Analyzer::analyzeEqual() {
    auto binar = std::static_pointer_cast<BinarOperatorNode>(mTempRoot);
    auto left = binar->getLeftOperandNode();

    if (left->getTokenDescriptor().tok == Token::dotdot) {
        auto left_bundle = std::static_pointer_cast<BinarOperatorNode>(left);
        ChainValueContainer::Property prop;

        switch (left_bundle->getRightOperandNode()->getTokenDescriptor().tok) {
        case Token::var_binary:    prop = ChainValueContainer::Property::binary;   break;
        case Token::var_name:      prop = ChainValueContainer::Property::name;     break;
        case Token::var_type:      prop = ChainValueContainer::Property::datatype; break;
        case Token::var_list_size: prop = ChainValueContainer::Property::size;     break;
        }

        mTempChain.push_back(std::make_shared<ChainValueContainer>(left_bundle->getLeftOperandNode(), prop));
    }
    else mTempChain.push_back(std::make_shared<ChainValueContainer>(left));

    auto right = binar->getRightOperandNode();

    switch (right->getTokenDescriptor().tok) {
    case Token::var_here: // container
    case Token::dotdot:   // container + property
    case Token::atSign:   // math formula
    case Token::number:   // number
    case Token::string:   // string
        break;
    }

}
