#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include <algorithm>

#include "LogWriter.h"
#include "Token.h"

class Lexer : public LogWriter {
public:
	static std::unordered_map<std::string, Token> sTokenDictionary;

	Lexer() noexcept : LogWriter("Lexer") {};

	Lexer& setOptionClearSpacing();
	Lexer& setOptionAssembleString();
	Lexer& setOptionAssembleNumber();

	Lexer& parse(const std::string& code);

	std::vector<TokenDescriptor> getListTokens() noexcept;
private:
	void gatherSize();
	TokenDescriptor generateToken();
	bool clearComment();
	bool clearInlineComment();
	std::string formString();
	std::string formNumber();

	bool markAsPlainCharacter();
	void searchToken();
	void checkPartionForDigit();

	std::vector<size_t> countElements(const std::string& str, const std::string & sample);

	std::string mCode;
	size_t mRow = 0;

	bool mOptionClearSpacing = false;
	bool mOptionAssembleString = false;
	bool mOptionAssembleNumber = false;

	std::vector<size_t> mSizeCollection;
	std::vector<TokenDescriptor> mTokens;

	bool mFoundToken = false;
	TokenDescriptor mCurrentToken;
	std::string mPartion;
};
