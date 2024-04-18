#pragma once
#include <vector>
#include <stack>

#include "Token.h"
#include "NodeMain.h"
#include "LogWriter.h"
#include "Lexer.h"

class TreeAssembler : public LogWriter {
public:
	TreeAssembler() noexcept : LogWriter("TreeAssembler") {}

	TreeAssembler& acceptTokenList(const std::vector<TokenDescriptor>& tokens) noexcept;
	TreeAssembler& setOptionConcadBracketToIndex() noexcept;
	TreeAssembler& parse();
	RootNode getRootNode() const;
private:
	std::shared_ptr<ParentNode> parseExpression();

	std::shared_ptr<ParentNode> parseToken_VarHere();
	std::shared_ptr<ParentNode> parseToken_VarFile();
	std::shared_ptr<ParentNode> parseToken_DotDotCheckCaseHere();
	std::shared_ptr<ParentNode> parseToken_DotDotCheckCaseDotDot();
	std::shared_ptr<ParentNode> parseToken_DotDotLookAround(std::shared_ptr<ParentNode> prev);
	std::shared_ptr<ParentNode> parseToken_DotDot(std::shared_ptr<ParentNode> prev);
	std::vector<InsideNode> parseToken_VarHereCheckCaseIndex();
	std::shared_ptr<ParentNode> parseToken_AtSign();
	std::shared_ptr<ParentNode> parseToken_Math();
	std::vector<std::pair<std::shared_ptr<ParentNode>, bool>> parseToken_MathGetRPN();
	std::shared_ptr<ParentNode> parseToken_MathParseFormula();
	void parseToken_MathFlush(std::vector<std::pair<std::shared_ptr<ParentNode>, bool>>& wholeRPNFormula, std::stack<TokenDescriptor>& operatorStack);
	std::shared_ptr<BinarOperatorNode> parseToken_Equal(std::shared_ptr < ParentNode> prev);

	void writeError(const std::string& message, bool reqFromWhere = false, const std::string& tokenValue = "", bool except = false);

	RootNode mRoot;
	std::vector<TokenDescriptor> mTokens;
	size_t mCurrentPos = 0;
	size_t mLines = 0;
	bool mOptionConcadBracketToIndex = false;

};

