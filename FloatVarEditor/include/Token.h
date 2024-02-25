#pragma once
#include <string>

enum class Token : char {
	empty,
	// main interaction block
	var_edit,
	var_read,
	var_clear,

	var_type,
	var_name,
	var_binary,

	var_write,

	var_push,
	var_pop,

	var_here,
	var_this,
	var_file,
	var_file_in,
	var_file_out,

	convert_toBool,
	convert_toChar,
	convert_toShort,
	convert_toInt,
	convert_toUnsignedInt,
	convert_toFloat,
	convert_toDouble,
	convert_toSize,

	bool_true,
	bool_false,

	var_list,
	var_list_size,

	exit,

	// additional syntax
	sqrBracket_open,
	sqrBracket_close,

	rndBracket_open,
	rndBracket_close,

	crlBracket_open,
	crlBracket_close,

	digit,
	number,

	comma,
	dot,

	equal,
	plus,
	minus,
	mult,
	divide,

	power,
	ampersant,
	procent,
	dollar,
	colomn,
	atSign,
	exclamationMark,
	hash,
	tilde,

	newline,
	dotdot,
	doubleQuote,
	quote,
	string,
	string_end,

	space,

	comment,

	undefinedToken,

	// and the last but not least
	help
};

struct TokenDescriptor {
	Token tok;
	std::string val;
};