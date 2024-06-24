#include "TreeAssembler.h"


std::shared_ptr<ParentNode> TreeAssembler::parseToken_DotDotCheckCaseHere() {
    const auto currentHere = mCurrentPos;
    switch (mTokens[currentHere].tok) {
        //variables
    case Token::var_binary:
    case Token::var_type:
    case Token::var_name:
    case Token::var_list_size:
        return std::make_shared<ValueNode>(mTokens[currentHere]);
        // actions
    case Token::var_edit:
    case Token::var_clear:
    case Token::var_read:
    case Token::var_pop:
        return std::make_shared<ActionNode>(mTokens[currentHere]);
    case Token::var_push:
        if (mTokens[currentHere + 1].tok == Token::string)
            return std::make_shared<UnarOperatorNode>(mTokens[currentHere], std::make_shared<ValueNode>(mTokens[++mCurrentPos]));
        else
            return std::make_shared<UnarOperatorNode>(mTokens[currentHere], std::make_shared<ParentNode>());
    default:
        writeError("is a keyword, what wasn't except, because requires property or action for keyword \"here\"", true, ":");
        return std::make_shared<ParentNode>();
    }
}

std::shared_ptr<ParentNode> TreeAssembler::parseToken_DotDotCheckCaseDotDot() {
    switch (mTokens[mCurrentPos].tok) {
        // convertors
    case Token::convert_toBool:
    case Token::convert_toChar:
    case Token::convert_toShort:
    case Token::convert_toInt:
    case Token::convert_toUnsignedInt:
    case Token::convert_toFloat:
    case Token::convert_toSize:
    case Token::convert_toDouble:
    case Token::convert_toString:
        return std::make_shared<ConvertNode>(mTokens[mCurrentPos]);
    case Token::var_write:
        return std::make_shared<ActionNode>(mTokens[mCurrentPos]);
    default:
        writeError("is a keyword, what wasn't except, because requires convertion for properties and keyword \"here\"", true, ":");
        return std::make_shared<ParentNode>();
    }
}

std::shared_ptr<ParentNode> TreeAssembler::parseToken_DotDotLookAround(std::shared_ptr<ParentNode> prev) {
    switch (prev->getTokenDescriptor().tok) { // previous token
    case Token::var_here:
        return parseToken_DotDotCheckCaseHere();
    case Token::var_file:
        switch (mTokens[mCurrentPos].tok) {
        case Token::var_file_in:
        case Token::var_file_out:
            return std::make_shared<UnarOperatorNode>(mTokens[mCurrentPos], std::make_shared<ValueNode>(mTokens[++mCurrentPos]));
        default:
            writeError("is a keyword, what wasn't except, because requires file action, \"in\" file or \"out\" file", true, ":");
            return std::shared_ptr<ParentNode>();
        }
    case Token::dotdot:
        return parseToken_DotDotCheckCaseDotDot();

    default:
        writeError("is a keyword, what wasn't except, because requires starting keywords", true, ":");
        return std::shared_ptr<ParentNode>();
    }
}

std::shared_ptr<ParentNode> TreeAssembler::parseToken_DotDot(std::shared_ptr<ParentNode> prev) {
    // #3 root node
    const size_t currentHere = mCurrentPos - 1;

    // look around the token
    auto right = parseToken_DotDotLookAround(prev);

    auto bin = std::make_shared<BinarOperatorNode>(mTokens.at(currentHere), prev, right);

    switch (mTokens[++mCurrentPos].tok) { // looking further
    case Token::equal:
        return parseToken_Equal(bin);
    case Token::dotdot:
        mCurrentPos++;
        return parseToken_DotDot(bin);
    default:
        writeError("is a keyword, what wasn't except, because requires \":\" or \"=\", or at least \";\"", true, ":");
    case Token::newline: // we saw a newline character, so it's the end of our forces
    case Token::sqrBracket_close: // or, we're in index space
        return bin;
    case Token::plus: // or, we're near math operations
    case Token::minus:
    case Token::mult:
    case Token::divide:
    case Token::procent:
        mCurrentPos--;
        return bin;
    }
}
