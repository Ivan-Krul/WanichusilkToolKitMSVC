#include "Executor.h"

Executor& Executor::getChains(const ListIChains& chains) noexcept {
    mChains = chains;
    return *this;
}

void Executor::run() {
    mMain = FloatVar();

    for (mLine = 0; mLine < mChains.size(); mLine++) {
        try {
            runChain();
        } catch (const std::exception& exc) {
            writeErr("something went wrong : " + std::string(exc.what()), true);
            return;
        }
    }
}

void Executor::runChain() {
    auto& chain = mChains[mLine];

    switch (chain->getType()) {
    case IChainValue::TypeChain::number:
        writeErr("number is in uncertant place");    break;
    case IChainValue::TypeChain::string:
        writeErr("string is in uncertant place");    break;
    case IChainValue::TypeChain::container:
        writeErr("container is in uncertant place"); break;
    case IChainValue::TypeChain::binary_operator:
        runBinaryOperand();                          break;
    case IChainValue::TypeChain::function:
        runFunction();                               break;
    }
}

void Executor::runBinaryOperand() {
    auto binarChain = std::static_pointer_cast<ChainValueBinarOperand, IChainValue>(mChains[mLine]);

    if (binarChain->getOperationType() != ChainValueBinarOperand::Operation::equal)
        throw std::exception((std::string(__FUNCTION__) + ": isn't equal").c_str());

    ChainContainerTarget target;
    target.setContainerPointer(&mMain);
    target.calculateTargetConainerPtr(binarChain->getLeftOperand());
    
    // starting to developing that right operator assignment and ASSIGN that crap
    // also in ChainContainerTarget is something wrong, I feel it,
    // looks like it's handling numbers and datatypes, where shouldn't be
    // also could be a problem of resolving ChainValueContainer, instead of ChainValueNumber
    // complicated stuff
}
