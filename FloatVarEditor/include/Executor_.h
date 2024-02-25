#pragma once
#include <stdio.h>

#include "FloatVar.h"
#include "NodeMain.h"
#include "LogMessage.h"

class Executor_ {
    using FloatVar = float_var_container_lib::FloatVar;

public:
    Executor_& getRootNode(const std::shared_ptr<RootNode>& root) noexcept;

    void run();
private:
    // templates for running partions of tree
    template<typename T>
    std::enable_if_t<std::is_same_v<T, ParentNode>> runPartion(const std::shared_ptr<ParentNode>& line);


    
    FloatVar pullValue(const std::shared_ptr<ParentNode>& line);
    FloatVar& pullFromContainer(const std::shared_ptr<ElementNode>& line);
    FloatVar pullFromContainerValue(const std::shared_ptr<BinarOperatorNode>& line);

    void convertToValue(FloatVar& reciever, const std::string& value);

    std::shared_ptr<RootNode> mRootNode;
    FloatVar mMain;

    size_t mLine;
    char mReqType[5]; // it will store required type for convertion value
    std::vector<LogMessage> mLogMessages;
};

template<typename T>
std::enable_if_t<std::is_same_v<T, ParentNode>> Executor_::runPartion(const std::shared_ptr<ParentNode>& line) {
    switch (line->getTokenDescriptor().tok) {
    case Token::equal:
        auto container = runPartion(std::static_pointer_cast<BinarOperatorNode, ParentNode>(line)->getLeftOperandNode());
        auto value = runPartion(std::static_pointer_cast<BinarOperatorNode, ParentNode>(line)->getRightOperandNode());
        
        container = value;

        break;
    case Token::dotdot:


        break;
    case Token::var_here:
        auto& instance = runPartion<ElementNode>(line);

        break;
    default:
        mLogMessages.push_back(LogMessage("Executor_", "What? In " + std::to_string(mLine) + " line is no head operation!"));
        break;
    }
}
