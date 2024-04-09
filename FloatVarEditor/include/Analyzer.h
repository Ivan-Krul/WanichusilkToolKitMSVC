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
    std::vector<ListIChains> getListChain() noexcept;

private:
    void analyzeLine(size_t line);

    std::vector<ListIChains> mChains;
    ListIChains              mTempChain;
    RootNode                 mRoot;

};

