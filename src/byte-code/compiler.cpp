//> Scanning on Demand compiler-c
#include <stdio.h>
//> Compiling Expressions compiler-include-stdlib
#include <stdlib.h>
//< Compiling Expressions compiler-include-stdlib
//> Local Variables compiler-include-string
#include <string.h>
//< Local Variables compiler-include-string

#include "common.hpp"
#include "compiler.hpp"
#include "scanner.hpp"
//> Compiling Expressions include-debug

#ifdef DEBUG_PRINT_CODE
#include "debug.hpp"
#endif
//< Compiling Expressions include-debug
//> Compiling Expressions parser

// extern Scanner scanner;

class Parser {
public:
    Scanner scanner;
    Token current;
    Token previous;
    //> had-error-field
    bool hadError;
    //< had-error-field
    //> panic-mode-field
    bool panicMode;
    //< panic-mode-field

    void errorAt(Token* token, const char* message);
    void error(const char* message);
    void errorAtCurrent(const char* message);
    void advance();
    void consume(TokenType type, const char* message);
    bool check(TokenType type);
    bool match(TokenType type);
};
//> parse-fn-type

//< parse-fn-type
/* Compiling Expressions parse-fn-type < Global Variables parse-fn-type
typedef void (*ParseFn)();
*/
//> Local Variables local-struct

class Local {
public:
    Token name;
    int depth;
    //> Closures is-captured-field
    bool isCaptured;
    //< Closures is-captured-field
};
//< Local Variables local-struct
//> Closures upvalue-struct
class Upvalue {
public:
    uint8_t index;
    bool isLocal;
};
//< Closures upvalue-struct
//> Calls and Functions function-type-enum
typedef enum {
    TYPE_FUNCTION,
    //> Methods and Initializers initializer-type-enum
    TYPE_INITIALIZER,
    //< Methods and Initializers initializer-type-enum
    //> Methods and Initializers method-type-enum
    TYPE_METHOD,
    //< Methods and Initializers method-type-enum
    TYPE_SCRIPT
} FunctionType;
//< Calls and Functions function-type-enum
//> Local Variables compiler-struct

//> precedence

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT, // =
    PREC_OR,         // or
    PREC_AND,        // and
    PREC_EQUALITY,   // == !=
    PREC_COMPARISON, // < > <= >=
    PREC_TERM,       // + -
    PREC_FACTOR,     // * /
    PREC_UNARY,      // ! -
    PREC_CALL,       // . ()
    PREC_PRIMARY
} Precedence;
//< precedence
class CompilerInterfaceConcrete;
//> Global Variables parse-fn-type
typedef void (*ParseFn)(bool canAssign, CompilerInterfaceConcrete* ci);
//< Global Variables parse-fn-type
//> parse-rule

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;
//< parse-rule

/* Local Variables compiler-struct < Calls and Functions enclosing-field
typedef struct {
*/
//> Calls and Functions enclosing-field
class Compiler {
public:
    struct Compiler* enclosing;
    //< Calls and Functions enclosing-field
    //> Calls and Functions function-fields
    ObjFunction* function;
    FunctionType type;

    //< Calls and Functions function-fields
    Local locals[UINT8_COUNT];
    int localCount;
    //> Closures upvalues-array
    Upvalue upvalues[UINT8_COUNT];
    //< Closures upvalues-array
    int scopeDepth;

    Chunk* chunk();
    void markInitialized();
    Token syntheticToken(const char* text);
    bool identifiersEqual(Token* a, Token* b);
};
//< Local Variables compiler-struct
//> Methods and Initializers class-compiler-struct

class ClassCompiler {
public:
    struct ClassCompiler* enclosing;
    //> Superclasses has-superclass
    bool hasSuperclass;
    //< Superclasses has-superclass
};
//< Methods and Initializers class-compiler-struct

//> Compiling Expressions compiling-chunk
/* Compiling Expressions compiling-chunk < Calls and Functions current-chunk
Chunk* compilingChunk;

static Chunk* currentChunk() {
  return compilingChunk;
}
*/

class CompilerInterfaceConcrete : public CompilerInterface {
private:
    friend void super_(bool canAssign, CompilerInterfaceConcrete* ci);
    friend void this_(bool canAssign, CompilerInterfaceConcrete* ci);
    friend void variable(bool canAssign, CompilerInterfaceConcrete* ci);
    friend void grouping(bool canAssign, CompilerInterfaceConcrete* ci);
    friend void dot(bool canAssign, CompilerInterfaceConcrete* ci);
    friend void call(bool canAssign, CompilerInterfaceConcrete* ci);
    friend void and_(bool canAssign, CompilerInterfaceConcrete* ci);
    friend void binary(bool canAssign, CompilerInterfaceConcrete* ci);
    friend void or_(bool canAssign, CompilerInterfaceConcrete* ci);
    friend void unary(bool canAssign, CompilerInterfaceConcrete* ci);
    friend void number(bool canAssign, CompilerInterfaceConcrete* ci);
    friend void string(bool canAssign, CompilerInterfaceConcrete* ci);
    friend void literal(bool canAssign, CompilerInterfaceConcrete* ci);

    Parser parser;
    //< Compiling Expressions parser
    //> Local Variables current-compiler
    Compiler* current = NULL;
    //< Local Variables current-compiler
    //> Methods and Initializers current-class
    ClassCompiler* currentClass = NULL;
    //< Methods and Initializers current-class
#ifdef DEBUG_PRINT_CODE
    Debug debug;
#endif
    ObjectFactory* factory;
    void block();
    void declaration();
    void function(FunctionType type);
    void funDeclaration();
    void method();
    void classDeclaration();
    void varDeclaration();
    void forStatement();
    void statement();
    void ifStatement();
    void whileStatement();
    void initCompiler(Compiler* compiler, FunctionType type);
    ObjFunction* endCompiler();
    void printStatement();
    void returnStatement();
    void synchronize();
    void expressionStatement();
    void expression();
    void namedVariable(Token name, bool canAssign);
    uint8_t argumentList();
    void parsePrecedence(Precedence precedence);
    ParseRule* getRule(TokenType type);
    void defineVariable(uint8_t global);
    uint8_t parseVariable(const char* errorMessage);
    void declareVariable();
    void addLocal(Token name);
    int resolveUpvalue(Compiler* compiler, Token* name);
    int resolveLocal(Compiler* compiler, Token* name);
    int addUpvalue(Compiler* compiler, uint8_t index, bool isLocal);
    void beginScope();
    void endScope();
    void patchJump(int offset);
    void emitConstant(Value value);
    void emitReturn();
    int emitJump(uint8_t instruction);
    void emitLoop(int loopStart);
    void emitByte(uint8_t byte);
    void emitBytes(uint8_t byte1, uint8_t byte2);
    uint8_t makeConstant(Value value);
    uint8_t identifierConstant(Token* name);

public:
    CompilerInterfaceConcrete(ObjectFactory* pFactory);
    //> Calls and Functions compile-h
    ObjFunction* compile(const char* source);
    //< Calls and Functions compile-h
    //> Garbage Collection mark-compiler-roots-h
    void markCompilerRoots(std::function<void(Obj*)> callback);
    //< Garbage Collection mark-compiler-roots-h
};

