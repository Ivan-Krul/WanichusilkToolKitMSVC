#include "Analyzer.h"

Analyzer& Analyzer::acceptRootNode(RootNode root) noexcept {
    mRoot = root;
    clearLogs();
    return *this;
}

Analyzer& Analyzer::analyze() {
    auto root_array = mRoot.getArray();

    for (size_t i = 0; i < root_array.size(); i++) {
        try {
            mTempRoot = root_array[i];
            analyzeLine(i);
        }
        catch (const std::exception& exc) {
            writeErr("something went wrong : " + std::string(exc.what()), i, true);
        }
        if (mTempChain.empty())
            writeErr("it contains no key values", i);
        else
            mChains.push_back(mTempChain);
    }
    return *this;
}

std::vector<ListIChains> Analyzer::getListChain() noexcept {
    return mChains;
}

void Analyzer::analyzeLine(size_t line) {
    auto binar = std::static_pointer_cast<BinarOperatorNode>(mTempRoot);

    switch (mTempRoot->getTokenDescriptor().tok) {
    case Token::dotdot:
        mTempChain.push_back(std::make_shared<ChainValueOperation>(binar->getRightOperandNode()->getTokenDescriptor().tok));
    case Token::equal:
        
    default:

        break;
    }
    

    mTempChain;

}
