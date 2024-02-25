#include "Executor.h"

Executor& Executor::getRootNode(const RootNode& root) noexcept {
    mNodes = root.getArray();
    mLine = 0;
	mMain = FloatVar();
    return *this;
}

void Executor::run() {
	switch (mNodes[mLine]->getTokenDescriptor().tok) {
	case Token::equal:
		runEqual();
	case Token::dotdot:
	default:
		break;
	}
	mLine++;
}

void Executor::runEqual() {
	auto current_line = *std::static_pointer_cast<BinarOperatorNode>(mNodes[mLine]);

	if (!isVariableProperty(*std::static_pointer_cast<BinarOperatorNode>(current_line.getLeftOperandNode()))) {
		writeLog("is function, but not variable");
		// ???
	}


}

bool Executor::isVariableProperty(const BinarOperatorNode& variable) {
	if (isPropertyConverted(variable))
		return true;

	switch (variable.getRightOperandNode()->getTokenDescriptor().tok) {
	case Token::var_binary:
	case Token::var_name:
	case Token::var_type:
		return true;
	default:
		return false;
	}
}

bool Executor::isPropertyConverted(const BinarOperatorNode& variable) {
	switch (variable.getRightOperandNode()->getTokenDescriptor().tok) {
	case Token::convert_toBool:
	case Token::convert_toChar:
	case Token::convert_toShort:
	case Token::convert_toInt:
	case Token::convert_toUnsignedInt:
	case Token::convert_toFloat:
	case Token::convert_toDouble:
	case Token::convert_toSize:
		return true;
	default:
		return false;
	}
}

void Executor::writeLog(const std::string& message) {
	mLogMessages.push_back(LogMessage("Executor", "In active line " + std::to_string(mLine) + ", " + message));
}
