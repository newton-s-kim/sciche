//> Chunks of Bytecode chunk-h
#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.hpp"
//> chunk-h-include-value
#include "value.hpp"
//< chunk-h-include-value
//> op-enum

typedef enum {
    //> op-constant
    OP_CONSTANT,
    //< op-constant
    //> Types of Values literal-ops
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    //< Types of Values literal-ops
    //> Global Variables pop-op
    OP_POP,
    //< Global Variables pop-op
    //> Local Variables get-local-op
    OP_GET_LOCAL,
    //< Local Variables get-local-op
    //> Local Variables set-local-op
    OP_SET_LOCAL,
    //< Local Variables set-local-op
    //> Global Variables get-global-op
    OP_GET_GLOBAL,
    //< Global Variables get-global-op
    //> Global Variables define-global-op
    OP_DEFINE_GLOBAL,
    //< Global Variables define-global-op
    //> Global Variables set-global-op
    OP_SET_GLOBAL,
    //< Global Variables set-global-op
    //> Closures upvalue-ops
    OP_GET_UPVALUE,
    OP_SET_UPVALUE,
    //< Closures upvalue-ops
    //> Classes and Instances property-ops
    OP_GET_PROPERTY,
    OP_SET_PROPERTY,
    //< Classes and Instances property-ops
    //> Superclasses get-super-op
    OP_GET_SUPER,
    //< Superclasses get-super-op
    //> Types of Values comparison-ops
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    //< Types of Values comparison-ops
    //> A Virtual Machine binary-ops
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    //> Types of Values not-op
    OP_NOT,
    //< Types of Values not-op
    //< A Virtual Machine binary-ops
    //> A Virtual Machine negate-op
    OP_NEGATE,
    //< A Virtual Machine negate-op
    //> Global Variables op-print
    OP_PRINT,
    //< Global Variables op-print
    //> Jumping Back and Forth jump-op
    OP_JUMP,
    //< Jumping Back and Forth jump-op
    //> Jumping Back and Forth jump-if-false-op
    OP_JUMP_IF_FALSE,
    //< Jumping Back and Forth jump-if-false-op
    //> Jumping Back and Forth loop-op
    OP_LOOP,
    //< Jumping Back and Forth loop-op
    //> Calls and Functions op-call
    OP_CALL,
    //< Calls and Functions op-call
    //> Methods and Initializers invoke-op
    OP_INVOKE,
    //< Methods and Initializers invoke-op
    //> Superclasses super-invoke-op
    OP_SUPER_INVOKE,
    //< Superclasses super-invoke-op
    //> Closures closure-op
    OP_CLOSURE,
    //< Closures closure-op
    //> Closures close-upvalue-op
    OP_CLOSE_UPVALUE,
    //< Closures close-upvalue-op
    OP_RETURN,
    //> Classes and Instances class-op
    OP_CLASS,
    //< Classes and Instances class-op
    //> Superclasses inherit-op
    OP_INHERIT,
    //< Superclasses inherit-op
    //> Methods and Initializers method-op
    OP_METHOD
    //< Methods and Initializers method-op
} OpCode;
//< op-enum
//> chunk-struct

class Chunk {
private:
    //> count-and-capacity
    int count;
    int capacity;
    //> chunk-lines
    int* lines;
    //< chunk-lines
    void* reallocate(void* pointer, size_t oldSize, size_t newSize);

public:
    //< count-and-capacity
    uint8_t* code;
    //> chunk-constants
    std::vector<Value> constants;
    //< chunk-constants
    //> init-chunk-h
    Chunk();
    //< init-chunk-h
    //> free-chunk-h
    ~Chunk();
    //< free-chunk-h
    /* Chunks of Bytecode write-chunk-h < Chunks of Bytecode write-chunk-with-line-h
    void writeChunk(Chunk* chunk, uint8_t byte);
    */
    //> write-chunk-with-line-h
    void write(uint8_t byte, int line);
    //< write-chunk-with-line-h
    //> add-constant-h
    int addConstant(Value value);
    //< add-constant-h
    int size(void);
    inline int getLine(int pos);
};
//< chunk-struct

int Chunk::getLine(int pos)
{
    return lines[pos];
}
#endif
