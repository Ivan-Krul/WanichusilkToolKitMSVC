#include "Lexer.h"

Lexer& Lexer::setOptionClearSpacing() {
	mOptionClearSpacing = true;
	return *this;
}

Lexer& Lexer::setOptionAssembleString() {
	mOptionAssembleString = true;
	return *this;
}

Lexer& Lexer::setOptionAssembleNumber() {
	mOptionAssembleNumber = true;
	return *this;
}

Lexer& Lexer::parse(const std::string& code) {
	mCode = code;
	mTokens.clear();
	mTokens.reserve(mCode.size() >> 2);
	clearLogs();

	gatherSize();

	while (mCode.size())
		mTokens.push_back(generateToken());

	mTokens.shrink_to_fit();

	return *this;
}

void Lexer::searchToken() {
	static auto res = sTokenDictionary.end();
	for (auto& sizeElement : mSizeCollection) {
		mPartion = mCode.substr(0, sizeElement);

		res = sTokenDictionary.find(mPartion);

		if (mFoundToken = (res != sTokenDictionary.end()))
			break;
	}

	if (!mFoundToken) {
		writeErr("is weird symbol. It looks like \"" + mCode[0] + '"', mLine);
		mPartion = mPartion[0];
	}

	mCurrentToken = { (!mFoundToken) ? Token::undefinedToken : res->second, mPartion };
}

TokenDescriptor Lexer::generateToken() {
	mCurrentToken = TokenDescriptor();
	mFoundToken = false;
	mPartion = "";

repeat:
	if (isspace(mCode[0]) || mCode[0] == 0) {
		if (markAsPlainCharacter())
			return { Token::empty, "" };
	} else {
		searchToken();

		// because hasn't separation between anything and something
		checkPartionForDigit();
	}

	// but these guys have

	if (mCurrentToken.tok == Token::comment) {
		if (clearComment()) {
			mCurrentToken = { Token::empty, "" };
			return mCurrentToken;
		}

		goto repeat;
	}

	if (mCurrentToken.tok == Token::hash) {
		if (clearInlineComment()) {
			mCurrentToken = { Token::empty, "" };
			return mCurrentToken;
		}

		goto repeat;
	}

	if (mCurrentToken.tok == Token::doubleQuote && mOptionAssembleString)
		mCurrentToken = { Token::string, formString() };

	if (mCurrentToken.tok == Token::space && mOptionClearSpacing)
		goto repeat;

	return mCurrentToken;
}

std::vector<TokenDescriptor>& Lexer::getListTokens() noexcept {
	return mTokens;
}

void Lexer::gatherSize() {
	bool isUnique = true;

	mSizeCollection.clear();
	mSizeCollection.reserve(sTokenDictionary.size() >> 3); // dividing by 8 size of all elements, because many elements could be same height
	mSizeCollection.push_back(1); // that's not excluding

	for (auto& token : sTokenDictionary) {
		for (auto& sizeElement : mSizeCollection) {
			isUnique = sizeElement != token.first.size();
			if (!isUnique)
				break;
		}

		if (isUnique)
			mSizeCollection.push_back(token.first.size());
	}

	std::sort(mSizeCollection.rbegin(), mSizeCollection.rend());
}
