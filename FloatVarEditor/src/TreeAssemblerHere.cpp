#include "TreeAssembler.h"

std::shared_ptr<ParentNode> TreeAssembler::parseToken_VarHere() {
    auto currentHere = mCurrentPos - 1;
    std::vector<InsideNode> indexes;

    // looks for binding to "here"
    if (mTokens[mCurrentPos].tok == Token::sqrBracket_open)
        indexes = parseToken_VarHereCheckCaseIndex();

    switch (mTokens[mCurrentPos].tok) {
        // looks for binding "here"
    case Token::dotdot:
        mCurrentPos++;
        return parseToken_DotDot(std::make_shared<ElementNode>(mTokens[currentHere], indexes));
    case Token::equal:
        return parseToken_DotDot(std::make_shared<ElementNode>(mTokens[currentHere], indexes));

    case Token::newline:
        return parseToken_DotDot(std::make_shared<ElementNode>(mTokens[currentHere], indexes));
    default:
        writeError("is a keyword, what wasn't except, because requires \":\" or \"=\"", true, "here");
        break;
    }

    return std::make_shared<ElementNode>(mTokens[mCurrentPos++ - 1], indexes);
}


std::vector<InsideNode> TreeAssembler::parseToken_VarHereCheckCaseIndex() {
    std::vector<InsideNode> indexes;
    size_t currentHere = mCurrentPos;

    while (mTokens[mCurrentPos].tok == Token::sqrBracket_open) {
        switch (mTokens[++mCurrentPos].tok) { // next token should be inside of [
        case Token::number:
        case Token::string:
            indexes.push_back(InsideNode(mTokens[currentHere], std::make_shared<ValueNode>(mTokens[mCurrentPos])));
            mCurrentPos += 2;
            // it should end in [ if it is
            // like "[ 3105491 ] ["
            //       ^   1^   2^ ^
            // or   "[ 3105491 ] :"
            //       ^   1^   2^ ^  
            break;
        case Token::var_here:
            mCurrentPos++;
            indexes.push_back(InsideNode(mTokens[currentHere], parseToken_VarHere()));
            mCurrentPos++;
            break;
            // like "[ here [ 1 ] : binary ] [ ..."
            //       ^  *^ &^    %^          ^
            // array:     [&^]                [&^]
        default:
            writeError("is a keyword, what can't be as index of array or map", true, "[");
            break;
        }
    }

    // it should end in next token after ]
    indexes.shrink_to_fit();
    return indexes;
}
