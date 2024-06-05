#include <iostream>

#include "FloatVar.h"
#include "Lexer.h"
#include "TreeAssembler.h"
#include "Analyzer.h"

#include "MainConveyor.h"

#define GET_CODE_PARTION(Variable) (#Variable)

int main(int args, char** argv)
{
    checkSource(args);

    std::string scriptSrc;

#ifdef FORCED_FILE
    if (loadSource(FORCED_FILEPATH, scriptSrc)) return 1;
#else
    if (loadSource(argv[args - 1], scriptSrc)) return 1;
#endif

    auto list = doLexing(scriptSrc);

    std::cout << "All tokens:\n";
    for (size_t i = 0; i < list.size(); i++) {
        std::cout << i << ":\t\"" << list[i].val << "\"\n";
    }

    auto tree = doTreeAssembling(list);
    auto chain = doAnalyzing(tree);

    std::cout << "All chains:\n";
    for (size_t i = 0; i < chain.size(); i++) {
        std::cout << i << ":\t\"" << chain[i].size() << "\"\n";
        for (size_t c = 0; c < chain[i].size(); c++) {
            std::cout <<'\t' << c << ":\t\"" << (int)chain[i][c]->getType() << "\"\n";
        }
    }

    return 0;
}
