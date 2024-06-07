#pragma once
#include "FloatVar.h"
#include "ChainValue.h"

class ChainContainerTarget {
    using FloatVar = float_var_container_lib::FloatVar;
public:
    void setContainerPointer(FloatVar* p_container) noexcept;
    void calculateTargetConainerPtr(const std::shared_ptr<IChainValue>& chain);

    ChainValueContainer::Property getProperty() const noexcept;
    bool isConverted() const noexcept;
    ChainValueNumber::DataType getDataType() const noexcept;
    FloatVar* getTarget() noexcept;

    template<typename T> T getValue() noexcept;

private:
    FloatVar* traceTargetContainerPtr(FloatVar* p_container, const std::shared_ptr<IChainValue>& chain);
    FloatVar* traceTargetContainerPtrRaw(FloatVar* p_container, const std::shared_ptr<ChainValueContainer>& chain);

    void pointToElementTargetWithString(FloatVar* p_target, const std::string& index_name);
    void pointToElementTargetWithNumber(FloatVar* p_target, const std::shared_ptr<IChainValue>& chain);
    void pointToElementTargetWithContainer(FloatVar* p_target, const std::shared_ptr<IChainValue>& chain);
    void pointToElementTargetWithFunction(FloatVar* p_target, const std::shared_ptr<IChainValue>& chain);

    std::shared_ptr<IChainValue> mContainerChain;
    FloatVar* pOriginContainer = nullptr;
    FloatVar* pTargetContainer = nullptr;

    ChainValueContainer::Property mProperty;
    bool mIsConverted = false;
    ChainValueNumber::DataType mDataType;
};

template<typename T>
inline T ChainContainerTarget::getValue() noexcept {
    switch (mDataType) {
    case ChainValueNumber::DataType::Bool:   return pTargetContainer->convertTo<bool>();
    case ChainValueNumber::DataType::Char:   return pTargetContainer->convertTo<char>();
    case ChainValueNumber::DataType::Short:  return pTargetContainer->convertTo<short>();
    case ChainValueNumber::DataType::Int:    return pTargetContainer->convertTo<int>();
    case ChainValueNumber::DataType::Unsign: return pTargetContainer->convertTo<unsigned>();
    case ChainValueNumber::DataType::Size:   return pTargetContainer->convertTo<uint64_t>();
    case ChainValueNumber::DataType::Float:  return pTargetContainer->convertTo<float>();
    case ChainValueNumber::DataType::Double: return pTargetContainer->convertTo<double>();
    }
}
