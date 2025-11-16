#pragma once

#include <string>

namespace sciche {
class token {
public:
    typedef enum {
        TOKEN_NONE = 0,
        TOKEN_INTEGER,
        TOKEN_FLOAT,
        TOKEN_BOOL,
        TOKEN_STRING,
        TOKEN_SYMBOL,
        TOKEN_PAREN_L,
        TOKEN_PAREN_R,
        TOKEN_BRACKET_L,
        TOKEN_BRACKET_R,
        TOKEN_GROOVED_BRACKET_L,
        TOKEN_GROOVED_BRACKET_R,
        TOKEN_PLUS,
        TOKEN_ADD_TO,
        TOKEN_MINUS,
        TOKEN_SUB_TO,
        TOKEN_STAR,
        TOKEN_MUL_TO,
        TOKEN_POWER,
        TOKEN_SLASH,
        TOKEN_DIV_TO,
        TOKEN_PERCENT,
        TOKEN_MODULO_TO,
        TOKEN_INCREMENT,
        TOKEN_DECREMENT,
        TOKEN_SHIFT_LEFT,
        TOKEN_SHIFT_LEFT_TO,
        TOKEN_SHIFT_RIGHT,
        TOKEN_SHIFT_RIGHT_TO,
        TOKEN_EXCLAMATION,
        TOKEN_EQ,
        TOKEN_LOGICAL_EQ,
        TOKEN_LOGICAL_NOT_EQ,
        TOKEN_AND,
        TOKEN_AND_TO,
        TOKEN_OR,
        TOKEN_OR_TO,
        TOKEN_LOGICAL_AND,
        TOKEN_LOGICAL_OR,
        TOKEN_LT,
        TOKEN_GT,
        TOKEN_LE,
        TOKEN_GE,
        TOKEN_IF,
        TOKEN_ELSE,
        TOKEN_FOR,
        TOKEN_WHILE,
        TOKEN_CLASS,
        TOKEN_FUNCTION,
        TOKEN_VAR,
        TOKEN_RETURN,
        TOKEN_NULL,
        TOKEN_SUPER,
        TOKEN_THIS,
        TOKEN_DOT,
        TOKEN_COMMA,
        TOKEN_COLON,
        TOKEN_SEMI_COLON,
        TOKEN_SINGLE_QUOTE,
        TOKEN_NEWLINE,
        TOKEN_EOF,
        TOKEN_MAX
    } tokenType;
    tokenType type;
    std::string s_val;
    int i_val;
    double f_val;
    bool b_val;

    size_t line;
    size_t column;

    token();
    void reset(void);
    std::string to_string(void) const;
};
std::ostream& operator<<(std::ostream& os, const token& token);
}; // namespace sciche
