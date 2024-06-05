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
        runFunction();
    }
}

void Executor::runBinaryOperand() {
    auto binarChain = std::static_pointer_cast<ChainValueBinarOperand, IChainValue>(mChains[mLine]);

    if (binarChain->getOperationType() != ChainValueBinarOperand::Operation::equal)
        throw std::exception((std::string(__FUNCTION__) + ": isn't equal").c_str());

    auto p_fv_target = getContainerTarget(binarChain);

}

float_var_container_lib::FloatVar* Executor::getContainerTarget(const std::shared_ptr<ChainValueBinarOperand>& equalChain) {
    auto container = std::static_pointer_cast<ChainValueContainer>(equalChain->getLeftOperand());
    auto trace = container->getTrace();

    FloatVar* p_fv_target = &mMain;

    for (size_t t = 0; t < trace.size(); t++) {
        switch (trace[t]->getType()) {
        case IChainValue::TypeChain::number:
        {
            auto number = std::static_pointer_cast<ChainValueNumber>(trace[t]);
            if(number->getDataType() == ChainValueNumber::DataType::Double || number->getDataType() == ChainValueNumber::DataType::Float)
                throw std::exception((std::string(__FUNCTION__) + ": indexing with number has to be only in integer numbers in range [0 - inf)").c_str());

            p_fv_target = &(p_fv_target->list[number->getValue<uint64_t>()]);
        } break;
        case IChainValue::TypeChain::string:
        {
            auto string = std::static_pointer_cast<ChainValueString>(trace[t])->getValue();
            auto iter = std::find_if(p_fv_target->list.begin(), p_fv_target->list.end(), [=](const decltype(p_fv_target->list.begin())& value) { return value->name == string; });

            if(iter == p_fv_target->list.end())
                throw std::exception((std::string(__FUNCTION__) + ": index with string like \""+string+"\" wasn't found").c_str());

            p_fv_target = &(*iter);
        } break;
        case IChainValue::TypeChain::
        }
    }

    return p_fv_target;
}
