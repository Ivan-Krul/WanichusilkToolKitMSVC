#include <iostream>
#include <string>

#include "Hardware.h"

#include "../include/Action.h"

int main(int argc, char* argv[]) {
    if (argc > 1) {
        if (gHardware.canBeFileOpen(argv[1])) {
            gHardware.read(argv[1]);
            std::cout << "Parsed: " << argv[1] << '\n';
        }
        else {
            std::cout << "File wasn't opened\n";
        }
    }

    std::string input;
    auto keyword = Keyword::NONE;

    std::cout << "CompacterManipulator\n";
    std::cout << "Type \"help\" to get started\n";

    while (!gNeedExit) {
        std::cout << "\nCompacterManipulator: ";
        std::cin >> input;
        //std::getline(std::cin >> std::ws, input);

        if (input.empty())
            continue;

        keyword = convertToKeyword(input);
        chooseAction(keyword);
    }
}
