#include "Analyzer.h"

Analyzer& Analyzer::acceptRootNode(RootNode root) noexcept {
    mRoot = root;
    clearLogs();
    return *this;
}

Analyzer& Analyzer::analyze() {
    for (size_t i = 0; i < mRoot.size(); i++) {
        try {
            analyzeLine(i);
        }
        catch (const std::exception& exc) {
            writeErr(", something went wrong : " + std::string(exc.what()), i, true);
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
    
}
