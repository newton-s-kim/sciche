//> A Virtual Machine vm-h
#ifndef clox_vm_h
#define clox_vm_h

/* A Virtual Machine vm-h < Calls and Functions vm-include-object
#include "chunk.hpp"
*/
//> Calls and Functions vm-include-object
#include "object.hpp"
//< Calls and Functions vm-include-object
//> Hash Tables vm-include-table
#include "table.hpp"
//< Hash Tables vm-include-table
//> vm-include-value
#include "value.hpp"
//< vm-include-value
//> stack-max
#ifdef DEBUG_TRACE_EXECUTION
#include "debug.hpp"
#endif
#include "compiler.hpp"

#include <map>
#include <stack>
#include <string>

//< stack-max
/* A Virtual Machine stack-max < Calls and Functions frame-max
#define STACK_MAX 256
*/
//> Calls and Functions frame-max
#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)
//< Calls and Functions frame-max
//> Calls and Functions call-frame

// clang-format off
//> interpret-result
typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR = 65,
  INTERPRET_RUNTIME_ERROR = 70
} InterpretResult;
// clang-format on

class CallFrame {
public:
    /* Calls and Functions call-frame < Closures call-frame-closure
      ObjFunction* function;
    */
    //> Closures call-frame-closure
    ObjClosure* closure;
    //< Closures call-frame-closure
    uint8_t* ip;
    Value* slots;
};
//< Calls and Functions call-frame

class VM : public ObjectFactory {
    /* A Virtual Machine vm-h < Calls and Functions frame-array
      Chunk* chunk;
    */
    /* A Virtual Machine ip < Calls and Functions frame-array
      uint8_t* ip;
    */
private:
#ifdef DEBUG_TRACE_EXECUTION
    Debug debug;
#endif
    void resetStack();
    void runtimeError(const char* format, ...);
    bool call(ObjClosure* closure, int argCount);
    bool invokeFromClass(ObjClass* klass, ObjString* name, int argCount);
    Value peek(int distance);
    ObjUpvalue* captureUpvalue(Value* local);
    void closeUpvalues(Value* last);
    bool isFalsey(Value value);

public:
    CompilerInterface* compiler;
    //> Calls and Functions frame-array
    CallFrame frames[FRAMES_MAX];
    int frameCount;

    //< Calls and Functions frame-array
    //> vm-stack
    Value stack[STACK_MAX];
    Value* stackTop;
    //< vm-stack
    //> Global Variables vm-globals
    Table globals;
    //< Global Variables vm-globals
    //> Methods and Initializers vm-init-string
    ObjString* initString;
    //< Methods and Initializers vm-init-string
    //> Closures open-upvalues-field
    ObjUpvalue* openUpvalues;
    //< Closures open-upvalues-field
    //> Garbage Collection vm-fields

    size_t bytesAllocated;
    size_t nextGC;
    //< Garbage Collection vm-fields
    //> Strings objects-root
    Obj* objects;
    //< Strings objects-root
    //> Garbage Collection vm-gray-stack
    std::stack<Obj*> grayStack;
    //< Garbage Collection vm-gray-stack
    //> Hash Tables strings
    std::map<std::string, ObjString*> strings;
    //< Hash Tables strings

private:
    bool callValue(Value callee, int argCount);
    bool bindMethod(ObjClass* klass, ObjString* name);
    void concatenate();
    InterpretResult run(void);
    bool invoke(ObjString* name, int argCount);
    void defineMethod(ObjString* name);
    void hack(bool b);
    //> Strings free-objects-h
    void freeObjects();
    //< Strings free-objects-h

    ObjBoundMethod* newBoundMethod(Value receiver, ObjClosure* method);
    ObjClass* newClass(std::string name);
    ObjClosure* newClosure(ObjFunction* function);
    ObjInstance* newInstance(ObjClass* klass);
    ObjNative* newNative(NativeFn function);
    ObjUpvalue* newUpvalue(Value* slot);
    ObjString* allocateString(std::string chars);

    void markArray(std::vector<Value>& array);
    void blackenObject(Obj* object);
    void markRoots();
    void traceReferences();
    void sweep();
    void collectGarbage();
    void markObject(Obj* object);
    void markValue(Value value);
    void removeString(void);
    void collect(size_t oldSize, size_t newSize);

public:
    //< Strings extern-vm
    VM();
    virtual ~VM();
    void defineNative(const char* name, NativeFn function);
    /* A Virtual Machine interpret-h < Scanning on Demand vm-interpret-h
    InterpretResult interpret(Chunk* chunk);
    */
    //> Scanning on Demand vm-interpret-h
    InterpretResult interpret(const char* source);
    //< Scanning on Demand vm-interpret-h
    //> push-pop
    void push(Value value);
    Value pop();
    //< push-pop
    ObjFunction* newFunction();
    ObjString* newString(std::string chars);
    ObjString* newString(const char* chars, int length);
};

//< interpret-result
//> Strings extern-vm

#endif