//> Calls and Functions current-chunk

Chunk* Compiler::chunk()
{
    return &function->chunk;
}
//< Calls and Functions current-chunk

//< Compiling Expressions compiling-chunk
//> Compiling Expressions error-at
void Parser::errorAt(Token* token, const char* message)
{
    //> check-panic-mode
    if (panicMode)
        return;
    //< check-panic-mode
    //> set-panic-mode
    panicMode = true;
    //< set-panic-mode
    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    }
    else if (token->type == TOKEN_ERROR) {
        // Nothing.
    }
    else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    hadError = true;
}
//< Compiling Expressions error-at
//> Compiling Expressions error
void Parser::error(const char* message)
{
    errorAt(&previous, message);
}
//< Compiling Expressions error
//> Compiling Expressions error-at-current
void Parser::errorAtCurrent(const char* message)
{
    errorAt(&current, message);
}
//< Compiling Expressions error-at-current
//> Compiling Expressions advance

void Parser::advance()
{
    previous = current;

    for (;;) {
        current = scanner.scanToken();
        if (current.type != TOKEN_ERROR)
            break;

        errorAtCurrent(current.start);
    }
}
//< Compiling Expressions advance
//> Compiling Expressions consume
void Parser::consume(TokenType type, const char* message)
{
    if (current.type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}
//< Compiling Expressions consume
//> Global Variables check
bool Parser::check(TokenType type)
{
    return current.type == type;
}
//< Global Variables check
//> Global Variables match
bool Parser::match(TokenType type)
{
    if (!check(type))
        return false;
    advance();
    return true;
}
//< Global Variables match

CompilerInterfaceConcrete::CompilerInterfaceConcrete(ObjectFactory* pFactory) : factory(pFactory)
{
}
//> Compiling Expressions emit-byte
void CompilerInterfaceConcrete::emitByte(uint8_t byte)
{
    current->chunk()->write(byte, parser.previous.line);
}
//< Compiling Expressions emit-byte
//> Compiling Expressions emit-bytes
void CompilerInterfaceConcrete::emitBytes(uint8_t byte1, uint8_t byte2)
{
    emitByte(byte1);
    emitByte(byte2);
}
//< Compiling Expressions emit-bytes
//> Jumping Back and Forth emit-loop
void CompilerInterfaceConcrete::emitLoop(int loopStart)
{
    emitByte(OP_LOOP);

    int offset = current->chunk()->size() - loopStart + 2;
    if (offset > UINT16_MAX)
        parser.error("Loop body too large.");

    emitByte((offset >> 8) & 0xff);
    emitByte(offset & 0xff);
}
//< Jumping Back and Forth emit-loop
//> Jumping Back and Forth emit-jump
int CompilerInterfaceConcrete::emitJump(uint8_t instruction)
{
    emitByte(instruction);
    emitByte(0xff);
    emitByte(0xff);
    return current->chunk()->size() - 2;
}
//< Jumping Back and Forth emit-jump
//> Compiling Expressions emit-return
void CompilerInterfaceConcrete::emitReturn()
{
    /* Calls and Functions return-nil < Methods and Initializers return-this
      emitByte(OP_NIL);
    */
    //> Methods and Initializers return-this
    if (current->type == TYPE_INITIALIZER) {
        emitBytes(OP_GET_LOCAL, 0);
    }
    else {
        emitByte(OP_NIL);
    }

    //< Methods and Initializers return-this
    emitByte(OP_RETURN);
}
//< Compiling Expressions emit-return
//> Compiling Expressions make-constant
uint8_t CompilerInterfaceConcrete::makeConstant(Value value)
{
    //> Garbage Collection add-constant-push
    factory->push(value);
    //< Garbage Collection add-constant-push
    int constant = current->chunk()->addConstant(value);
    //> Garbage Collection add-constant-pop
    factory->pop();
    //< Garbage Collection add-constant-pop
    if (constant > UINT8_MAX) {
        parser.error("Too many constants in one chunk.");
        return 0;
    }

    return (uint8_t)constant;
}
//< Compiling Expressions make-constant
//> Compiling Expressions emit-constant
void CompilerInterfaceConcrete::emitConstant(Value value)
{
    emitBytes(OP_CONSTANT, makeConstant(value));
}
//< Compiling Expressions emit-constant
//> Jumping Back and Forth patch-jump
void CompilerInterfaceConcrete::patchJump(int offset)
{
    // -2 to adjust for the bytecode for the jump offset itself.
    int jump = current->chunk()->size() - offset - 2;

    if (jump > UINT16_MAX) {
        parser.error("Too much code to jump over.");
    }

    current->chunk()->code[offset] = (jump >> 8) & 0xff;
    current->chunk()->code[offset + 1] = jump & 0xff;
}
//< Jumping Back and Forth patch-jump
//> Local Variables init-compiler
/* Local Variables init-compiler < Calls and Functions init-compiler
static void initCompiler(Compiler* compiler) {
*/
//> Calls and Functions init-compiler
void CompilerInterfaceConcrete::initCompiler(Compiler* compiler, FunctionType type)
{
    //> store-enclosing
    compiler->enclosing = current;
    //< store-enclosing
    compiler->function = NULL;
    compiler->type = type;
    //< Calls and Functions init-compiler
    compiler->localCount = 0;
    compiler->scopeDepth = 0;
    //> Calls and Functions init-function
    compiler->function = factory->newFunction();
    //< Calls and Functions init-function
    current = compiler;
    //> Calls and Functions init-function-name
    if (type != TYPE_SCRIPT) {
        current->function->name = std::string(parser.previous.start, parser.previous.length);
    }
    //< Calls and Functions init-function-name
    //> Calls and Functions init-function-slot

    Local* local = &current->locals[current->localCount++];
    local->depth = 0;
    //> Closures init-zero-local-is-captured
    local->isCaptured = false;
    //< Closures init-zero-local-is-captured
    /* Calls and Functions init-function-slot < Methods and Initializers slot-zero
      local->name.start = "";
      local->name.length = 0;
    */
    //> Methods and Initializers slot-zero
    if (type != TYPE_FUNCTION) {
        local->name.start = "this";
        local->name.length = 4;
    }
    else {
        local->name.start = "";
        local->name.length = 0;
    }
    //< Methods and Initializers slot-zero
    //< Calls and Functions init-function-slot
}
//< Local Variables init-compiler
//> Compiling Expressions end-compiler
/* Compiling Expressions end-compiler < Calls and Functions end-compiler
static void endCompiler() {
*/
//> Calls and Functions end-compiler
ObjFunction* CompilerInterfaceConcrete::endCompiler()
{
    //< Calls and Functions end-compiler
    emitReturn();
    //> Calls and Functions end-function
    ObjFunction* function = current->function;

//< Calls and Functions end-function
//> dump-chunk
#ifdef DEBUG_PRINT_CODE
    if (!parser.hadError) {
        /* Compiling Expressions dump-chunk < Calls and Functions disassemble-end
            disassembleChunk(currentChunk(), "code");
        */
        //> Calls and Functions disassemble-end
        debug.disassembleChunk(current->chunk(), function->name != NULL ? function->name->chars : "<script>");
        //< Calls and Functions disassemble-end
    }
#endif
    //< dump-chunk
    //> Calls and Functions return-function

    //> restore-enclosing
    current = current->enclosing;
    //< restore-enclosing
    return function;
    //< Calls and Functions return-function
}
//< Compiling Expressions end-compiler
//> Local Variables begin-scope
void CompilerInterfaceConcrete::beginScope()
{
    current->scopeDepth++;
}
//< Local Variables begin-scope
//> Local Variables end-scope
void CompilerInterfaceConcrete::endScope()
{
    current->scopeDepth--;
    //> pop-locals

    while (current->localCount > 0 && current->locals[current->localCount - 1].depth > current->scopeDepth) {
        /* Local Variables pop-locals < Closures end-scope
            emitByte(OP_POP);
        */
        //> Closures end-scope
        if (current->locals[current->localCount - 1].isCaptured) {
            emitByte(OP_CLOSE_UPVALUE);
        }
        else {
            emitByte(OP_POP);
        }
        //< Closures end-scope
        current->localCount--;
    }
    //< pop-locals
}
//< Local Variables end-scope
//> Compiling Expressions forward-declarations

//> Global Variables forward-declarations
// static void statement();
// static void declaration();
//< Global Variables forward-declarations

//< Compiling Expressions forward-declarations
//> Global Variables identifier-constant
uint8_t CompilerInterfaceConcrete::identifierConstant(Token* name)
{
    return makeConstant(OBJ_VAL(factory->newString(name->start, name->length)));
}
//< Global Variables identifier-constant
//> Local Variables identifiers-equal
bool Compiler::identifiersEqual(Token* a, Token* b)
{
    if (a->length != b->length)
        return false;
    return memcmp(a->start, b->start, a->length) == 0;
}
//< Local Variables identifiers-equal
//> Local Variables resolve-local
int CompilerInterfaceConcrete::resolveLocal(Compiler* compiler, Token* name)
{
    for (int i = compiler->localCount - 1; i >= 0; i--) {
        Local* local = &compiler->locals[i];
        if (compiler->identifiersEqual(name, &local->name)) {
            //> own-initializer-error
            if (local->depth == -1) {
                parser.error("Can't read local variable in its own initializer.");
            }
            //< own-initializer-error
            return i;
        }
    }

    return -1;
}
//< Local Variables resolve-local
//> Closures add-upvalue
int CompilerInterfaceConcrete::addUpvalue(Compiler* compiler, uint8_t index, bool isLocal)
{
    int upvalueCount = compiler->function->upvalueCount;
    //> existing-upvalue

    for (int i = 0; i < upvalueCount; i++) {
        Upvalue* upvalue = &compiler->upvalues[i];
        if (upvalue->index == index && upvalue->isLocal == isLocal) {
            return i;
        }
    }

    //< existing-upvalue
    //> too-many-upvalues
    if (upvalueCount == UINT8_COUNT) {
        parser.error("Too many closure variables in function.");
        return 0;
    }

    //< too-many-upvalues
    compiler->upvalues[upvalueCount].isLocal = isLocal;
    compiler->upvalues[upvalueCount].index = index;
    return compiler->function->upvalueCount++;
}
//< Closures add-upvalue
//> Closures resolve-upvalue
int CompilerInterfaceConcrete::resolveUpvalue(Compiler* compiler, Token* name)
{
    if (compiler->enclosing == NULL)
        return -1;

    int local = resolveLocal(compiler->enclosing, name);
    if (local != -1) {
        //> mark-local-captured
        compiler->enclosing->locals[local].isCaptured = true;
        //< mark-local-captured
        return addUpvalue(compiler, (uint8_t)local, true);
    }

    //> resolve-upvalue-recurse
    int upvalue = resolveUpvalue(compiler->enclosing, name);
    if (upvalue != -1) {
        return addUpvalue(compiler, (uint8_t)upvalue, false);
    }

    //< resolve-upvalue-recurse
    return -1;
}
//< Closures resolve-upvalue
//> Local Variables add-local
void CompilerInterfaceConcrete::addLocal(Token name)
{
    //> too-many-locals
    if (current->localCount == UINT8_COUNT) {
        parser.error("Too many local variables in function.");
        return;
    }

    //< too-many-locals
    Local* local = &current->locals[current->localCount++];
    local->name = name;
    /* Local Variables add-local < Local Variables declare-undefined
      local->depth = current->scopeDepth;
    */
    //> declare-undefined
    local->depth = -1;
    //< declare-undefined
    //> Closures init-is-captured
    local->isCaptured = false;
    //< Closures init-is-captured
}
//< Local Variables add-local
//> Local Variables declare-variable
void CompilerInterfaceConcrete::declareVariable()
{
    if (current->scopeDepth == 0)
        return;

    Token* name = &parser.previous;
    //> existing-in-scope
    for (int i = current->localCount - 1; i >= 0; i--) {
        Local* local = &current->locals[i];
        if (local->depth != -1 && local->depth < current->scopeDepth) {
            break; // [negative]
        }

        if (current->identifiersEqual(name, &local->name)) {
            parser.error("Already a variable with this name in this scope.");
        }
    }

    //< existing-in-scope
    addLocal(*name);
}
//< Local Variables declare-variable
//> Global Variables parse-variable
uint8_t CompilerInterfaceConcrete::parseVariable(const char* errorMessage)
{
    parser.consume(TOKEN_IDENTIFIER, errorMessage);
    //> Local Variables parse-local

    declareVariable();
    if (current->scopeDepth > 0)
        return 0;

    //< Local Variables parse-local
    return identifierConstant(&parser.previous);
}
//< Global Variables parse-variable
//> Local Variables mark-initialized
void Compiler::markInitialized()
{
    //> Calls and Functions check-depth
    if (scopeDepth == 0)
        return;
    //< Calls and Functions check-depth
    locals[localCount - 1].depth = scopeDepth;
}
//< Local Variables mark-initialized
//> Global Variables define-variable
void CompilerInterfaceConcrete::defineVariable(uint8_t global)
{
    //> Local Variables define-variable
    if (current->scopeDepth > 0) {
        //> define-local
        current->markInitialized();
        //< define-local
        return;
    }

    //< Local Variables define-variable
    emitBytes(OP_DEFINE_GLOBAL, global);
}
//< Global Variables define-variable
//> Calls and Functions argument-list
uint8_t CompilerInterfaceConcrete::argumentList()
{
    uint8_t argCount = 0;
    if (!parser.check(TOKEN_RIGHT_PAREN)) {
        do {
            expression();
            //> arg-limit
            if (argCount == 255) {
                parser.error("Can't have more than 255 arguments.");
            }
            //< arg-limit
            argCount++;
        } while (parser.match(TOKEN_COMMA));
    }
    parser.consume(TOKEN_RIGHT_PAREN, "Expect ')' after arguments.");
    return argCount;
}
//< Calls and Functions argument-list
//> Jumping Back and Forth and
void and_(bool canAssign, CompilerInterfaceConcrete* ci)
{
    (void)canAssign;
    int endJump = ci->emitJump(OP_JUMP_IF_FALSE);

    ci->emitByte(OP_POP);
    ci->parsePrecedence(PREC_AND);

    ci->patchJump(endJump);
}
//< Jumping Back and Forth and
//> Compiling Expressions binary
/* Compiling Expressions binary < Global Variables binary
static void binary() {
*/
//> Global Variables binary
void binary(bool canAssign, CompilerInterfaceConcrete* ci)
{
    (void)canAssign;
    //< Global Variables binary
    TokenType operatorType = ci->parser.previous.type;
    ParseRule* rule = ci->getRule(operatorType);
    ci->parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        //> Types of Values comparison-operators
    case TOKEN_BANG_EQUAL:
        ci->emitBytes(OP_EQUAL, OP_NOT);
        break;
    case TOKEN_EQUAL_EQUAL:
        ci->emitByte(OP_EQUAL);
        break;
    case TOKEN_GREATER:
        ci->emitByte(OP_GREATER);
        break;
    case TOKEN_GREATER_EQUAL:
        ci->emitBytes(OP_LESS, OP_NOT);
        break;
    case TOKEN_LESS:
        ci->emitByte(OP_LESS);
        break;
    case TOKEN_LESS_EQUAL:
        ci->emitBytes(OP_GREATER, OP_NOT);
        break;
        //< Types of Values comparison-operators
    case TOKEN_PLUS:
        ci->emitByte(OP_ADD);
        break;
    case TOKEN_MINUS:
        ci->emitByte(OP_SUBTRACT);
        break;
    case TOKEN_STAR:
        ci->emitByte(OP_MULTIPLY);
        break;
    case TOKEN_SLASH:
        ci->emitByte(OP_DIVIDE);
        break;
    default:
        return; // Unreachable.
    }
}
//< Compiling Expressions binary
//> Calls and Functions compile-call
void call(bool canAssign, CompilerInterfaceConcrete* ci)
{
    (void)canAssign;
    uint8_t argCount = ci->argumentList();
    ci->emitBytes(OP_CALL, argCount);
}
//< Calls and Functions compile-call
//> Classes and Instances compile-dot
void dot(bool canAssign, CompilerInterfaceConcrete* ci)
{
    ci->parser.consume(TOKEN_IDENTIFIER, "Expect property name after '.'.");
    uint8_t name = ci->identifierConstant(&ci->parser.previous);

    if (canAssign && ci->parser.match(TOKEN_EQUAL)) {
        ci->expression();
        ci->emitBytes(OP_SET_PROPERTY, name);
        //> Methods and Initializers parse-call
    }
    else if (ci->parser.match(TOKEN_LEFT_PAREN)) {
        uint8_t argCount = ci->argumentList();
        ci->emitBytes(OP_INVOKE, name);
        ci->emitByte(argCount);
        //< Methods and Initializers parse-call
    }
    else {
        ci->emitBytes(OP_GET_PROPERTY, name);
    }
}
//< Classes and Instances compile-dot
//> Types of Values parse-literal
/* Types of Values parse-literal < Global Variables parse-literal
static void literal() {
*/
//> Global Variables parse-literal
void literal(bool canAssign, CompilerInterfaceConcrete* ci)
{
    (void)canAssign;
    //< Global Variables parse-literal
    switch (ci->parser.previous.type) {
    case TOKEN_FALSE:
        ci->emitByte(OP_FALSE);
        break;
    case TOKEN_NIL:
        ci->emitByte(OP_NIL);
        break;
    case TOKEN_TRUE:
        ci->emitByte(OP_TRUE);
        break;
    default:
        return; // Unreachable.
    }
}
//< Types of Values parse-literal
//> Compiling Expressions grouping
/* Compiling Expressions grouping < Global Variables grouping
static void grouping() {
*/
//> Global Variables grouping
void grouping(bool canAssign, CompilerInterfaceConcrete* ci)
{
    (void)canAssign;
    //< Global Variables grouping
    ci->expression();
    ci->parser.consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}
