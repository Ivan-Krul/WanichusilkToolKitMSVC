#include <iostream>

#include "FloatVar.h"
#include "Lexer.h"
#include "TreeAssembler.h"
#include "Analyzer.h"

#include "MainConveyor.h"

int main(int args, char** argv)
{
    checkSource(args);

    std::string scriptSrc;

#ifdef FORCED_FILE
    if (loadSource(FORCED_FILEPATH, scriptSrc)) return 1;
#else
    if (loadSource(argv[args - 1], scriptSrc)) return 1;
#endif

    TreeAssembler tree_asm;
    Analyzer analyz;

    setLogWriterOn(tree_asm);
    setLogWriterOn(analyz);

    auto list = doLexing(scriptSrc);

    std::cout << "All tokens:\n";

    for (size_t i = 0; i < list.size(); i++) {
        std::cout << i << ":\t\"" << list[i].val << "\"\n";
    }

    auto tree = tree_asm
        .acceptTokenList(list)
        .parse()
        .getRootNode();

    tree_asm.outputLogs();

    auto chain_list = analyz.acceptRootNode(tree)
        .analyze()
        .getListChain();

    analyz.outputLogs();

    return 0;
}
