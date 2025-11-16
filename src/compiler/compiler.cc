#include "compiler.h"

#include "macros.h"

#include <sstream>

/*
programme      : declaration* EOF

declaration    : classDecl
               | funDecl
               | statement

classDecl      : "class" IDENTIFIER ( ":" IDENTIFIER )? "{" function* "}"
funDecl        : "function" function

statement      : exprStmt
               | forStmt
               | ifStmt
               | returnStmt
               | whileStmt
               | block

exprStmt       : expression ;
forStmt        : "for" "(" IDENTIFIER ":" expression ")" statement
ifStmt         : "if" "(" expression ")" statement ( "else" statement )?
returnStmt     : "return" expression?
whileStmt      : "while" "(" expression ")" statement
block          : "{" declaration* "}"

expression     : assignment ;

assignment     : ( call "." )? IDENTIFIER "=" assignment
               | logic_or ;

logic_or       : logic_and ( "||" logic_and )*
logic_and      : equality ( "&&" equality )*
equality       : comparison ( ( "!=" | "==" ) comparison )*
comparison     : term ( ( ">" | ">=" | "<" | "<=" ) term )*
term           : factor ( ( "-" | "+" ) factor )*
factor         : unary ( ( "/" | "*" ) unary )*

unary          : ( "!" | "-" ) unary | call
call           : primary ( "(" arguments? ")" | "." IDENTIFIER )*
primary        : "true" | "false" | "nil" | "this"
               | NUMBER | STRING | IDENTIFIER | "(" expression ")"
               | "super" "." IDENTIFIER

function       : IDENTIFIER "(" parameters? ")" block
parameters     : IDENTIFIER ( "," IDENTIFIER )*
arguments      : expression ( "," expression )*

NUMBER         : DIGIT+ ( "." DIGIT+ )?
STRING         : "\"" <any char except "\"">* "\""
IDENTIFIER     : ALPHA ( ALPHA | DIGIT )*
ALPHA          : "a" ... "z" | "A" ... "Z" | "_"
DIGIT          : "0" ... "9" ;
*/

#define PROMPT_IDLE ">> "
#define PROMPT_INCOMPLETE ">>> "

