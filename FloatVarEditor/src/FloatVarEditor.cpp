#include <iostream>

#include "FloatVar.h"
#include "Lexer.h"
#include "TreeAssembler.h"
#include "Executor.h"

#include <fstream>

#define FORCED_FILE
#define FORCED_FILEPATH "examples/#1 hello_world.fvs"

#define FORCED_DEBUG

int loadSource(const char* argv, std::string& str);


int main(int args, char** argv)
{
#ifndef FORCED_FILE
    if (args < 2) {
        std::cout << "ERROR: No source file\n\a";
        return 1;
    }
#endif

    std::string scriptSrc;

#ifdef FORCED_FILE
    if (loadSource(FORCED_FILEPATH, scriptSrc)) return 1;
#else
    if (loadSource(argv[1], scriptSrc)) return 1;
#endif

    Lexer lexer;
    TreeAssembler tree_asm;

#ifndef FORCED_DEBUG
    if (args < 3 && argv[2] == "-d")
#endif
    {
        lexer.setOptionOutputLogs();
        tree_asm.setOptionOutputLogs();
    }

    auto list = lexer
        .setOptionClearSpacing()
        .setOptionAssembleString()
        .setOptionAssembleNumber()
        .parse(scriptSrc)
        .outputLogs()
        .getListTokens();

    std::cout << "All tokens:\n";

    for (size_t i = 0; i < list.size(); i++) {
        std::cout << i << ":\t\"" << list[i].val << "\"\n";
    }

    auto tree = tree_asm
        .acceptTokenList(list)
        .parse()
        .outputLogs()
        .getRootNode();

    return 0;
}



int loadSource(const char* argv, std::string& str) {
    std::ifstream fin(argv);
    if (!fin.is_open()) {
        std::cout << "ERROR: The file isn't opening as \"" << argv << "\"\n\a";
        return 1;
    }

    if(fin.bad()) {
        std::cout << "ERROR: Bad file stream\n\a";
        return 1;
    }

    std::getline(fin, str, '\0');
    fin.close();

    return 0;
}
