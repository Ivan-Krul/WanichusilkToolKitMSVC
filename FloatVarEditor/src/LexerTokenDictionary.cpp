#include "Lexer.h"

decltype(Lexer::sTokenDictionary) Lexer::sTokenDictionary = {
	{".",Token::dot},
	{",",Token::comma},
	{"[",Token::sqrBracket_open},
	{"]",Token::sqrBracket_close},
	{"{",Token::crlBracket_open},
	{"}",Token::crlBracket_close},
	{"(",Token::rndBracket_open},
	{")",Token::rndBracket_close},
	{"=",Token::equal},
	{"+",Token::plus},
	{"-",Token::minus},
	{"*",Token::mult},
	{"/",Token::divide},
	{"//",Token::comment},

	{"edit", Token::var_edit},
	{"read", Token::var_read},
	{"write", Token::var_write},
	{"here", Token::var_here},
	{"this", Token::var_this},
	{"clear", Token::var_clear},

	{"file", Token::var_file},
	{"in", Token::var_file_in},
	{"out", Token::var_file_out},

	{"type", Token::var_type},

	{"name", Token::var_name},

	{"push", Token::var_push},
	{"pop", Token::var_pop},

	{"binary",  Token::var_binary},

	{"toFloat",    Token::convert_toFloat},
	{"toBool",     Token::convert_toBool},
	{"toChar",     Token::convert_toChar},
	{"toShort",    Token::convert_toShort},
	{"toInt",      Token::convert_toInt},
	{"toUnsigned", Token::convert_toUnsignedInt},
	{"toDouble",   Token::convert_toDouble},
	{"toSize",     Token::convert_toSize},
	{"toString",     Token::convert_toString},

	{"size", Token::var_list_size},

	{"true", Token::bool_true},
	{"false", Token::bool_false},

	{"exit", Token::exit},

	{"0", Token::digit},
	{"1", Token::digit},
	{"2", Token::digit},
	{"3", Token::digit},
	{"4", Token::digit},
	{"5", Token::digit},
	{"6", Token::digit},
	{"7", Token::digit},
	{"8", Token::digit},
	{"9", Token::digit},

	{";", Token::newline},
	{":", Token::dotdot},

	{"\"", Token::doubleQuote},
	{"\".",Token::string_end},
	{"'",  Token::quote},

	{"^", Token::power},
	{"&", Token::ampersant},
	{"%", Token::procent},
	{"$", Token::dollar},
	{"|", Token::colomn},
	{"@", Token::atSign},
	{"!", Token::exclamationMark},
	{"#", Token::hash},
	{"~", Token::tilde},

	{"?", Token::help}
};