namespace sciche {
compiler::compiler(virtual_machine* vm) : m_vm(vm), m_eof(false)
{
}

compiler::~compiler()
{
}

void compiler::parseExpr(void)
{
    parseAssignExpr();
}

void compiler::parsePrimaryExpr(void)
{
    token token = m_scanner.scan();
    std::stringstream ss;
    LOG("%s", token.to_string().c_str());
    switch (token.type) {
    case token::TOKEN_NULL:
        break;
    case token::TOKEN_INTEGER:
        break;
    case token::TOKEN_FLOAT:
        break;
    case token::TOKEN_BOOL:
        break;
    case token::TOKEN_SYMBOL:
        break;
    case token::TOKEN_STRING:
        break;
    case token::TOKEN_PAREN_L:
        parseExpr();
        token = m_scanner.scan();
        if (token::TOKEN_PAREN_R != token.type) {
            ss << "expected ) but got" << token;
            LOG("%s", ss.str().c_str());
            throw std::runtime_error(ss.str());
        }
        break;
    default:
        ss << "unexpected token " << token;
        LOG("%s", ss.str().c_str());
        throw std::runtime_error(ss.str());
        break;
    }
}

void compiler::parseAssignExpr(void)
{
    parseLogicalOrExpr();
    token token = m_scanner.scan();
    if (token::TOKEN_EQ == token.type) {
        parseAssignExpr();
    }
    else {
        m_scanner.unscan();
    }
}

void compiler::parseLogicalOrExpr(void)
{
    parseLogicalAndExpr();
    token token = m_scanner.scan();
    if (token::TOKEN_LOGICAL_OR == token.type) {
        parseLogicalOrExpr();
    }
    else {
        m_scanner.unscan();
    }
}

void compiler::parseLogicalAndExpr(void)
{
    parseEqualityExpr();
    token token = m_scanner.scan();
    if (token::TOKEN_LOGICAL_AND == token.type) {
        parseLogicalAndExpr();
    }
    else {
        m_scanner.unscan();
    }
}

void compiler::parseEqualityExpr(void)
{
    parseComparisonExpr();
    token token = m_scanner.scan();
    if (token::TOKEN_LOGICAL_EQ == token.type) {
        parseComparisonExpr();
    }
    else if (token::TOKEN_LOGICAL_NOT_EQ == token.type) {
        parseComparisonExpr();
    }
    else {
        m_scanner.unscan();
    }
}

void compiler::parseComparisonExpr(void)
{
    parseTermExpr();
    token token = m_scanner.scan();
    if (token::TOKEN_GT == token.type) {
        parseTermExpr();
    }
    else if (token::TOKEN_GE == token.type) {
        parseTermExpr();
    }
    else if (token::TOKEN_LT == token.type) {
        parseTermExpr();
    }
    else if (token::TOKEN_LE == token.type) {
        parseTermExpr();
    }
    else {
        m_scanner.unscan();
    }
}

void compiler::parseTermExpr(void)
{
    parseFactorExpr();
    token token = m_scanner.scan();
    if (token::TOKEN_PLUS == token.type) {
        parseTermExpr();
    }
    else if (token::TOKEN_MINUS == token.type) {
        parseTermExpr();
    }
    else {
        m_scanner.unscan();
    }
}

void compiler::parseFactorExpr(void)
{
    parseUnaryExpr();
    token token = m_scanner.scan();
    LOG("%s", token.to_string().c_str());
    if (token::TOKEN_STAR == token.type) {
        parseFactorExpr();
    }
    else if (token::TOKEN_SLASH == token.type) {
        parseFactorExpr();
    }
    else {
        m_scanner.unscan();
    }
}

void compiler::parseUnaryExpr(void)
{
    token token = m_scanner.scan();
    if (token::TOKEN_MINUS == token.type) {
        parseUnaryExpr();
    }
    else if (token::TOKEN_EXCLAMATION == token.type) {
        parseUnaryExpr();
    }
    else {
        m_scanner.unscan();
        parseFuncCallExpr();
    }
}

void compiler::parseFuncArgsExpr(void)
{
    LOG("entered");
    token token = m_scanner.scan();
    if (token::TOKEN_PAREN_R != token.type) {
        m_scanner.unscan();
        while (true) {
            parseExpr();
            token = m_scanner.scan();
            if (token::TOKEN_PAREN_R == token.type) {
                m_scanner.unscan();
                break;
            }
            else if (token::TOKEN_COMMA != token.type) {
                std::stringstream ss;
                ss << "expected ) or , but got " << token;
                LOG("%s", ss.str().c_str());
                throw std::runtime_error(ss.str());
            }
        };
    }
    else {
        m_scanner.unscan();
    }
}

void compiler::parseFuncCallExpr(void)
{
    parsePrimaryExpr();
    while (true) {
        token token = m_scanner.scan();
        if (token::TOKEN_PAREN_L == token.type) {
            parseFuncArgsExpr();
            token = m_scanner.scan();
            if (token::TOKEN_PAREN_R != token.type) {
                std::stringstream ss;
                ss << "expected ) but got " << token;
                LOG("%s", ss.str().c_str());
                throw std::runtime_error(ss.str());
            }
        }
        else if (token::TOKEN_DOT == token.type) {
            parsePrimaryExpr();
        }
        else if (token::TOKEN_BRACKET_L == token.type) {
            parsePrimaryExpr();
        }
        else {
            m_scanner.unscan();
            break;
        }
    }
}

void compiler::parseIfStmt(void)
{
    std::stringstream ss;
    token token = m_scanner.scan();
    if (token::TOKEN_PAREN_L != token.type) {
        ss << "( was expected but got " << token;
        LOG("%s", ss.str().c_str());
        throw std::runtime_error(ss.str());
    }
    parseExpr();
    token = m_scanner.scan();
    if (token::TOKEN_PAREN_R != token.type) {
        ss << ") was expected but got " << token;
        LOG("%s", ss.str().c_str());
        throw std::runtime_error(ss.str());
    }
    parseStmt();
    token = m_scanner.scan();
    if (token::TOKEN_ELSE == token.type) {
        parseStmt();
    }
    else {
        m_scanner.unscan();
    }
}

void compiler::parseFuncParamsExpr(void)
{
    token token = m_scanner.scan();
    if (token::TOKEN_PAREN_R != token.type) {
        m_scanner.unscan();
        while (true) {
            parsePrimaryExpr();
            token = m_scanner.scan();
            if (token::TOKEN_PAREN_R == token.type) {
                m_scanner.unscan();
                break;
            }
            else if (token::TOKEN_COMMA != token.type) {
                std::stringstream ss;
                ss << "expected ) or , but got " << token;
                LOG("%s", ss.str().c_str());
                throw std::runtime_error(ss.str());
            }
        };
    }
    else {
        m_scanner.unscan();
    }
}

void compiler::parseClassDeclStmt(void)
{
    parsePrimaryExpr();
    token token = m_scanner.scan();
    if (token::TOKEN_COLON == token.type) {
        parsePrimaryExpr();
        token = m_scanner.scan();
    }
    if (token::TOKEN_GROOVED_BRACKET_L != token.type) {
        std::stringstream ss;
        ss << "{ is expected but got " << token;
        LOG("%s", ss.str().c_str());
        throw std::runtime_error(ss.str());
    }
    while (true) {
        if (token::TOKEN_FUNCTION == token.type) {
            parseFuncDeclStmt();
        }
        else {
            break;
        }
    }
    if (token::TOKEN_PAREN_R != token.type) {
        std::stringstream ss;
        ss << "expected ) but got " << token;
        LOG("%s", ss.str().c_str());
        throw std::runtime_error(ss.str());
    }
}

void compiler::parseFuncDeclStmt(void)
{
    parsePrimaryExpr();
    token token = m_scanner.scan();
    if (token::TOKEN_PAREN_L != token.type) {
        std::stringstream ss;
        ss << "( is expected but got " << token;
        LOG("%s", ss.str().c_str());
        throw std::runtime_error(ss.str());
    }
    parseFuncParamsExpr();
    token = m_scanner.scan();
    if (token::TOKEN_PAREN_R != token.type) {
        std::stringstream ss;
        ss << "expected ) but got " << token;
        LOG("%s", ss.str().c_str());
        throw std::runtime_error(ss.str());
    }

    parseBlockStmt();
}

void compiler::parseReturnStmt(void)
{
    parseExpr();
}

void compiler::parseBlockStmt(void)
{
    token token = m_scanner.scan();
    if (token::TOKEN_GROOVED_BRACKET_L != token.type) {
        std::stringstream ss;
        ss << "expected { but got" << token;
        LOG("%s", ss.str().c_str());
        throw std::runtime_error(ss.str());
    }
    while (token = m_scanner.scan(), token::TOKEN_GROOVED_BRACKET_R != token.type) {
        m_scanner.unscan();
        parseStmt();
    }
}

void compiler::parseForStmt(void)
{
}

void compiler::parseWhileStmt(void)
{
    token token = m_scanner.scan();
    if (token::TOKEN_PAREN_L != token.type) {
        std::stringstream ss;
        ss << "expected ( but got" << token;
        LOG("%s", ss.str().c_str());
        throw std::runtime_error(ss.str());
    }
    parseExpr();
    token = m_scanner.scan();
    if (token::TOKEN_PAREN_R != token.type) {
        std::stringstream ss;
        ss << "expected ) but got" << token;
        LOG("%s", ss.str().c_str());
        throw std::runtime_error(ss.str());
    }
    parseStmt();
}

void compiler::parseStmt(void)
{
    token token = m_scanner.scan();
    LOG("token: %s", token.to_string().c_str());
    if (token::TOKEN_IF == token.type) {
        parseIfStmt();
    }
    else if (token::TOKEN_RETURN == token.type) {
        parseReturnStmt();
    }
    else if (token::TOKEN_FOR == token.type) {
        parseForStmt();
    }
    else if (token::TOKEN_WHILE == token.type) {
        parseWhileStmt();
    }
    else if (token::TOKEN_EOF == token.type) {
        m_eof = true;
    }
    else if (token::TOKEN_GROOVED_BRACKET_L == token.type) {
        m_scanner.unscan();
        parseBlockStmt();
    }
    else {
        m_scanner.unscan();
        parseExpr();
    }
    LOG("leave")
}

void compiler::parseDecl(void)
{
    LOG("enter")
    token token = m_scanner.scan();
    m_scanner.get_streamer()->set_prompt(PROMPT_INCOMPLETE);
    if (token::TOKEN_CLASS == token.type) {
        parseClassDeclStmt();
    }
    else if (token::TOKEN_FUNCTION == token.type) {
        parseFuncDeclStmt();
    }
    else {
        m_scanner.unscan();
        parseStmt();
    }
    LOG("leave")
}

void compiler::compile(streamer* streamer)
{
    m_eof = false;
    m_scanner.set_streamer(streamer);
    while (!m_eof) {
        streamer->set_prompt(PROMPT_IDLE);
        parseDecl();
        LOG("m_eof: %s", ((m_eof) ? "true" : "false"));
    }
}

}; // namespace sciche
