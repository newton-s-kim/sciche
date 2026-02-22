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
#include "dl.hpp"
#include "primitive.hpp"

#include <map>
#include <stack>
#include <string>
#include <unordered_map>

#define THREAD_MAX 4
// clang-format off
//> interpret-result

namespace sce {
typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR = 65,
  INTERPRET_RUNTIME_ERROR = 70
} InterpretResult;
// clang-format on

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
    ObjThread* thread;
    nsl::vector<ObjThread*> threadStack;

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
    std::unordered_map<std::string, ObjString*> strings;
    //< Hash Tables strings
    Primitive primitive;

private:
    bool callValue(Value callee, int argCount);
    bool bindMethod(ObjClass* klass, ObjString* name);
    InterpretResult run(void);
    bool invoke(ObjString* name, int argCount);
    void defineMethod(ObjString* name);
    void hack(bool b);
    //> Strings free-objects-h
    void freeObjects();
    //< Strings free-objects-h

    ObjBoundMethod* newBoundMethod(Value receiver, ObjClosure* method);
    ObjClass* newClass(nsl::string name);
    ObjClosure* newClosure(ObjFunction* function);
    ObjInstance* newInstance(ObjClass* klass);
    ObjNative* newNative(NativeFn function);
    ObjUpvalue* newUpvalue(Value* slot);
    ObjString* allocateString(std::string chars);

    void markArray(nsl::vector<Value>& array);
    void blackenObject(Obj* object);
    void markRoots();
    void traceReferences();
    void sweep();
    void collectGarbage();
    void markObject(Obj* object);
    void markValue(Value value);
    void removeString(void);
    void collect(size_t oldSize, size_t newSize);
    std::unordered_map<std::string, dl*> m_dl;

public:
    //< Strings extern-vm
    VM();
    virtual ~VM();
    void defineNative(const char* name, NativeFn function);
    void defineSymbol(const char* name, NativeClass* klass);
    void defineNumber(const char* name, double v);
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
    // ObjString* newString(std::string& chars);
    ObjString* newString(const char* chars);
    ObjString* newString(const char* chars, int length);
    ObjComplex* newComplex(const std::complex<double> cv);
    ObjList* newList(void);
    ObjMap* newMap(void);
    ObjCol* newCol(size_t size = 0, ObjFillType fill_type = OBJ_FILL_DEFAULT);
    ObjRow* newRow(size_t size = 0, ObjFillType fill_type = OBJ_FILL_DEFAULT);
    ObjMat* newMat(size_t rows = 0, size_t cols = 0, ObjFillType fill_type = OBJ_FILL_DEFAULT);
    ObjCube* newCube(size_t rows = 0, size_t cols = 0, size_t depth = 0, ObjFillType fill_type = OBJ_FILL_DEFAULT);
    ObjNativeClass* newNativeClass(NativeClass* klass);
    ObjNativeObject* newNativeObj(NativeObject* klass);
    ObjThread* newThread(void);
    ObjThread* newThread(ObjClosure* closure);
    bool loadLibrary(std::string path, std::string name);
    bool callFunction(Value value, int argc, Value* argv, bool startNew);
};

//< interpret-result
//> Strings extern-vm
} // namespace sce
#endif
