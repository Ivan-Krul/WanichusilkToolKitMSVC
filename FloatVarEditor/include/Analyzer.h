#pragma once

#include "FloatVar.h"
#include "NodeMain.h"
#include "LogWriter.h"
#include "ChainValue.h"


class Analyzer : public LogWriter {
public:
    inline Analyzer() noexcept : LogWriter("Analyzer") {}

    Analyzer& acceptRootNode(RootNode root) noexcept;
    Analyzer& analyze();
    ListIChains getListChain() noexcept;

private:
    void analyzeLine();

    ListIChains                  mChains;
    std::shared_ptr<IChainValue> mTempChain;
    RootNode                     mRoot;
    std::shared_ptr<ParentNode>  mTempRoot;
};

