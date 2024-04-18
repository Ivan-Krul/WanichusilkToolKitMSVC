#pragma once
#include <fstream>
#include <iostream>
#include <string>

#include "LogWriter.h"

#ifdef _DEBUG
#include "DebugSettings.h"
#endif // _DEBUG

int loadSource(const char* argv, std::string& str) {
    std::ifstream fin(argv);
    if (!fin.is_open()) {
        std::cout << "ERROR: The file isn't opening as \"" << argv << "\"\n\a";
        return 1;
    }

    if (fin.bad()) {
        std::cout << "ERROR: Bad file stream\n\a";
        return 1;
    }

    std::getline(fin, str, '\0');
    fin.close();

    return 0;
}

#ifdef FORCED_FILE
int checkSource(int args) { return 0; }
#else
int checkSource(int args) {
    if (args < 2) {
        std::cout << "ERROR: No source file\n\a";
        return 1;
    }

    return 0;
}
#endif

void setLogWriterOn(LogWriter& logWriter) {
#ifndef FORCED_DEBUG
    if (args < 3 && argv[2] == "-d")
#endif
    {
        logWriter.setOptionOutputLogs();
    }
}

std::vector<TokenDescriptor> doLexing(const std::string& scriptSrc) {
    Lexer lexer;
    setLogWriterOn(lexer);

    lexer.setOptionClearSpacing()
        .setOptionAssembleString()
        .setOptionAssembleNumber()
        .parse(scriptSrc);

    lexer.outputLogs();

    return lexer.getListTokens();
}
