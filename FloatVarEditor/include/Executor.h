#pragma once

#include "FloatVar.h"
#include "NodeMain.h"
#include "LogMessage.h"

class Executor {
    using FloatVar       = float_var_container_lib::FloatVar;
    using ParentNodeList = std::vector<std::shared_ptr<ParentNode>>;

public:
    Executor& getRootNode(const RootNode& root) noexcept;
    void      run();

private:
    void runEqual();
    bool isVariableProperty(const BinarOperatorNode& variable);
    bool isPropertyConverted(const BinarOperatorNode& variable);
     
    void writeLog(const std::string& message);

    size_t         mLine;
    FloatVar       mMain;
    ParentNodeList mNodes;

    std::vector<LogMessage> mLogMessages;
};

