//> Scanning on Demand scanner-c
#include <stdio.h>
#include <string.h>

#include "common.hpp"
#include "scanner.hpp"

// Scanner scanner;
//> init-scanner
void Scanner::init(const char* source)
{
    start = source;
    current = source;
    line = 1;
}
//< init-scanner
//> is-alpha
bool Scanner::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
//< is-alpha
//> is-digit
bool Scanner::isDigit(char c)
{
    return c >= '0' && c <= '9';
}
//< is-digit
//> is-at-end
bool Scanner::isAtEnd()
{
    return *current == '\0';
}
//< is-at-end
//> advance
char Scanner::advance()
{
    current++;
    return current[-1];
}
//< advance
//> peek
char Scanner::peek()
{
    return *current;
}
//< peek
//> peek-next
char Scanner::peekNext()
{
    if (isAtEnd())
        return '\0';
    return current[1];
}
//< peek-next
//> match
bool Scanner::match(char expected)
{
    if (isAtEnd())
        return false;
    if (*current != expected)
        return false;
    current++;
    return true;
}
//< match
//> make-token
Token Scanner::makeToken(TokenType type)
{
    Token token;
    token.type = type;
    token.start = start;
    token.length = (int)(current - start);
    token.line = line;
    return token;
}
//< make-token
//> error-token
Token Scanner::errorToken(const char* message)
{
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = line;
    return token;
}
//< error-token
//> skip-whitespace
void Scanner::skipWhitespace()
{
    for (;;) {
        char c = peek();
        switch (c) {
        case ' ':
        case '\r':
        case '\t':
            advance();
            break;
            //> newline
        case '\n':
            line++;
            advance();
            break;
            //< newline
            //> comment
        case '/':
            if (peekNext() == '/') {
                // A comment goes until the end of the line.
                while (peek() != '\n' && !isAtEnd())
                    advance();
            }
            else {
                return;
            }
            break;
            //< comment
        default:
            return;
        }
    }
}
//< skip-whitespace
//> check-keyword
TokenType Scanner::checkKeyword(int istart, int length, const char* rest, TokenType type)
{
    if (current - start == istart + length && memcmp(start + istart, rest, length) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}
//< check-keyword
//> identifier-type
TokenType Scanner::identifierType()
{
    //> keywords
    switch (start[0]) {
    case 'a':
        return checkKeyword(1, 2, "nd", TOKEN_AND);
    case 'c':
        return checkKeyword(1, 4, "lass", TOKEN_CLASS);
    case 'e':
        return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        //> keyword-f
    case 'f':
        if (current - start > 1) {
            switch (start[1]) {
            case 'a':
                return checkKeyword(2, 3, "lse", TOKEN_FALSE);
            case 'o':
                return checkKeyword(2, 1, "r", TOKEN_FOR);
            case 'u':
                return checkKeyword(2, 1, "n", TOKEN_FUN);
            }
        }
        break;
        //< keyword-f
    case 'i':
        return checkKeyword(1, 1, "f", TOKEN_IF);
    case 'n':
        return checkKeyword(1, 2, "il", TOKEN_NIL);
    case 'o':
        return checkKeyword(1, 1, "r", TOKEN_OR);
    case 'p':
        return checkKeyword(1, 4, "rint", TOKEN_PRINT);
    case 'r':
        return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
    case 's':
        return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        //> keyword-t
    case 't':
        if (current - start > 1) {
            switch (start[1]) {
            case 'h':
                return checkKeyword(2, 2, "is", TOKEN_THIS);
            case 'r':
                return checkKeyword(2, 2, "ue", TOKEN_TRUE);
            }
        }
        break;
        //< keyword-t
    case 'v':
        return checkKeyword(1, 2, "ar", TOKEN_VAR);
    case 'w':
        return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }

    //< keywords
    return TOKEN_IDENTIFIER;
}
//< identifier-type
//> identifier
Token Scanner::identifier()
{
    while (isAlpha(peek()) || isDigit(peek()))
        advance();
    return makeToken(identifierType());
}
//< identifier
//> number
Token Scanner::number()
{
    TokenType token_type = TOKEN_NUMBER;
    while (isDigit(peek()))
        advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the ".".
        advance();

        while (isDigit(peek()))
            advance();
    }

    return makeToken(token_type);
}
//< number
//> string
Token Scanner::string()
{
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n')
            line++;
        advance();
    }

    if (isAtEnd())
        return errorToken("Unterminated string.");

    // The closing quote.
    advance();
    return makeToken(TOKEN_STRING);
}
//< string
//> scan-token
Token Scanner::scanToken()
{
    //> call-skip-whitespace
    skipWhitespace();
    //< call-skip-whitespace
    start = current;

    if (isAtEnd())
        return makeToken(TOKEN_EOF);
    //> scan-char

    char c = advance();
    //> scan-identifier
    if (isAlpha(c))
        return identifier();
    //< scan-identifier
    //> scan-number
    if (isDigit(c))
        return number();
    //< scan-number

    switch (c) {
    case '(':
        return makeToken(TOKEN_LEFT_PAREN);
    case ')':
        return makeToken(TOKEN_RIGHT_PAREN);
    case '{':
        return makeToken(TOKEN_LEFT_BRACE);
    case '}':
        return makeToken(TOKEN_RIGHT_BRACE);
    case ';':
        return makeToken(TOKEN_SEMICOLON);
    case ',':
        return makeToken(TOKEN_COMMA);
    case '.':
        return makeToken(TOKEN_DOT);
    case '-':
        return makeToken(TOKEN_MINUS);
    case '+':
        return makeToken(TOKEN_PLUS);
    case '/':
        return makeToken(TOKEN_SLASH);
    case '*':
        return makeToken(TOKEN_STAR);
        //> two-char
    case '!':
        return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
        return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
        return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
        return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        //< two-char
        //> scan-string
    case '"':
        return string();
        //< scan-string
    }
    //< scan-char

    return errorToken("Unexpected character.");
}
//< scan-token
