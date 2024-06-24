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
        assignFromString(target, std::static_pointer_cast<ChainValueString>(binarChain->getRightOperand())->getValue(), set_binary | set_name | set_datatype);
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
            if (target.isConverted())
                throw std::exception("TODO: convertion handling for binary containers");

            switch (assignedFrom.getProperty()) {
            case ChainValueContainer::Property::binary:   target.getTarget()->binary = assignedFrom.getTarget()->binary;      break;
            case ChainValueContainer::Property::name:     target.getTarget()->binary = assignedFrom.getTarget()->name;        break;
            case ChainValueContainer::Property::datatype: target.getTarget()->binary = assignedFrom.getTarget()->data_type;   break;
            case ChainValueContainer::Property::size:     target.getTarget()->binary = assignedFrom.getTarget()->list.size(); break;
            case ChainValueContainer::Property::none:
                throw std::exception((std::string(__FUNCTION__) + ": you can't assign raw number as datatype").c_str());
            }
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

void Executor::assignContainer(ChainContainerTarget& target, ChainContainerTarget& value, const std::bitset<bits_count>& flagsToAssign) {
    switch (target.getProperty()) {
    case ChainValueContainer::Property::binary:
        break;
    case ChainValueContainer::Property::name:
    case ChainValueContainer::Property::datatype:
    case ChainValueContainer::Property::size:
    case ChainValueContainer::Property::none:
        break;

    }
}

void Executor::assignFromString(ChainContainerTarget& target, const std::string& value, const std::bitset<bits_count>& flagsToAssign) {
    if (flagsToAssign.count() == 0)
        throw std::exception((std::string(__FUNCTION__) + ": all bitset flags are set to 0").c_str());

    switch (target.getProperty()) {
    case ChainValueContainer::Property::binary:
        if (!flagsToAssign[get_binary])
            throw std::exception((std::string(__FUNCTION__) + ": you can't set binary by assigning string").c_str());
        target.getTarget()->binary = value;
        break;
    case ChainValueContainer::Property::name:
        if (!flagsToAssign[get_name])
            throw std::exception((std::string(__FUNCTION__) + ": you can't set name by assigning string").c_str());
        target.getTarget()->name = value;
        break;
    case ChainValueContainer::Property::datatype:
        if (!flagsToAssign[get_datatype])
            throw std::exception((std::string(__FUNCTION__) + ": you can't set datatype by assigning string").c_str());
        memcpy(target.getTarget()->data_type, value.c_str(), 4);
        break;
    case ChainValueContainer::Property::size:
        throw std::exception((std::string(__FUNCTION__) + ": you can't manually set size by assigning string").c_str());
    case ChainValueContainer::Property::none:
        throw std::exception((std::string(__FUNCTION__) + ": you can't manually set container itself by assigning string").c_str());
    }
}

void Executor::assignToString(std::string& target, ChainContainerTarget& value, const std::bitset<bits_count>& flagsToAssign) {
    if (flagsToAssign.count() == 0)
        throw std::exception((std::string(__FUNCTION__) + ": all bitset flags are set to 0").c_str());

    switch (value.getProperty()) {
    case ChainValueContainer::Property::binary:
        if (!flagsToAssign[get_binary])
            throw std::exception((std::string(__FUNCTION__) + ": you can't assign from binary").c_str());
        target = value.getTarget()->binary;
        break;
    case ChainValueContainer::Property::name:
        if (!flagsToAssign[get_name])
            throw std::exception((std::string(__FUNCTION__) + ": you can't assign from name").c_str());
        target = value.getTarget()->name;
        break;
    case ChainValueContainer::Property::datatype:
        if (!flagsToAssign[get_datatype])
            throw std::exception((std::string(__FUNCTION__) + ": you can't assign from datatype").c_str());
        target = value.getTarget()->data_type;
        break;
    case ChainValueContainer::Property::size:
        throw std::exception((std::string(__FUNCTION__) + ": you can't assign size of number as raw to string").c_str());
    case ChainValueContainer::Property::none:
        throw std::exception((std::string(__FUNCTION__) + ": you can't manually set string by assigning container itself").c_str());
    }
}
