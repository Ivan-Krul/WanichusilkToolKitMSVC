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
        /*runFunction();*/                               break;
    }
}

void Executor::runBinaryOperand() {
    auto binarChain = std::static_pointer_cast<ChainValueBinarOperand, IChainValue>(mChains[mLine]);

    if (binarChain->getOperationType() != ChainValueBinarOperand::Operation::equal)
        throw std::exception((std::string(__FUNCTION__) + ": isn't equal").c_str());

    ChainContainerTarget target;
    target.setContainerPointer(&mMain);
    target.calculateTargetConainerPtr(binarChain->getLeftOperand());

    if (target.isConverted())
        throw std::exception("TODO: handle convertion of target");

    // assign
    switch (binarChain->getRightOperand()->getType()) {
    case IChainValue::TypeChain::string:
        assignString(target, std::static_pointer_cast<ChainValueString>(binarChain->getRightOperand())->getValue());
        break;
    case IChainValue::TypeChain::number:
    {
        auto number = *std::static_pointer_cast<ChainValueNumber>(binarChain->getRightOperand());

        switch (target.getProperty()) {
        case ChainValueContainer::Property::binary:
            target.getTarget()->setBinaryValue(number.getValue<uint64_t>());
            //target.getTarget()->;
            break;
        case ChainValueContainer::Property::name:
            throw std::exception((std::string(__FUNCTION__) + ": you can't name the container by raw number").c_str());
        case ChainValueContainer::Property::datatype:
            throw std::exception((std::string(__FUNCTION__) + ": you can't set datatype of the container by raw number").c_str());
        case ChainValueContainer::Property::size:
            throw std::exception((std::string(__FUNCTION__) + ": you can't manually set size by assigning number").c_str());
        case ChainValueContainer::Property::none:
            throw std::exception((std::string(__FUNCTION__) + ": you can't manually set container itself by assigning number").c_str());
        }
    } break;
    case IChainValue::TypeChain::container:
    {
        ChainContainerTarget assignedFrom;
        assignedFrom.setContainerPointer(&mMain);
        assignedFrom.calculateTargetConainerPtr(binarChain->getRightOperand());


        switch (target.getProperty()) {
        case ChainValueContainer::Property::size:
            throw std::exception((std::string(__FUNCTION__) + ": you can't manually set size by assigning number").c_str());
        case ChainValueContainer::Property::none:
            *target.getTarget() = *assignedFrom.getTarget(); break;
        case ChainValueContainer::Property::name:
            if (assignedFrom.isConverted())
                throw std::exception((std::string(__FUNCTION__) + ": you can't assign raw number as name").c_str());
            switch (assignedFrom.getProperty()) {
            case ChainValueContainer::Property::binary:
                target.getTarget()->name = assignedFrom.getTarget()->binary; break;
            case ChainValueContainer::Property::name:
                target.getTarget()->name = assignedFrom.getTarget()->name; break;
            case ChainValueContainer::Property::datatype:
                target.getTarget()->name = assignedFrom.getTarget()->data_type; break;
            case ChainValueContainer::Property::none:
                throw std::exception((std::string(__FUNCTION__) + ": you can't assign container as name").c_str());
            case ChainValueContainer::Property::size:
                throw std::exception((std::string(__FUNCTION__) + ": you can't assign raw number as name").c_str());
            }
            break;
        case ChainValueContainer::Property::datatype:
            if (assignedFrom.isConverted())
                throw std::exception((std::string(__FUNCTION__) + ": you can't assign raw number as datatype").c_str());
            switch (assignedFrom.getProperty()) {
            case ChainValueContainer::Property::binary:
                memcpy(target.getTarget()->data_type, assignedFrom.getTarget()->binary.c_str(), 4); break;
            case ChainValueContainer::Property::name:
                memcpy(target.getTarget()->data_type, assignedFrom.getTarget()->name.c_str(), 4); break;
            case ChainValueContainer::Property::datatype:
                memcpy(target.getTarget()->data_type, assignedFrom.getTarget()->data_type, 4); break;
            case ChainValueContainer::Property::none:
                throw std::exception((std::string(__FUNCTION__) + ": you can't assign container as datatype").c_str());
            case ChainValueContainer::Property::size:
                throw std::exception((std::string(__FUNCTION__) + ": you can't assign raw number as datatype").c_str());
            }
        case ChainValueContainer::Property::binary:
        }
    }
    }

    // OMG it's so huge, need to shrink it as well
    // handle binary assign from unconverted and converted values
    // also about math in function section
    // and binar operators
    // and optimize it if it's not cool
    // also make a convertion possible, but later
    // btw, we can swap all assigns to assingString()
    // it's useful I think
}

void Executor::assignString(ChainContainerTarget& target, const std::string& value) {
    switch (target.getProperty()) {
    case ChainValueContainer::Property::binary:
        target.getTarget()->binary = value;
        break;
    case ChainValueContainer::Property::name:
        target.getTarget()->name = value;
        break;
    case ChainValueContainer::Property::datatype:
        memcpy(target.getTarget()->data_type, value.c_str(), 4);
        break;
    case ChainValueContainer::Property::size:
        throw std::exception((std::string(__FUNCTION__) + ": you can't manually set size by assigning string").c_str());
    case ChainValueContainer::Property::none:
        throw std::exception((std::string(__FUNCTION__) + ": you can't manually set container itself by assigning string").c_str());
    }
}
