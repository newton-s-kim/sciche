#include "token.h"

#include <iostream>
#include <sstream>
#include <vector>

namespace sciche {
token::token() : line(0), column(0)
{
    reset();
}

void token::reset(void)
{
    f_val = 0;
    i_val = 0;
    s_val.clear();
    b_val = false;
    type = TOKEN_NONE;
}

std::string token::to_string(void) const
{
    static std::vector<std::string> type_name = {"TOKEN_NONE",
                                                 "TOKEN_INTEGER",
                                                 "TOKEN_FLOAT",
                                                 "TOKEN_BOOL",
                                                 "TOKEN_STRING",
                                                 "TOKEN_SYMBOL",
                                                 "TOKEN_PAREN_L",
                                                 "TOKEN_PAREN_R",
                                                 "TOKEN_BRACKET_L",
                                                 "TOKEN_BRACKET_R",
                                                 "TOKEN_GROOVED_BRACKET_L",
                                                 "TOKEN_GROOVED_BRACKET_R",
                                                 "TOKEN_PLUS",
                                                 "TOKEN_ADD_TO",
                                                 "TOKEN_MINUS",
                                                 "TOKEN_SUB_TO",
                                                 "TOKEN_STAR",
                                                 "TOKEN_MUL_TO",
                                                 "TOKEN_POWER",
                                                 "TOKEN_SLASH",
                                                 "TOKEN_DIV_TO",
                                                 "TOKEN_PERCENT",
                                                 "TOKEN_MODULO_TO",
                                                 "TOKEN_INCREMENT",
                                                 "TOKEN_DECREMENT",
                                                 "TOKEN_SHIFT_LEFT",
                                                 "TOKEN_SHIFT_LEFT_TO",
                                                 "TOKEN_SHIFT_RIGHT",
                                                 "TOKEN_SHIFT_RIGHT_TO",
                                                 "TOKEN_EXCLAMATION",
                                                 "TOKEN_EQ",
                                                 "TOKEN_LOGICAL_EQ",
                                                 "TOKEN_LOGICAL_NOT_EQ",
                                                 "TOKEN_AND",
                                                 "TOKEN_AND_TO",
                                                 "TOKEN_OR",
                                                 "TOKEN_OR_TO",
                                                 "TOKEN_LOGICAL_AND",
                                                 "TOKEN_LOGICAL_OR",
                                                 "TOKEN_LT",
                                                 "TOKEN_GT",
                                                 "TOKEN_LE",
                                                 "TOKEN_GE",
                                                 "TOKEN_IF",
                                                 "TOKEN_ELSE",
                                                 "TOKEN_FOR",
                                                 "TOKEN_WHILE",
                                                 "TOKEN_CLASS",
                                                 "TOKEN_FUNCTION",
                                                 "TOKEN_VAR",
                                                 "TOKEN_RETURN",
                                                 "TOKEN_NULL",
                                                 "TOKEN_SUPER",
                                                 "TOKEN_THIS",
                                                 "TOKEN_DOT",
                                                 "TOKEN_COMMA",
                                                 "TOKEN_COLON",
                                                 "TOKEN_SEMI_COLON",
                                                 "TOKEN_SINGLE_QUOTE",
                                                 "TOKEN_NEWLINE",
                                                 "TOKEN_EOF",
                                                 "TOKEN_MAX"};
    std::stringstream ss;
    ss << type_name[type] << "/" << s_val << "(" << line << ":" << column << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const token& token)
{
    os << token.to_string();
    return os;
}
}; // namespace sciche