//< Compiling Expressions grouping
/* Compiling Expressions number < Global Variables number
static void number() {
*/
//> Compiling Expressions number
//> Global Variables number
void number(bool canAssign, CompilerInterfaceConcrete* ci)
{
    (void)canAssign;
    //< Global Variables number
    double value = strtod(ci->parser.previous.start, NULL);
    /* Compiling Expressions number < Types of Values const-number-val
      emitConstant(value);
    */
    //> Types of Values const-number-val
    ci->emitConstant(NUMBER_VAL(value));
    //< Types of Values const-number-val
}
//< Compiling Expressions number
//> Jumping Back and Forth or
void or_(bool canAssign, CompilerInterfaceConcrete* ci)
{
    (void)canAssign;
    int elseJump = ci->emitJump(OP_JUMP_IF_FALSE);
    int endJump = ci->emitJump(OP_JUMP);

    ci->patchJump(elseJump);
    ci->emitByte(OP_POP);

    ci->parsePrecedence(PREC_OR);
    ci->patchJump(endJump);
}
//< Jumping Back and Forth or
/* Strings parse-string < Global Variables string
static void string() {
*/
//> Strings parse-string
//> Global Variables string
void string(bool canAssign, CompilerInterfaceConcrete* ci)
{
    (void)canAssign;
    //< Global Variables string
    ci->emitConstant(OBJ_VAL(ci->factory->newString(ci->parser.previous.start + 1, ci->parser.previous.length - 2)));
}
//< Strings parse-string
/* Global Variables read-named-variable < Global Variables named-variable-signature
static void namedVariable(Token name) {
*/
//> Global Variables named-variable-signature
void CompilerInterfaceConcrete::namedVariable(Token name, bool canAssign)
{
    //< Global Variables named-variable-signature
    /* Global Variables read-named-variable < Local Variables named-local
      uint8_t arg = identifierConstant(&name);
    */
    //> Global Variables read-named-variable
    //> Local Variables named-local
    uint8_t getOp, setOp;
    int arg = resolveLocal(current, &name);
    if (arg != -1) {
        getOp = OP_GET_LOCAL;
        setOp = OP_SET_LOCAL;
        //> Closures named-variable-upvalue
    }
    else if ((arg = resolveUpvalue(current, &name)) != -1) {
        getOp = OP_GET_UPVALUE;
        setOp = OP_SET_UPVALUE;
        //< Closures named-variable-upvalue
    }
    else {
        arg = identifierConstant(&name);
        getOp = OP_GET_GLOBAL;
        setOp = OP_SET_GLOBAL;
    }
    //< Local Variables named-local
    /* Global Variables read-named-variable < Global Variables named-variable
      emitBytes(OP_GET_GLOBAL, arg);
    */
    //> named-variable

    /* Global Variables named-variable < Global Variables named-variable-can-assign
      if (match(TOKEN_EQUAL)) {
    */
    //> named-variable-can-assign
    if (canAssign && parser.match(TOKEN_EQUAL)) {
        //< named-variable-can-assign
        expression();
        /* Global Variables named-variable < Local Variables emit-set
            emitBytes(OP_SET_GLOBAL, arg);
        */
        //> Local Variables emit-set
        emitBytes(setOp, (uint8_t)arg);
        //< Local Variables emit-set
    }
    else {
        /* Global Variables named-variable < Local Variables emit-get
            emitBytes(OP_GET_GLOBAL, arg);
        */
        //> Local Variables emit-get
        emitBytes(getOp, (uint8_t)arg);
        //< Local Variables emit-get
    }
    //< named-variable
}
//< Global Variables read-named-variable
/* Global Variables variable-without-assign < Global Variables variable
static void variable() {
  namedVariable(parser.previous);
}
*/
//> Global Variables variable
void variable(bool canAssign, CompilerInterfaceConcrete* ci)
{
    ci->namedVariable(ci->parser.previous, canAssign);
}
//< Global Variables variable
//> Superclasses synthetic-token
Token Compiler::syntheticToken(const char* text)
{
    Token token;
    token.start = text;
    token.length = (int)strlen(text);
    return token;
}
//< Superclasses synthetic-token
//> Superclasses super
void super_(bool canAssign, CompilerInterfaceConcrete* ci)
{
    (void)canAssign;
    //> super-errors
    if (ci->currentClass == NULL) {
        ci->parser.error("Can't use 'super' outside of a class.");
    }
    else if (!ci->currentClass->hasSuperclass) {
        ci->parser.error("Can't use 'super' in a class with no superclass.");
    }

    //< super-errors
    ci->parser.consume(TOKEN_DOT, "Expect '.' after 'super'.");
    ci->parser.consume(TOKEN_IDENTIFIER, "Expect superclass method name.");
    uint8_t name = ci->identifierConstant(&ci->parser.previous);
    //> super-get

    ci->namedVariable(ci->current->syntheticToken("this"), false);
    /* Superclasses super-get < Superclasses super-invoke
      namedVariable(syntheticToken("super"), false);
      emitBytes(OP_GET_SUPER, name);
    */
    //< super-get
    //> super-invoke
    if (ci->parser.match(TOKEN_LEFT_PAREN)) {
        uint8_t argCount = ci->argumentList();
        ci->namedVariable(ci->current->syntheticToken("super"), false);
        ci->emitBytes(OP_SUPER_INVOKE, name);
        ci->emitByte(argCount);
    }
    else {
        ci->namedVariable(ci->current->syntheticToken("super"), false);
        ci->emitBytes(OP_GET_SUPER, name);
    }
    //< super-invoke
}
//< Superclasses super
//> Methods and Initializers this
void this_(bool canAssign, CompilerInterfaceConcrete* ci)
{
    (void)canAssign;
    //> this-outside-class
    if (ci->currentClass == NULL) {
        ci->parser.error("Can't use 'this' outside of a class.");
        return;
    }

    //< this-outside-class
    variable(false, ci);
} // [this]
//< Methods and Initializers this
//> Compiling Expressions unary
/* Compiling Expressions unary < Global Variables unary
static void unary() {
*/
//> Global Variables unary
void unary(bool canAssign, CompilerInterfaceConcrete* ci)
{
    (void)canAssign;
    //< Global Variables unary
    TokenType operatorType = ci->parser.previous.type;

    // Compile the operand.
    /* Compiling Expressions unary < Compiling Expressions unary-operand
      parser.expression();
    */
    //> unary-operand
    ci->parsePrecedence(PREC_UNARY);
    //< unary-operand

    // Emit the operator instruction.
    switch (operatorType) {
        //> Types of Values compile-not
    case TOKEN_BANG:
        ci->emitByte(OP_NOT);
        break;
        //< Types of Values compile-not
    case TOKEN_MINUS:
        ci->emitByte(OP_NEGATE);
        break;
    default:
        return; // Unreachable.
    }
}
//< Compiling Expressions unary
//> Compiling Expressions rules
ParseRule rules[] = {
    /* Compiling Expressions rules < Calls and Functions infix-left-paren
      [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
    */
    //> Calls and Functions infix-left-paren
    [TOKEN_LEFT_PAREN] = {grouping, call, PREC_CALL},
    //< Calls and Functions infix-left-paren
    [TOKEN_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},
    [TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE}, // [big]
    [TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},
    [TOKEN_COMMA] = {NULL, NULL, PREC_NONE},
    /* Compiling Expressions rules < Classes and Instances table-dot
      [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
    */
    //> Classes and Instances table-dot
    [TOKEN_DOT] = {NULL, dot, PREC_CALL},
    //< Classes and Instances table-dot
    [TOKEN_MINUS] = {unary, binary, PREC_TERM},
    [TOKEN_PLUS] = {NULL, binary, PREC_TERM},
    [TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},
    [TOKEN_SLASH] = {NULL, binary, PREC_FACTOR},
    [TOKEN_STAR] = {NULL, binary, PREC_FACTOR},
    /* Compiling Expressions rules < Types of Values table-not
      [TOKEN_BANG]          = {NULL,     NULL,   PREC_NONE},
    */
    //> Types of Values table-not
    [TOKEN_BANG] = {unary, NULL, PREC_NONE},
    //< Types of Values table-not
    /* Compiling Expressions rules < Types of Values table-equal
      [TOKEN_BANG_EQUAL]    = {NULL,     NULL,   PREC_NONE},
    */
    //> Types of Values table-equal
    [TOKEN_BANG_EQUAL] = {NULL, binary, PREC_EQUALITY},
    //< Types of Values table-equal
    [TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},
    /* Compiling Expressions rules < Types of Values table-comparisons
      [TOKEN_EQUAL_EQUAL]   = {NULL,     NULL,   PREC_NONE},
      [TOKEN_GREATER]       = {NULL,     NULL,   PREC_NONE},
      [TOKEN_GREATER_EQUAL] = {NULL,     NULL,   PREC_NONE},
      [TOKEN_LESS]          = {NULL,     NULL,   PREC_NONE},
      [TOKEN_LESS_EQUAL]    = {NULL,     NULL,   PREC_NONE},
    */
    //> Types of Values table-comparisons
    [TOKEN_EQUAL_EQUAL] = {NULL, binary, PREC_EQUALITY},
    [TOKEN_GREATER] = {NULL, binary, PREC_COMPARISON},
    [TOKEN_GREATER_EQUAL] = {NULL, binary, PREC_COMPARISON},
    [TOKEN_LESS] = {NULL, binary, PREC_COMPARISON},
    [TOKEN_LESS_EQUAL] = {NULL, binary, PREC_COMPARISON},
    //< Types of Values table-comparisons
    /* Compiling Expressions rules < Global Variables table-identifier
      [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
    */
    //> Global Variables table-identifier
    [TOKEN_IDENTIFIER] = {variable, NULL, PREC_NONE},
    //< Global Variables table-identifier
    /* Compiling Expressions rules < Strings table-string
      [TOKEN_STRING]        = {NULL,     NULL,   PREC_NONE},
    */
    //> Strings table-string
    [TOKEN_STRING] = {string, NULL, PREC_NONE},
    //< Strings table-string
    [TOKEN_NUMBER] = {number, NULL, PREC_NONE},
    /* Compiling Expressions rules < Jumping Back and Forth table-and
      [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
    */
    //> Jumping Back and Forth table-and
    [TOKEN_AND] = {NULL, and_, PREC_AND},
    //< Jumping Back and Forth table-and
    [TOKEN_CLASS] = {NULL, NULL, PREC_NONE},
    [TOKEN_ELSE] = {NULL, NULL, PREC_NONE},
    /* Compiling Expressions rules < Types of Values table-false
      [TOKEN_FALSE]         = {NULL,     NULL,   PREC_NONE},
    */
    //> Types of Values table-false
    [TOKEN_FALSE] = {literal, NULL, PREC_NONE},
    //< Types of Values table-false
    [TOKEN_FOR] = {NULL, NULL, PREC_NONE},
    [TOKEN_FUN] = {NULL, NULL, PREC_NONE},
    [TOKEN_IF] = {NULL, NULL, PREC_NONE},
    /* Compiling Expressions rules < Types of Values table-nil
      [TOKEN_NIL]           = {NULL,     NULL,   PREC_NONE},
    */
    //> Types of Values table-nil
    [TOKEN_NIL] = {literal, NULL, PREC_NONE},
    //< Types of Values table-nil
    /* Compiling Expressions rules < Jumping Back and Forth table-or
      [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
    */
    //> Jumping Back and Forth table-or
    [TOKEN_OR] = {NULL, or_, PREC_OR},
    //< Jumping Back and Forth table-or
    [TOKEN_PRINT] = {NULL, NULL, PREC_NONE},
    [TOKEN_RETURN] = {NULL, NULL, PREC_NONE},
    /* Compiling Expressions rules < Superclasses table-super
      [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
    */
    //> Superclasses table-super
    [TOKEN_SUPER] = {super_, NULL, PREC_NONE},
    //< Superclasses table-super
    /* Compiling Expressions rules < Methods and Initializers table-this
      [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
    */
    //> Methods and Initializers table-this
    [TOKEN_THIS] = {this_, NULL, PREC_NONE},
    //< Methods and Initializers table-this
    /* Compiling Expressions rules < Types of Values table-true
      [TOKEN_TRUE]          = {NULL,     NULL,   PREC_NONE},
    */
    //> Types of Values table-true
    [TOKEN_TRUE] = {literal, NULL, PREC_NONE},
    //< Types of Values table-true
    [TOKEN_VAR] = {NULL, NULL, PREC_NONE},
    [TOKEN_WHILE] = {NULL, NULL, PREC_NONE},
    [TOKEN_ERROR] = {NULL, NULL, PREC_NONE},
    [TOKEN_EOF] = {NULL, NULL, PREC_NONE},
};
//< Compiling Expressions rules
//> Compiling Expressions parse-precedence
void CompilerInterfaceConcrete::parsePrecedence(Precedence precedence)
{
    /* Compiling Expressions parse-precedence < Compiling Expressions precedence-body
      // What goes here?
    */
    //> precedence-body
    parser.advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        parser.error("Expect expression.");
        return;
    }

    /* Compiling Expressions precedence-body < Global Variables prefix-rule
      prefixRule();
    */
    //> Global Variables prefix-rule
    bool canAssign = precedence <= PREC_ASSIGNMENT;
    prefixRule(canAssign, this);
    //< Global Variables prefix-rule
    //> infix

    while (precedence <= getRule(parser.current.type)->precedence) {
        parser.advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        /* Compiling Expressions infix < Global Variables infix-rule
            infixRule();
        */
        //> Global Variables infix-rule
        infixRule(canAssign, this);
        //< Global Variables infix-rule
    }
    //> Global Variables invalid-assign

    if (canAssign && parser.match(TOKEN_EQUAL)) {
        parser.error("Invalid assignment target.");
    }
    //< Global Variables invalid-assign
    //< infix
    //< precedence-body
}
//< Compiling Expressions parse-precedence
//> Compiling Expressions get-rule
ParseRule* CompilerInterfaceConcrete::getRule(TokenType type)
{
    return &rules[type];
}
//< Compiling Expressions get-rule
//> Compiling Expressions expression
void CompilerInterfaceConcrete::expression()
{
    /* Compiling Expressions expression < Compiling Expressions expression-body
      // What goes here?
    */
    //> expression-body
    parsePrecedence(PREC_ASSIGNMENT);
    //< expression-body
}
//< Compiling Expressions expression
//> Local Variables block
void CompilerInterfaceConcrete::block()
{
    while (!parser.check(TOKEN_RIGHT_BRACE) && !parser.check(TOKEN_EOF)) {
        declaration();
    }

    parser.consume(TOKEN_RIGHT_BRACE, "Expect '}' after block.");
}
//< Local Variables block
//> Calls and Functions compile-function
void CompilerInterfaceConcrete::function(FunctionType type)
{
    Compiler compiler;
    initCompiler(&compiler, type);
    beginScope(); // [no-end-scope]

    parser.consume(TOKEN_LEFT_PAREN, "Expect '(' after function name.");
    //> parameters
    if (!parser.check(TOKEN_RIGHT_PAREN)) {
        do {
            current->function->arity++;
            if (current->function->arity > 255) {
                parser.errorAtCurrent("Can't have more than 255 parameters.");
            }
            uint8_t constant = parseVariable("Expect parameter name.");
            defineVariable(constant);
        } while (parser.match(TOKEN_COMMA));
    }
    //< parameters
    parser.consume(TOKEN_RIGHT_PAREN, "Expect ')' after parameters.");
    parser.consume(TOKEN_LEFT_BRACE, "Expect '{' before function body.");
    block();

    ObjFunction* function = endCompiler();
    /* Calls and Functions compile-function < Closures emit-closure
      emitBytes(OP_CONSTANT, makeConstant(OBJ_VAL(function)));
    */
    //> Closures emit-closure
    emitBytes(OP_CLOSURE, makeConstant(OBJ_VAL(function)));
    //< Closures emit-closure
    //> Closures capture-upvalues

    for (int i = 0; i < function->upvalueCount; i++) {
        emitByte(compiler.upvalues[i].isLocal ? 1 : 0);
        emitByte(compiler.upvalues[i].index);
    }
    //< Closures capture-upvalues
}
//< Calls and Functions compile-function
//> Methods and Initializers method
void CompilerInterfaceConcrete::method()
{
    parser.consume(TOKEN_IDENTIFIER, "Expect method name.");
    uint8_t constant = identifierConstant(&parser.previous);
    //> method-body

    //< method-body
    /* Methods and Initializers method-body < Methods and Initializers method-type
      FunctionType type = TYPE_FUNCTION;
    */
    //> method-type
    FunctionType type = TYPE_METHOD;
    //< method-type
    //> initializer-name
    if (parser.previous.length == 4 && memcmp(parser.previous.start, "init", 4) == 0) {
        type = TYPE_INITIALIZER;
    }

    //< initializer-name
    //> method-body
    function(type);
    //< method-body
    emitBytes(OP_METHOD, constant);
}
//< Methods and Initializers method
//> Classes and Instances class-declaration
void CompilerInterfaceConcrete::classDeclaration()
{
    parser.consume(TOKEN_IDENTIFIER, "Expect class name.");
    //> Methods and Initializers class-name
    Token className = parser.previous;
    //< Methods and Initializers class-name
    uint8_t nameConstant = identifierConstant(&parser.previous);
    declareVariable();

    emitBytes(OP_CLASS, nameConstant);
    defineVariable(nameConstant);

    //> Methods and Initializers create-class-compiler
    ClassCompiler classCompiler;
    //> Superclasses init-has-superclass
    classCompiler.hasSuperclass = false;
    //< Superclasses init-has-superclass
    classCompiler.enclosing = currentClass;
    currentClass = &classCompiler;

    //< Methods and Initializers create-class-compiler
    //> Superclasses compile-superclass
    if (parser.match(TOKEN_LESS)) {
        parser.consume(TOKEN_IDENTIFIER, "Expect superclass name.");
        variable(false, this);
        //> inherit-self

        if (current->identifiersEqual(&className, &parser.previous)) {
            parser.error("A class can't inherit from itself.");
        }

        //< inherit-self
        //> superclass-variable
        beginScope();
        addLocal(current->syntheticToken("super"));
        defineVariable(0);

        //< superclass-variable
        namedVariable(className, false);
        emitByte(OP_INHERIT);
        //> set-has-superclass
        classCompiler.hasSuperclass = true;
        //< set-has-superclass
    }

    //< Superclasses compile-superclass
    //> Methods and Initializers load-class
    namedVariable(className, false);
    //< Methods and Initializers load-class
    parser.consume(TOKEN_LEFT_BRACE, "Expect '{' before class body.");
    //> Methods and Initializers class-body
    while (!parser.check(TOKEN_RIGHT_BRACE) && !parser.check(TOKEN_EOF)) {
        method();
    }
    //< Methods and Initializers class-body
    parser.consume(TOKEN_RIGHT_BRACE, "Expect '}' after class body.");
    //> Methods and Initializers pop-class
    emitByte(OP_POP);
    //< Methods and Initializers pop-class
    //> Superclasses end-superclass-scope

    if (classCompiler.hasSuperclass) {
        endScope();
    }
    //< Superclasses end-superclass-scope
    //> Methods and Initializers pop-enclosing

    currentClass = currentClass->enclosing;
    //< Methods and Initializers pop-enclosing
}
//< Classes and Instances class-declaration
//> Calls and Functions fun-declaration
void CompilerInterfaceConcrete::funDeclaration()
{
    uint8_t global = parseVariable("Expect function name.");
    current->markInitialized();
    function(TYPE_FUNCTION);
    defineVariable(global);
}
//< Calls and Functions fun-declaration
//> Global Variables var-declaration
void CompilerInterfaceConcrete::varDeclaration()
{
    uint8_t global = parseVariable("Expect variable name.");

    if (parser.match(TOKEN_EQUAL)) {
        expression();
    }
    else {
        emitByte(OP_NIL);
    }
    parser.consume(TOKEN_SEMICOLON, "Expect ';' after variable declaration.");

    defineVariable(global);
}
//< Global Variables var-declaration
//> Global Variables expression-statement
void CompilerInterfaceConcrete::expressionStatement()
{
    expression();
    parser.consume(TOKEN_SEMICOLON, "Expect ';' after expression.");
    emitByte(OP_POP);
}
//< Global Variables expression-statement
//> Jumping Back and Forth for-statement
void CompilerInterfaceConcrete::forStatement()
{
    //> for-begin-scope
    beginScope();
    //< for-begin-scope
    parser.consume(TOKEN_LEFT_PAREN, "Expect '(' after 'for'.");
    /* Jumping Back and Forth for-statement < Jumping Back and Forth for-initializer
      consume(TOKEN_SEMICOLON, "Expect ';'.");
    */
    //> for-initializer
    if (parser.match(TOKEN_SEMICOLON)) {
        // No initializer.
    }
    else if (parser.match(TOKEN_VAR)) {
        varDeclaration();
    }
    else {
        expressionStatement();
    }
    //< for-initializer

    int loopStart = current->chunk()->size();
    /* Jumping Back and Forth for-statement < Jumping Back and Forth for-exit
      consume(TOKEN_SEMICOLON, "Expect ';'.");
    */
    //> for-exit
    int exitJump = -1;
    if (!parser.match(TOKEN_SEMICOLON)) {
        expression();
        parser.consume(TOKEN_SEMICOLON, "Expect ';' after loop condition.");

        // Jump out of the loop if the condition is false.
        exitJump = emitJump(OP_JUMP_IF_FALSE);
        emitByte(OP_POP); // Condition.
    }

    //< for-exit
    /* Jumping Back and Forth for-statement < Jumping Back and Forth for-increment
      consume(TOKEN_RIGHT_PAREN, "Expect ')' after for clauses.");
    */
    //> for-increment
    if (!parser.match(TOKEN_RIGHT_PAREN)) {
        int bodyJump = emitJump(OP_JUMP);
        int incrementStart = current->chunk()->size();
        expression();
        emitByte(OP_POP);
        parser.consume(TOKEN_RIGHT_PAREN, "Expect ')' after for clauses.");

        emitLoop(loopStart);
        loopStart = incrementStart;
        patchJump(bodyJump);
    }
    //< for-increment

    statement();
    emitLoop(loopStart);
    //> exit-jump

    if (exitJump != -1) {
        patchJump(exitJump);
        emitByte(OP_POP); // Condition.
    }

    //< exit-jump
    //> for-end-scope
    endScope();
    //< for-end-scope
}
//< Jumping Back and Forth for-statement
//> Jumping Back and Forth if-statement
void CompilerInterfaceConcrete::ifStatement()
{
    parser.consume(TOKEN_LEFT_PAREN, "Expect '(' after 'if'.");
    expression();
    parser.consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition."); // [paren]

    int thenJump = emitJump(OP_JUMP_IF_FALSE);
    //> pop-then
    emitByte(OP_POP);
    //< pop-then
    statement();

    //> jump-over-else
    int elseJump = emitJump(OP_JUMP);

    //< jump-over-else
    patchJump(thenJump);
    //> pop-end
    emitByte(OP_POP);
    //< pop-end
    //> compile-else

    if (parser.match(TOKEN_ELSE))
        statement();
    //< compile-else
    //> patch-else
    patchJump(elseJump);
    //< patch-else
}
//< Jumping Back and Forth if-statement
//> Global Variables print-statement
void CompilerInterfaceConcrete::printStatement()
{
    expression();
    parser.consume(TOKEN_SEMICOLON, "Expect ';' after value.");
    emitByte(OP_PRINT);
}
//< Global Variables print-statement
//> Calls and Functions return-statement
void CompilerInterfaceConcrete::returnStatement()
{
    //> return-from-script
    if (current->type == TYPE_SCRIPT) {
        parser.error("Can't return from top-level code.");
    }

    //< return-from-script
    if (parser.match(TOKEN_SEMICOLON)) {
        emitReturn();
    }
    else {
        //> Methods and Initializers return-from-init
        if (current->type == TYPE_INITIALIZER) {
            parser.error("Can't return a value from an initializer.");
        }

        //< Methods and Initializers return-from-init
        expression();
        parser.consume(TOKEN_SEMICOLON, "Expect ';' after return value.");
        emitByte(OP_RETURN);
    }
}
//< Calls and Functions return-statement
//> Jumping Back and Forth while-statement
void CompilerInterfaceConcrete::whileStatement()
{
    //> loop-start
    int loopStart = current->chunk()->size();
    //< loop-start
    parser.consume(TOKEN_LEFT_PAREN, "Expect '(' after 'while'.");
    expression();
    parser.consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition.");

    int exitJump = emitJump(OP_JUMP_IF_FALSE);
    emitByte(OP_POP);
    statement();
    //> loop
    emitLoop(loopStart);
    //< loop

    patchJump(exitJump);
    emitByte(OP_POP);
}
//< Jumping Back and Forth while-statement
//> Global Variables synchronize
void CompilerInterfaceConcrete::synchronize()
{
    parser.panicMode = false;

    while (parser.current.type != TOKEN_EOF) {
        if (parser.previous.type == TOKEN_SEMICOLON)
            return;
        switch (parser.current.type) {
        case TOKEN_CLASS:
        case TOKEN_FUN:
        case TOKEN_VAR:
        case TOKEN_FOR:
        case TOKEN_IF:
        case TOKEN_WHILE:
        case TOKEN_PRINT:
        case TOKEN_RETURN:
            return;

        default:; // Do nothing.
        }

        parser.advance();
    }
}
//< Global Variables synchronize
//> Global Variables declaration
void CompilerInterfaceConcrete::declaration()
{
    //> Classes and Instances match-class
    if (parser.match(TOKEN_CLASS)) {
        classDeclaration();
        /* Calls and Functions match-fun < Classes and Instances match-class
          if (match(TOKEN_FUN)) {
        */
    }
    else if (parser.match(TOKEN_FUN)) {
        //< Classes and Instances match-class
        //> Calls and Functions match-fun
        funDeclaration();
        /* Global Variables match-var < Calls and Functions match-fun
          if (match(TOKEN_VAR)) {
        */
    }
    else if (parser.match(TOKEN_VAR)) {
        //< Calls and Functions match-fun
        //> match-var
        varDeclaration();
    }
    else {
        statement();
    }
    //< match-var
    /* Global Variables declaration < Global Variables match-var
      statement();
    */
    //> call-synchronize

    if (parser.panicMode)
        synchronize();
    //< call-synchronize
}
//< Global Variables declaration
//> Global Variables statement
void CompilerInterfaceConcrete::statement()
{
    if (parser.match(TOKEN_PRINT)) {
        printStatement();
        //> Jumping Back and Forth parse-for
    }
    else if (parser.match(TOKEN_FOR)) {
        forStatement();
        //< Jumping Back and Forth parse-for
        //> Jumping Back and Forth parse-if
    }
    else if (parser.match(TOKEN_IF)) {
        ifStatement();
        //< Jumping Back and Forth parse-if
        //> Calls and Functions match-return
    }
    else if (parser.match(TOKEN_RETURN)) {
        returnStatement();
        //< Calls and Functions match-return
        //> Jumping Back and Forth parse-while
    }
    else if (parser.match(TOKEN_WHILE)) {
        whileStatement();
        //< Jumping Back and Forth parse-while
        //> Local Variables parse-block
    }
    else if (parser.match(TOKEN_LEFT_BRACE)) {
        beginScope();
        block();
        endScope();
        //< Local Variables parse-block
        //> parse-expressions-statement
    }
    else {
        expressionStatement();
        //< parse-expressions-statement
    }
}
//< Global Variables statement

