#include "Executor_.h"

Executor_& Executor_::getRootNode(const std::shared_ptr<RootNode>& root) noexcept {
    mRootNode = root;
    mLine = 0;
    return *this;
}

void Executor_::run() {
    auto line = mRootNode->getArray().front();

    for (mLine = 0; mLine < mRootNode->getArray().size(); mLine++) {
        line = mRootNode->getArray().at(mLine);
        runPartion<ParentNode>(line);
    }
}

float_var_container_lib::FloatVar& Executor_::pullFromContainer(const std::shared_ptr<ElementNode>& line) {
    if (line->getIndexSize() == 0)
        return mMain;

    auto& inst = *line;
    for (size_t i = 0; i < inst.getIndexSize(); i++) {
        switch (inst[i].getGutsNode()->getTokenDescriptor().tok) {
        case Token::dotdot:
            pullFromContainer(std::static_pointer_cast<ElementNode, ParentNode>(inst[i].getGutsNode()));
            break;
        case Token::number:
        case Token::string:
            pullValue(inst[i].getGutsNode());
            break;
        default:
            break;
        }
    }

}

float_var_container_lib::FloatVar Executor_::pullFromContainerValue(const std::shared_ptr<BinarOperatorNode>& line) {
    return FloatVar();
}

float_var_container_lib::FloatVar Executor_::pullValue(const std::shared_ptr<ParentNode>& line) {
    FloatVar fv;
    switch (line->getTokenDescriptor().tok) {
    case Token::number:
        convertToValue(fv, line->getTokenDescriptor().val);
        fv.setBinaryValue(line->getTokenDescriptor().val);
        return fv;
    case Token::string:
        fv.binary = line->getTokenDescriptor().val;
        return fv;
    default:
        mLogMessages.push_back(LogMessage("Executor_", "What? In " + std::to_string(mLine) + " line is not a value!"));
        break;
    }

    return fv;
}


void Executor_::convertToValue(FloatVar& reciever, const std::string& value) {
    switch (*reinterpret_cast<int*>(mReqType)) {
    case 0x6E626F6C: // nbol
    case 0x6E636872: // nchr
    case 0x6E756368: // nuch
        reciever.setBinaryValue(value.c_str(), 1);
        break;
    case 0x6E736872: // nshr
    case 0x6E757368: // nush
        reciever.setBinaryValue(value.c_str(), 2);
        break;
    case 0x6E696E74: // nint
    case 0x6E75696E: // nuin
    case 0x6E666C74: // nflt
        reciever.setBinaryValue(value.c_str(), 4);
        break;
    case 0x6E6C6738: // nlg8
    case 0x6E756C67: // nulg
    case 0x6E64626C: // ndbl
        reciever.setBinaryValue(value.c_str(), 8);
        break;

    case 0x766F6964: // void
    default:
        mLogMessages.push_back(LogMessage("Executor_", "What? In " + std::to_string(mLine) + " line is void value, instead of numeric type, or at least bool!"));
        break;
    }
}

