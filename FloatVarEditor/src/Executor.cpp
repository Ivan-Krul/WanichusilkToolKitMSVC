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

    auto p_fv_target = getContainerTarget(std::static_pointer_cast<ChainValueContainer>(binarChain->getLeftOperand()));

}

float_var_container_lib::FloatVar* Executor::getContainerTarget(const std::shared_ptr<ChainValueContainer>& container) {
    auto trace = container->getTrace();

    FloatVar* p_fv_target = &mMain;

    for (size_t t = 0; t < trace.size(); t++) {
        switch (trace[t]->getType()) {
        case IChainValue::TypeChain::number:
            pointToElementTargetWithNumber(p_fv_target, std::static_pointer_cast<ChainValueNumber>(trace[t]));
            break;
        case IChainValue::TypeChain::string:
            pointToElementTargetWithString(p_fv_target, std::static_pointer_cast<ChainValueString>(trace[t])->getValue());
            break;
        case IChainValue::TypeChain::container:
        {
            auto container = std::static_pointer_cast<ChainValueContainer>(trace[t]);
            if(container->getPropertySelection() == ChainValueContainer::Property::none)
                throw std::exception((std::string(__FUNCTION__) + ": indexing with container itself impossible").c_str());

            auto cont = getContainerTarget(container);

            switch (container->getPropertySelection()) {
            case ChainValueContainer::Property::binary: 
                pointToElementTargetWithString(p_fv_target, cont->binary); break;
            case ChainValueContainer::Property::datatype:
                pointToElementTargetWithString(p_fv_target, cont->data_type); break;
            case ChainValueContainer::Property::name:
                pointToElementTargetWithString(p_fv_target, cont->name); break;
            case ChainValueContainer::Property::size:
                if (cont->list.size() >= p_fv_target->list.size())
                    throw std::exception((std::string(__FUNCTION__) + ": index of size is overflowing").c_str());

                p_fv_target = &(p_fv_target->list[cont->list.size()]);
            default:
                break;
            }
        }
        case IChainValue::TypeChain::function:
        {
            auto function = std::static_pointer_cast<ChainValueOperation>(trace[t]);

            switch (function->getOperation()) {
            case ChainValueOperation::Operation::math:
                throw std::exception("TODO: handle math in indexing");
            default:
                throw std::exception((std::string(__FUNCTION__) + ": all functions returns nothing").c_str());
            }
        }
        case IChainValue::TypeChain::binary_operator:
            throw std::exception((std::string(__FUNCTION__) + ": indexing with binary operations itself impossible").c_str());
        }
    }

    return p_fv_target;
}

void Executor::pointToElementTargetWithString(FloatVar* p_target, const std::string& index_name) {
    auto iter = std::find_if(p_target->list.begin(), p_target->list.end(), [=](const decltype(p_target->list.begin())& value) { return value->name == index_name; });

    if (iter == p_target->list.end())
        throw std::exception((std::string(__FUNCTION__) + ": index with string like \"" + index_name + "\" wasn't found").c_str());

    p_target = &(*iter);
}

void Executor::pointToElementTargetWithNumber(FloatVar* p_target, const std::shared_ptr<ChainValueNumber>& number) {
    if (number->isResolved()) {
        if (number->isValid())
            throw std::exception((std::string(__FUNCTION__) + ": number for indexing for is invalid").c_str());
        if (number->getDataType() == ChainValueNumber::DataType::Double || number->getDataType() == ChainValueNumber::DataType::Float)
            throw std::exception((std::string(__FUNCTION__) + ": indexing with number has to be only in integer numbers in range [0 - inf)").c_str());
        if (number->getValue<uint64_t>() >= p_target->list.size())
            throw std::exception((std::string(__FUNCTION__) + ": index is overflowing").c_str());

        p_target = &(p_target->list[number->getValue<uint64_t>()]);
    } else std::exception("TODO: handling for unresolved number to get container");
}
