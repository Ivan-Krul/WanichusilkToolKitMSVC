#include "ChainContainerTarget.h"

void ChainContainerTarget::setContainerPointer(FloatVar* p_container) noexcept {
    pOriginContainer = p_container;
}

void ChainContainerTarget::calculateTargetConainerPtr(const std::shared_ptr<IChainValue>& chain) {
    pTargetContainer = traceTargetContainerPtr(pOriginContainer, chain);
}

ChainValueContainer::Property ChainContainerTarget::getProperty() const noexcept {
    return mProperty;
}

bool ChainContainerTarget::isConverted() const noexcept {
    return mIsConverted;
}

ChainValueNumber::DataType ChainContainerTarget::getDataType() const noexcept {
    return mDataType;
}

ChainContainerTarget::FloatVar* ChainContainerTarget::getTarget() noexcept {
    return pTargetContainer;
}

ChainContainerTarget::FloatVar* ChainContainerTarget::traceTargetContainerPtr(FloatVar* p_container, const std::shared_ptr<IChainValue>& chain) {
    if (chain->getType() == IChainValue::TypeChain::number) {
        auto number_unresolved = std::static_pointer_cast<ChainValueNumber>(chain);
        if (number_unresolved->isResolved())
            throw std::exception((std::string(__FUNCTION__) + ": indexing convertion is resolved").c_str());

        mIsConverted = true;
        mDataType = number_unresolved->getDataType();

        return traceTargetContainerPtrRaw(p_container, std::static_pointer_cast<ChainValueContainer>(number_unresolved->getUnresolvedChain()));
    } else
        return traceTargetContainerPtrRaw(p_container, std::static_pointer_cast<ChainValueContainer>(chain));
}

ChainContainerTarget::FloatVar* ChainContainerTarget::traceTargetContainerPtrRaw(FloatVar* p_container, const std::shared_ptr<ChainValueContainer>& chain) {
    auto p_target = p_container;
    auto trace = chain->getTrace();
    mProperty = chain->getPropertySelection();

    for (size_t t = 0; t < trace.size(); t++) {
        switch (trace[t]->getType()) {
        case IChainValue::TypeChain::number:
            pointToElementTargetWithNumber(p_target, trace[t]);
            break;
        case IChainValue::TypeChain::string:
            pointToElementTargetWithString(p_target, std::static_pointer_cast<ChainValueString>(trace[t])->getValue());
            break;
        case IChainValue::TypeChain::container:
            pointToElementTargetWithContainer(p_target, trace[t]);
            break;
        case IChainValue::TypeChain::function:
            pointToElementTargetWithFunction(p_target, trace[t]);
            break;
        case IChainValue::TypeChain::binary_operator:
            throw std::exception((std::string(__FUNCTION__) + ": indexing with binary operations itself impossible").c_str());
        }
    }
    return p_target;
}

void ChainContainerTarget::pointToElementTargetWithString(FloatVar* p_target, const std::string& index_name) {
    auto search = [&index_name](decltype(*(p_target->list.begin())) value) { return value.name == index_name; };
    auto iter = std::find_if(p_target->list.begin(), p_target->list.end(), search);

    if (iter == p_target->list.end())
        throw std::exception((std::string(__FUNCTION__) + ": index with string like \"" + index_name + "\" wasn't found").c_str());

    p_target = &(*iter);
}

void ChainContainerTarget::pointToElementTargetWithNumber(FloatVar* p_target, const std::shared_ptr<IChainValue>& chain) {
    auto number = std::static_pointer_cast<ChainValueNumber>(chain);

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

void ChainContainerTarget::pointToElementTargetWithContainer(FloatVar* p_target, const std::shared_ptr<IChainValue>& chain) {
    auto child_container = std::static_pointer_cast<ChainValueContainer>(chain);
    if (child_container->getPropertySelection() == ChainValueContainer::Property::none)
        throw std::exception((std::string(__FUNCTION__) + ": indexing with container itself impossible").c_str());

    auto cont = traceTargetContainerPtr(pOriginContainer, child_container);

    switch (child_container->getPropertySelection()) {
    case ChainValueContainer::Property::binary:
        pointToElementTargetWithString(p_target, cont->binary); break;
    case ChainValueContainer::Property::datatype:
        pointToElementTargetWithString(p_target, cont->data_type); break;
    case ChainValueContainer::Property::name:
        pointToElementTargetWithString(p_target, cont->name); break;
    case ChainValueContainer::Property::size:
        if (cont->list.size() >= p_target->list.size())
            throw std::exception((std::string(__FUNCTION__) + ": index of size is overflowing").c_str());

        p_target = &(p_target->list[cont->list.size()]);
        break;
    }
}

void ChainContainerTarget::pointToElementTargetWithFunction(FloatVar* p_target, const std::shared_ptr<IChainValue>& chain) {
    auto function = std::static_pointer_cast<ChainValueOperation>(chain);

    switch (function->getOperation()) {
    case ChainValueOperation::Operation::math:
        throw std::exception("TODO: handle math in indexing");
    default:
        throw std::exception((std::string(__FUNCTION__) + ": all functions returns nothing").c_str());
    }
}

