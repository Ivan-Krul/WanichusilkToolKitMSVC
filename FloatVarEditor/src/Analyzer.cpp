#include "Analyzer.h"

Analyzer& Analyzer::acceptRootNode(RootNode root) noexcept {
    mRoot = root;
    clearLogs();
    return *this;
}

Analyzer& Analyzer::analyze() {
    auto& root_array = mRoot.getArray();

    for (mLine = 0; mLine < root_array.size(); mLine++) {
        try {
            mTempChain = std::shared_ptr<IChainValue>();
            mTempRoot = root_array[mLine];
            analyzeLine();
        }
        catch (const std::exception& exc) {
            writeErr("something went wrong : " + std::string(exc.what()), true);
        }
        if (mTempChain == std::shared_ptr<IChainValue>())
            writeErr("it contains no key values", mLine);
        else
            mChains.push_back(mTempChain);
    }
    return *this;
}

ListIChains Analyzer::getListChain() noexcept {
    return mChains;
}

void Analyzer::analyzeLine() {
    switch (mTempRoot->getTokenDescriptor().tok) {
    case Token::dotdot:
        mTempChain = std::make_shared<ChainValueOperation>(std::static_pointer_cast<BinarOperatorNode>(mTempRoot)->getRightOperandNode()->getTokenDescriptor().tok);
        break;
    case Token::equal:
        mTempChain = std::make_shared<ChainValueBinarOperand>(*std::static_pointer_cast<BinarOperatorNode>(mTempRoot).get());
        break;
    default:
        break;
    }
}