/* Scanning on Demand compiler-c < Compiling Expressions compile-signature
void compile(const char* source) {
*/
/* Compiling Expressions compile-signature < Calls and Functions compile-signature
bool compile(const char* source, Chunk* chunk) {
*/

//> Calls and Functions compile-signature
ObjFunction* CompilerInterfaceConcrete::compile(const char* source)
{
    //< Calls and Functions compile-signature
    parser.scanner.init(source);
    /* Scanning on Demand dump-tokens < Compiling Expressions compile-chunk
      int line = -1;
      for (;;) {
        Token token = scanToken();
        if (token.line != line) {
          printf("%4d ", token.line);
          line = token.line;
        } else {
          printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start); // [format]

        if (token.type == TOKEN_EOF) break;
      }
    */
    //> Local Variables compiler
    Compiler compiler;
    //< Local Variables compiler
    /* Local Variables compiler < Calls and Functions call-init-compiler
      initCompiler(&compiler);
    */
    //> Calls and Functions call-init-compiler
    initCompiler(&compiler, TYPE_SCRIPT);
    //< Calls and Functions call-init-compiler
    /* Compiling Expressions init-compile-chunk < Calls and Functions call-init-compiler
      compilingChunk = chunk;
    */
    //> Compiling Expressions compile-chunk
    //> init-parser-error

    parser.hadError = false;
    parser.panicMode = false;

    //< init-parser-error
    parser.advance();
    //< Compiling Expressions compile-chunk
    /* Compiling Expressions compile-chunk < Global Variables compile
      eparser.xpression();
      consume(TOKEN_EOF, "Expect end of expression.");
    */
    //> Global Variables compile

    while (!parser.match(TOKEN_EOF)) {
        declaration();
    }

    //< Global Variables compile
    /* Compiling Expressions finish-compile < Calls and Functions call-end-compiler
      endCompiler();
    */
    /* Compiling Expressions return-had-error < Calls and Functions call-end-compiler
      return !parser.hadError;
    */
    //> Calls and Functions call-end-compiler
    ObjFunction* pfunction = endCompiler();
    return parser.hadError ? NULL : pfunction;
    //< Calls and Functions call-end-compiler
}
//> Garbage Collection mark-compiler-roots
void CompilerInterfaceConcrete::markCompilerRoots(std::function<void(Obj*)> callback)
{
    Compiler* compiler = current;
    while (compiler != NULL) {
        callback((Obj*)compiler->function);
        compiler = compiler->enclosing;
    }
}
//< Garbage Collection mark-compiler-roots

class CompilerFactoryConcrete : public CompilerFactory {
public:
    CompilerInterface* create(ObjectFactory* factory);
};

CompilerInterface* CompilerFactoryConcrete::create(ObjectFactory* factory)
{
    return new CompilerInterfaceConcrete(factory);
}

CompilerFactory* CompilerFactory::instance(void)
{
    static CompilerFactoryConcrete s_compiler_factory;
    return &s_compiler_factory;
}
