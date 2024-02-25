#pragma once
#include <iostream>
#include <string>

class LogMessage {
public:
	inline LogMessage(const std::string& sender, const std::string& message) {
		mSender = sender;
		mMessage = message;
	}

	inline void print() const {
		std::cout << mSender << ": " << mMessage << '\n';
	}

private:
	std::string mMessage;
	std::string mSender;
};

