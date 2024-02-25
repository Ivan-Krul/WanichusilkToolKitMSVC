#include "TreeAssembler.h"

TreeAssembler& TreeAssembler::acceptTokenList(const std::vector<TokenDescriptor>& tokens) noexcept {
    mTokens = tokens;
    mCurrentPos = 0;
    mLines = 0;
    mRoot.clear();
    mLogMessages.clear();
    return *this;
}

TreeAssembler& TreeAssembler::setOptionOutputLogs() noexcept {
    mOptionOutputLogs = true;
    return *this;
}

TreeAssembler& TreeAssembler::setOptionConcadBracketToIndex() noexcept {
    mOptionConcadBracketToIndex = true;
    return *this;
}

TreeAssembler& TreeAssembler::outputLogs() {
    if (mOptionOutputLogs) {
        if (mLogMessages.empty())
            mLogMessages.push_back(LogMessage("TreeAssembler", "Maybe everything is okay"));
        for (auto& message : mLogMessages) {
            message.print();
        }
    }
    return *this;
}

std::shared_ptr<ParentNode> TreeAssembler::parseExpression() {
    // strategy goes here!
    switch (mTokens[mCurrentPos++].tok) {
    case Token::var_here:
        return parseToken_VarHere();
    case Token::var_file:
        return parseToken_VarFile();
    default:
        writeError("is starting keyword, what wasn't except, because requires \"here\" or \"file\"", true, "start of the line");
    case Token::empty:
        return std::shared_ptr<ParentNode>();
    }
}

TreeAssembler& TreeAssembler::parse() {
    std::shared_ptr<ParentNode> ptr;
    
    while (mCurrentPos < mTokens.size()) {
        try {
            mLines++;
            ptr = parseExpression();
        } catch (const std::exception exc) {
            writeError(", something went wrong : " + std::string(exc.what()), false, "", true);
        }
        if (mCurrentPos >= mTokens.size())
            break;
        if (mTokens[mCurrentPos].tok == Token::empty)
            break;
        if (mTokens[mCurrentPos].tok == Token::newline) {
            mRoot.push(ptr);
            mCurrentPos++;
        } else
            writeError("this tree was ignored, because you forgot \";\" or something else in. Instead: \"" + mTokens[mCurrentPos].val + "\"");
    }
    return *this;
}

RootNode TreeAssembler::getRootNode() const {
    return mRoot;
}

std::shared_ptr<ParentNode> TreeAssembler::parseToken_VarFile() {
    switch (mTokens[mCurrentPos].tok) {
    case Token::dotdot:
        return parseToken_DotDot(std::make_shared<ActionNode>(mTokens[mCurrentPos++ - 1]));
    default:
        writeError("is a keyword, what wasn't except, because requires only \":\"", true, "file");
    case Token::newline:
        return std::make_shared<ElementNode>(mTokens[mCurrentPos++ - 1]);
    }
}

std::shared_ptr<BinarOperatorNode> TreeAssembler::parseToken_Equal(std::shared_ptr<ParentNode> prev) {
    // #2 root node
    size_t currentHere = mCurrentPos;

    std::shared_ptr<ParentNode> secondOperand;
    
    switch (mTokens[++mCurrentPos].tok) {
    case Token::var_here:
        mCurrentPos++;
        secondOperand = parseToken_VarHere();
        break;
    case Token::var_file:
        mCurrentPos++;
        secondOperand = parseToken_VarFile();
        break;
    case Token::string:
    case Token::number:
    case Token::bool_false:
    case Token::bool_true:
        secondOperand = std::make_shared<ValueNode>(mTokens[mCurrentPos++]);
        break;
    case Token::atSign:
        secondOperand = parseToken_AtSign();
        break;
    default:
        writeError("is a keyword, what wasn't except, because requires starting keywords, or at least value: string or number", true, "=");
        break;
    }

    return std::make_shared<BinarOperatorNode>(mTokens[currentHere], prev, secondOperand);
}

void TreeAssembler::writeError(const std::string& message, bool reqFromWhere, const std::string& tokenValue, bool except) {
    mLogMessages.push_back(LogMessage(except ? "TreeAssebler:Exception" : "TreeAssebler", "In " + std::to_string(mLines) + " active line " + message + (reqFromWhere ? ". '"+tokenValue+"' requires it":"")));
}
