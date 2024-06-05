#pragma once

#include "FloatVar.h"
#include "ChainValue.h"
#include "LogWriter.h"

class Executor : public LogWriter {
    using FloatVar       = float_var_container_lib::FloatVar;

public:
    Executor() noexcept : LogWriter("Executor") {}

    Executor& getChains(const ListIChains& chains) noexcept;
    void      run();

private:

    void runChain();

    void runBinaryOperand();
    FloatVar* getContainerTarget(const std::shared_ptr<ChainValueBinarOperand>& equalChain);

    void runFunction();

    FloatVar    mMain;
    ListIChains mChains;
};

