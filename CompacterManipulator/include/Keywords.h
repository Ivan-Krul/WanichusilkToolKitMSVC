#pragma once
#include <unordered_map>
#include <string>

#include "Hardware.h"

enum class Keyword {
    NONE,
    EXIT,

    OPEN,
    SAVE,

    CREATE,
    OUTPUT,
    WRITE,
    DELETE,
    LIST,
    MERGE,
    DIVIDE,

    CLEAR,
    CLEAR_SCREEN,

    HELP,

    first = EXIT,
    last = HELP
};

constexpr char gBannedFilepathChar = '$';
constexpr char gBannedCompacterName = '~';
constexpr const char* gAgreement = "yes";
constexpr char gArrayMarker = '@';

const std::unordered_map<std::string, Keyword> gKeywords = {
    {"exit", Keyword::EXIT},
    {"quit", Keyword::EXIT},

    {"open", Keyword::OPEN},
    {"load", Keyword::OPEN},
    {"cat", Keyword::OPEN},

    {"save", Keyword::SAVE},
    {"touch", Keyword::SAVE},

    {"create", Keyword::CREATE},
    {"touch", Keyword::CREATE},

    {"output", Keyword::OUTPUT},
    {"print", Keyword::OUTPUT},
    {"echo", Keyword::OUTPUT},
    {"read", Keyword::OUTPUT},

    {"clear_screen", Keyword::CLEAR_SCREEN},
    {"cls", Keyword::CLEAR_SCREEN},
    {"flush", Keyword::CLEAR_SCREEN},

    {"merge", Keyword::MERGE},
    {"glue", Keyword::MERGE},

    {"divide", Keyword::DIVIDE},
    {"crop", Keyword::DIVIDE},

    {"list", Keyword::LIST},
    {"watch", Keyword::LIST},

    {"write", Keyword::WRITE},
    {"set", Keyword::WRITE},

    {"delete", Keyword::DELETE},
    {"del", Keyword::DELETE},
    {"rm", Keyword::DELETE},

    {"clear", Keyword::CLEAR},
    {"reset", Keyword::CLEAR},

    {"help", Keyword::HELP},
    {"huh?", Keyword::HELP},
    {"?", Keyword::HELP},
};

Keyword convertToKeyword(std::string str) {
    for (auto& ch : str)
        ch = tolower(ch);
    
    Keyword kw;
    try {
        kw = gKeywords.at(str);
        return kw;
    }
    catch (...) {
        return Keyword::NONE;
    }
}
