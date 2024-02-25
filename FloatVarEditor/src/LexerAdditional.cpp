#include "Lexer.h"


bool Lexer::clearComment() {
	size_t commentElement = mCode.find("//");
	size_t newline = mCode.find("\n");

	while (commentElement > newline) {
		mLine++;
		commentElement -= newline + 1; // all content including newline
		mCode.erase(0, newline + 1);
		newline = mCode.find("\n");
	}

	if (commentElement == mCode.npos && commentElement < newline) {
		mLogMessages.push_back(LogMessage("Lexer", "In " + std::to_string(mLine) + " line comment at the end of the file wasn't closed. Ignore all further!"));
		return true;
	}

	mCode.erase(0, commentElement + 2);
	return false;
}

bool Lexer::clearInlineComment() {
	size_t commentElement = mCode.find("\n");
	if (commentElement == mCode.npos) {
		mCode.erase(0, mCode.size());
		return true;
	}

	mCode.erase(0, commentElement);
	return false;
}

std::string Lexer::formNumber() {
	std::string number;
	size_t digitCounterSize = 1;
	bool isFractional = false;

	while (isdigit(mCode[digitCounterSize]) || (!isFractional && mCode[digitCounterSize] == '.')) {
		if (!isFractional && mCode[digitCounterSize] == '.')
			isFractional = true;
		digitCounterSize++;
	}

	number = mCode.substr(0, digitCounterSize);
	mCode.erase(0, digitCounterSize);

	return number;
}

std::string Lexer::formString() {
	size_t quoteElement = mCode.find("\".");

	if (quoteElement == mCode.npos) {
		mLogMessages.push_back(LogMessage("Lexer", "Hold on, where's the end of string symbol in " + std::to_string(mLine) + " line??? If you don't know it's '\".'"));
		return std::string();
	}

	std::string str = mCode.substr(0, quoteElement);
	mCode.erase(0, quoteElement + 2);

	size_t backSlashElement;
	while ((backSlashElement = str.find('\\')) != str.npos) {
		switch (str[backSlashElement + 1]) {
		case 'a': str[backSlashElement + 1] = '\a';
		case 'b': str[backSlashElement + 1] = '\b';
		case 'r': str[backSlashElement + 1] = '\r';
		case 'f': str[backSlashElement + 1] = '\f';
		case 'v': str[backSlashElement + 1] = '\v';
		case 't': str[backSlashElement + 1] = '\t';
		case 'n': str[backSlashElement + 1] = '\n';
		case '\\':
		default:
			str.erase(backSlashElement, 1);
			break;
		}
	}

	return str;
}

void Lexer::checkPartionForDigit() {
	if (mCurrentToken.tok == Token::digit && mOptionAssembleNumber)
		mCurrentToken = { Token::number,formNumber() };
	else
		mCode.erase(0, mPartion.size());
}

std::vector<size_t> Lexer::countElements(const std::string& str, const std::string& sample) {
	std::vector<size_t> counts;
	const auto elemSize = sample.size();

	// if sample is empty, or sample is bigger, than string
	if (sample.empty() || signed(str.size()) - signed(sample.size()) > 0)
		return counts;

	// select 'o'
	// from [Your eyes shines like rays of sun light]
	//       |^|||||||||||||||||||||||||^|||||||||||
	// 
	// or
	// select 'ro'
	// from [rooster like to roast some chicks, but has been roasted by cooker]
	//       ^-||||||||||||||^-||||||||||||||||||||||||||||||^-||||||||||||||
	for (size_t i = 0; i < str.size() - elemSize + 1; i++) {
		if (str.substr(i, elemSize) == sample) // no convertion to int and compare, due to flexibility
			counts.push_back(i);
	}

	return counts;
}

bool Lexer::markAsPlainCharacter() {
	if (mCode[0] == 0)
		return true;

	mFoundToken = true;

	if (mCode[0] == '\n')
		mLine++;

	mCurrentToken.tok = Token::space;
	mCurrentToken.val = mCode[0];

	mCode.erase(0, 1);
	return false;
}
