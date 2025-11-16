#include "scanner.h"
#include "macros.h"

#include <sstream>
#include <stdexcept>

namespace sciche {
scanner::scanner() : m_streamer(NULL)
{
    // clang-format off
    m_reserved = {
        {"true", token::TOKEN_BOOL},
        {"false", token::TOKEN_BOOL},
        {"if", token::TOKEN_IF},
        {"else", token::TOKEN_ELSE},
        {"for", token::TOKEN_FOR},
        {"while", token::TOKEN_WHILE},
        {"class", token::TOKEN_CLASS},
        {"function", token::TOKEN_FUNCTION},
        {"var", token::TOKEN_VAR},
        {"return", token::TOKEN_RETURN},
        {"null", token::TOKEN_NULL},
        {"super", token::TOKEN_SUPER},
        {"this", token::TOKEN_THIS},
        {"in", token::TOKEN_FOR}
    };
    // clang-format on
}

scanner::~scanner()
{
}

void scanner::scanString(void)
{
    m_token.type = token::TOKEN_STRING;
    char ch = 0;
    while (ch = m_streamer->getch(), ch && '"' != ch) {
        m_token.s_val += ch;
    }
}

void scanner::scanSymbol(char ch)
{
    m_token.type = token::TOKEN_SYMBOL;
    m_token.s_val += ch;
    while (ch = m_streamer->getch(), isalpha(ch) || isdigit(ch) || '_' == ch) {
        m_token.s_val += ch;
    }
    auto it = m_reserved.find(m_token.s_val);
    if (it != m_reserved.end())
        m_token.type = it->second;
    if (token::TOKEN_BOOL == m_token.type)
        m_token.b_val = ("true" == m_token.s_val) ? true : false;
}

void scanner::scanHex(void)
{
    m_token.type = token::TOKEN_INTEGER;
    char ch = 0;
    while (ch = m_streamer->getch(), isalpha(ch) || ('A' <= ch && 'F' >= ch) || ('a' <= ch && 'f' >= ch)) {
        m_token.s_val += ch;
    }
}

void scanner::scanOct(void)
{
    m_token.type = token::TOKEN_INTEGER;
    char ch = 0;
    while (ch = m_streamer->getch(), ('0' <= ch && '7' >= ch)) {
        m_token.s_val += ch;
    }
}

void scanner::scanDecimal(char ch)
{
    m_token.type = token::TOKEN_INTEGER;
    m_token.s_val += ch;
    while (ch = m_streamer->getch(), isdigit(ch)) {
        m_token.s_val += ch;
    }
}

void scanner::scanNumber(char ch)
{
    if ('0' == ch) {
        ch = m_streamer->getch();
        if ('x' == ch) {
            scanHex();
            m_token.i_val = std::stol(m_token.s_val, nullptr, 16);
        }
        else if (isdigit(ch)) {
            scanOct();
            m_token.i_val = std::stol(m_token.s_val, nullptr, 8);
        }
        else if ('.' == ch) {
            scanDecimal(ch);
            m_token.type = token::TOKEN_FLOAT;
            m_token.f_val = atof(m_token.s_val.c_str());
        }
        else {
            m_streamer->unch();
            m_token.type = token::TOKEN_INTEGER;
            m_token.s_val = ch;
            m_token.i_val = 0;
        }
    }
    else {
        scanDecimal(ch);
        ch = m_streamer->getch();
        if ('.' == ch) {
            scanDecimal(ch);
            m_token.type = token::TOKEN_FLOAT;
            m_token.f_val = atof(m_token.s_val.c_str());
        }
        else {
            m_streamer->unch();
            m_token.i_val = atoi(m_token.s_val.c_str());
        }
    }
}

void scanner::consumeToEoc(void)
{
    char ch = 0;
    while (ch = m_streamer->getch(), ch) {
        if ('*' == ch) {
            ch = m_streamer->getch();
            if ('/' == ch)
                break;
        }
    }
}

void scanner::consumeToEol(void)
{
    char ch = 0;
    while (ch = m_streamer->getch(), ch) {
        if ('\n' == ch || '\r' == ch) {
            m_streamer->unch();
            break;
        }
    }
}

void scanner::unscan(void)
{
    m_backup.push(m_token);
}

token scanner::scan(void)
{
    if (!m_backup.empty()) {
        m_token = m_backup.top();
        m_backup.pop();
        return m_token;
    }
    m_token.type = token::TOKEN_NONE;
    m_token.reset();
    char ch = 0;
    std::stringstream ss;
    while (token::TOKEN_NONE == m_token.type) {
        ch = m_streamer->getch();
        LOG("ch: %d", (int)ch)
        switch (ch) {
        case '(':
            m_token.type = token::TOKEN_PAREN_L;
            break;
        case ')':
            m_token.type = token::TOKEN_PAREN_R;
            break;
        case '+':
            ch = m_streamer->getch();
            if ('+' == ch) {
                m_token.type = token::TOKEN_INCREMENT;
            }
            else if ('=' == ch) {
                m_token.type = token::TOKEN_ADD_TO;
            }
            else {
                m_streamer->unch();
                m_token.type = token::TOKEN_PLUS;
            }
            break;
        case '-':
            ch = m_streamer->getch();
            if ('-' == ch) {
                m_token.type = token::TOKEN_DECREMENT;
            }
            else if ('=' == ch) {
                m_token.type = token::TOKEN_SUB_TO;
            }
            else {
                m_streamer->unch();
                m_token.type = token::TOKEN_MINUS;
            }
            break;
        case '*':
            ch = m_streamer->getch();
            if ('*' == ch) {
                m_token.type = token::TOKEN_POWER;
            }
            else if ('=' == ch) {
                m_token.type = token::TOKEN_MUL_TO;
            }
            else {
                m_streamer->unch();
                m_token.type = token::TOKEN_STAR;
            }
            break;
        case '/':
            ch = m_streamer->getch();
            if ('=' == ch) {
                m_token.type = token::TOKEN_DIV_TO;
            }
            else if ('/' == ch) {
                consumeToEol();
            }
            else if ('*' == ch) {
                consumeToEoc();
            }
            else {
                m_streamer->unch();
                m_token.type = token::TOKEN_SLASH;
            }
            break;
        case '%':
            ch = m_streamer->getch();
            if ('=' == ch) {
                m_token.type = token::TOKEN_MODULO_TO;
            }
            else {
                m_streamer->unch();
                m_token.type = token::TOKEN_PERCENT;
            }
            break;
        case '<':
            ch = m_streamer->getch();
            if ('=' == ch) {
                m_token.type = token::TOKEN_LE;
            }
            else if ('<' == ch) {
                ch = m_streamer->getch();
                if ('=' == ch) {
                    m_token.type = token::TOKEN_SHIFT_LEFT_TO;
                }
                else {
                    m_streamer->unch();
                    m_token.type = token::TOKEN_SHIFT_LEFT;
                }
            }
            else {
                m_streamer->unch();
                m_token.type = token::TOKEN_LT;
            }
            break;
        case '>':
            ch = m_streamer->getch();
            if ('=' == ch) {
                m_token.type = token::TOKEN_GE;
            }
            else if ('>' == ch) {
                ch = m_streamer->getch();
                if ('=' == ch) {
                    m_token.type = token::TOKEN_SHIFT_RIGHT_TO;
                }
                else {
                    m_streamer->unch();
                    m_token.type = token::TOKEN_SHIFT_RIGHT;
                }
            }
            else {
                m_streamer->unch();
                m_token.type = token::TOKEN_GT;
            }
            break;
        case '!':
            ch = m_streamer->getch();
            if ('=' == ch) {
                m_token.type = token::TOKEN_LOGICAL_NOT_EQ;
            }
            else {
                m_streamer->unch();
                m_token.type = token::TOKEN_EXCLAMATION;
            }
            break;
        case '=':
            ch = m_streamer->getch();
            if ('=' == ch) {
                m_token.type = token::TOKEN_LOGICAL_EQ;
            }
            else {
                m_streamer->unch();
                m_token.type = token::TOKEN_EQ;
            }
            break;
        case '|':
            ch = m_streamer->getch();
            if ('=' == ch) {
                m_token.type = token::TOKEN_OR_TO;
            }
            else if ('|' == ch) {
                m_token.type = token::TOKEN_LOGICAL_OR;
            }
            else {
                m_streamer->unch();
                m_token.type = token::TOKEN_OR;
            }
            break;
        case '&':
            ch = m_streamer->getch();
            if ('=' == ch) {
                m_token.type = token::TOKEN_AND_TO;
            }
            else if ('&' == ch) {
                m_token.type = token::TOKEN_LOGICAL_AND;
            }
            else {
                m_streamer->unch();
                m_token.type = token::TOKEN_AND;
            }
            break;
        case '.':
            m_token.type = token::TOKEN_DOT;
            break;
        case ',':
            m_token.type = token::TOKEN_COMMA;
            break;
        case ':':
            m_token.type = token::TOKEN_COLON;
            break;
        case ';':
            m_token.type = token::TOKEN_SEMI_COLON;
            break;
        case '[':
            m_token.type = token::TOKEN_BRACKET_L;
            break;
        case ']':
            m_token.type = token::TOKEN_BRACKET_R;
            break;
        case '{':
            m_token.type = token::TOKEN_GROOVED_BRACKET_L;
            break;
        case '}':
            m_token.type = token::TOKEN_GROOVED_BRACKET_R;
            break;
        case '\'':
            m_token.type = token::TOKEN_SINGLE_QUOTE;
            break;
        case '"':
            scanString();
            break;
        case '\n':
            /*
            m_token.type = token::TOKEN_NEWLINE;
            ch = m_streamer->getch();
            if ('\r' != ch)
                m_streamer->unch();
            m_line_no++;
            m_line_pos = m_streamer->pos();
            */
            break;
        case '\r':
            /*
            m_token.type = token::TOKEN_NEWLINE;
            ch = m_streamer->getch();
            if ('\n' != ch)
                m_streamer->unch();
            m_line_no++;
            m_line_pos = m_streamer->pos();
            */
            break;
        case 0:
            m_token.type = token::TOKEN_EOF;
            break;
        case ' ':
        case '\t':
            break;
        default:
            if (isalpha(ch) || '_' == ch) {
                scanSymbol(ch);
            }
            else if (isdigit(ch)) {
                scanNumber(ch);
            }
            else {
                ss << "undefined character " << ch;
                ss << "(0x" << (std::hex) << (int)(0xff & ch) << (std::dec) << ")";
                ss << " at " << m_line_no << ":" << m_streamer->pos() - m_line_pos + 1;
                throw std::runtime_error(ss.str());
                break;
            }
        }
    }
    // LOG("line/pos: %lu, %lu", m_line_no, m_streamer->pos() - m_line_pos);
    m_token.line = m_line_no;
    m_token.column = m_streamer->pos() - m_line_pos - m_token.s_val.size() + 1;
    return m_token;
}

void scanner::set_streamer(streamer* streamer)
{
    // streamer* orig = m_streamer;
    m_streamer = streamer;
}

streamer* scanner::get_streamer(void)
{
    return m_streamer;
}
}; // namespace sciche
