#include "TreeAssembler.h"

std::shared_ptr<ParentNode> TreeAssembler::parseToken_AtSign() {
    std::shared_ptr<ParentNode> inside;
    const auto currentHere = mCurrentPos; // in '@'

    if (mTokens[++mCurrentPos].tok != Token::rndBracket_open) {
        writeError("is no open round bracket, what should start a formula", true, "@");
        return inside;
    }

    inside = parseToken_Math();

    mCurrentPos++;

    return std::make_shared<InsideNode>(mTokens[currentHere], inside);
}

std::shared_ptr<ParentNode> TreeAssembler::parseToken_Math() {
    // we're starting in (

    // formula is a value, or variable, or a bunch of values what was made any computations

    // we have 3 instances of using a formula
    // 1.   formula
    // 2.   formula (+/-/...) formula
    // 3.   ( formula )

    // we're getting into a problem
    // who's controlling transfer formula?
    // what I mean by that

    // [2][+][3][*][8]
    //  > 2^ >? 1^  < 
    // we have to look further to see, is formula near prioritising operator

    // and ends after )

    auto wholeRPNFormula = parseToken_MathGetRPN();
    std::shared_ptr<ParentNode> right;
    std::shared_ptr<ParentNode> left;
    std::stack<std::shared_ptr<ParentNode>> treeStack;

    for (auto& rpnTok : wholeRPNFormula) {
        switch (rpnTok.first->getTokenDescriptor().tok) {
        case Token::plus:
        case Token::minus:
        case Token::mult:
        case Token::divide:
        case Token::procent:
            if (!rpnTok.second) {
                right = treeStack.top();
                treeStack.pop();
                left = treeStack.top();
                treeStack.pop();
                treeStack.push(std::make_shared<BinarOperatorNode>(rpnTok.first->getTokenDescriptor(), left, right));
                break;
            }
        default:
            treeStack.push(rpnTok.first);
            break;
        }
    }

    return treeStack.top();
}

// Shunting yard algorithm
std::vector<std::pair<std::shared_ptr<ParentNode>, bool>> TreeAssembler::parseToken_MathGetRPN() {
    // I would use RPN, like:
    // was: 4 - 2 * 3 / 2 + 1
    // res: 4 2 3 * 2 / - 1 +
    // or : ((4 ((2 3 *) 2 /) -) 1 +)
    // better would be to assemble to node
    // how?
    // here's a tree, what we want from previous formula          
    //          ┌─[+]─┐
    //          │    [1]
    //       ┌─[-]─┐ 
    //      [4]    │
    //          ┌─[/]─┐
    //          │    [2]
    //       ┌─[*]─┐ 
    //       │    [3]
    //      [2]

    std::vector<std::pair<std::shared_ptr<ParentNode>, bool>> wholeRPNFormula;
    std::stack<TokenDescriptor> operatorStack;
    // parse to list of operators and formulas to RPN
    while (mTokens[mCurrentPos].tok != Token::rndBracket_close) {
        wholeRPNFormula.push_back(std::make_pair(parseToken_MathParseFormula(), true));

        switch (mTokens[++mCurrentPos].tok) {
        case Token::plus:
        case Token::minus:
            parseToken_MathFlush(wholeRPNFormula, operatorStack);
        case Token::mult:
        case Token::divide:
        case Token::procent:
            operatorStack.push(mTokens[mCurrentPos]);
            break;
        default:
            writeError("is unknown operator", true, "(");
            break;
        case Token::rndBracket_close:
            break;
        }
    }

    parseToken_MathFlush(wholeRPNFormula, operatorStack);
    return wholeRPNFormula;
}

std::shared_ptr<ParentNode> TreeAssembler::parseToken_MathParseFormula() {
    std::shared_ptr<ParentNode> formula;
    switch (mTokens[++mCurrentPos].tok) {
    case Token::rndBracket_open:
        return parseToken_Math();
    case Token::number:
    case Token::var_this:
        return std::make_shared<ValueNode>(mTokens[mCurrentPos]);
    case Token::var_here:
        mCurrentPos++;
        return parseToken_VarHere();
    default:
        writeError("isn't a number, or value", true, "(");
        return std::shared_ptr<ParentNode>();
    }
}

void TreeAssembler::parseToken_MathFlush(std::vector<std::pair<std::shared_ptr<ParentNode>, bool>>& wholeRPNFormula, std::stack<TokenDescriptor>& operatorStack) {
    while (!operatorStack.empty()) {
        wholeRPNFormula.push_back(std::make_pair(std::make_shared<ParentNode>(operatorStack.top()), false));
        operatorStack.pop();
    }
}
