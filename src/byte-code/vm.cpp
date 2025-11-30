//> A Virtual Machine vm-c
//> Types of Values include-stdarg
#include <stdarg.h>
//< Types of Values include-stdarg
//> vm-include-stdio
#include <stdio.h>
//> Strings vm-include-string
#include <string.h>
//< Strings vm-include-string
//> Calls and Functions vm-include-time
#include <time.h>
//< Calls and Functions vm-include-time

//< vm-include-stdio
#include "common.hpp"
//> Scanning on Demand vm-include-compiler
#include "compiler.hpp"
//< Scanning on Demand vm-include-compiler
//> vm-include-debug
#include "debug.hpp"
//< vm-include-debug
//> Strings vm-include-object-memory
#include "object.hpp"
//< Strings vm-include-object-memory
#include "log.hpp"
#include "vm.hpp"

#define GC_HEAP_GROW_FACTOR 2
//< Garbage Collection heap-grow-factor

void VM::collect(size_t oldSize, size_t newSize)
{
    //> Garbage Collection updated-bytes-allocated
    bytesAllocated += newSize - oldSize;
    //< Garbage Collection updated-bytes-allocated
    //> Garbage Collection call-collect
    if (newSize > oldSize) {
#ifdef DEBUG_STRESS_GC
        collectGarbage();
#endif
        //> collect-on-next

        if (bytesAllocated > nextGC) {
            collectGarbage();
        }
        //< collect-on-next
    }
}

//> Garbage Collection mark-object
void VM::markObject(Obj* object)
{
    if (object == NULL)
        return;
    //> check-is-marked
    if (object->isMarked)
        return;

//< check-is-marked
//> log-mark-object
#ifdef DEBUG_LOG_GC
    printf("%p mark ", (void*)object);
    ValueUtil util;
    util.print(OBJ_VAL(object));
    printf("\n");
#endif

    //< log-mark-object
    object->isMarked = true;
    //> add-to-gray-stack

    grayStack.push(object);
    //< add-to-gray-stack
}
//< Garbage Collection mark-object
//> Garbage Collection mark-value
void VM::markValue(Value value)
{
    if (IS_OBJ(value))
        markObject(AS_OBJ(value));
}
//< Garbage Collection mark-value
//> Garbage Collection mark-array
void VM::markArray(std::vector<Value>& array)
{
    for (size_t i = 0; i < array.size(); i++) {
        markValue(array[i]);
    }
}
//< Garbage Collection mark-array
//> Garbage Collection blacken-object
void VM::blackenObject(Obj* object)
{
//> log-blacken-object
#ifdef DEBUG_LOG_GC
    printf("%p blacken ", (void*)object);
    ValueUtil util;
    util.print(OBJ_VAL(object));
    printf("\n");
#endif

    //< log-blacken-object
    switch (object->type) {
        //> Methods and Initializers blacken-bound-method
    case OBJ_BOUND_METHOD: {
        ObjBoundMethod* bound = (ObjBoundMethod*)object;
        markValue(bound->receiver);
        markObject((Obj*)bound->method);
        break;
    }
        //< Methods and Initializers blacken-bound-method
        //> Classes and Instances blacken-class
    case OBJ_CLASS: {
        ObjClass* klass = (ObjClass*)object;
        //> Methods and Initializers mark-methods
        klass->methods.mark([=](Value value) {
            markValue(value);
        });
        //< Methods and Initializers mark-methods
        break;
    }
        //< Classes and Instances blacken-class
        //> blacken-closure
    case OBJ_CLOSURE: {
        ObjClosure* closure = (ObjClosure*)object;
        markObject((Obj*)closure->function);
        for (int i = 0; i < closure->upvalueCount; i++) {
            markObject((Obj*)closure->upvalues[i]);
        }
        break;
    }
        //< blacken-closure
        //> blacken-function
    case OBJ_FUNCTION: {
        ObjFunction* function = (ObjFunction*)object;
        markArray(function->chunk.constants);
        break;
    }
        //< blacken-function
        //> Classes and Instances blacken-instance
    case OBJ_INSTANCE: {
        ObjInstance* instance = (ObjInstance*)object;
        markObject((Obj*)instance->klass);
        instance->fields.mark([=](Value value) {
            markValue(value);
        });
        break;
    }
        //< Classes and Instances blacken-instance
        //> blacken-upvalue
    case OBJ_UPVALUE:
        markValue(((ObjUpvalue*)object)->closed);
        break;
        //< blacken-upvalue
    case OBJ_NATIVE:
    case OBJ_STRING:
        break;
    }
}
//< Garbage Collection blacken-object
//> Garbage Collection mark-roots
void VM::markRoots()
{
    for (Value* slot = stack; slot < stackTop; slot++) {
        markValue(*slot);
    }
    //> mark-closures

    for (int i = 0; i < frameCount; i++) {
        markObject((Obj*)frames[i].closure);
    }
    //< mark-closures
    //> mark-open-upvalues

    for (ObjUpvalue* upvalue = openUpvalues; upvalue != NULL; upvalue = upvalue->next) {
        markObject((Obj*)upvalue);
    }
    //< mark-open-upvalues
    //> mark-globals

    globals.mark([=](Value value) {
        markValue(value);
    });
    //< mark-globals
    //> call-mark-compiler-roots
    compiler->markCompilerRoots([=](Obj* obj) {
        markObject(obj);
    });
    //< call-mark-compiler-roots
    //> Methods and Initializers mark-init-string
    markObject((Obj*)initString);
    //< Methods and Initializers mark-init-string
}
//< Garbage Collection mark-roots
//> Garbage Collection trace-references
void VM::traceReferences()
{
    while (!grayStack.empty()) {
        Obj* object = grayStack.top();
        grayStack.pop();
        blackenObject(object);
    }
}
//< Garbage Collection trace-references
//> Garbage Collection sweep
void VM::sweep()
{
    Obj* previous = NULL;
    Obj* pObject = objects;
    while (pObject != NULL) {
        if (pObject->isMarked) {
            //> unmark
            pObject->isMarked = false;
            //< unmark
            previous = pObject;
            pObject = pObject->next;
        }
        else {
            Obj* unreached = pObject;
            pObject = pObject->next;
            if (previous != NULL) {
                previous->next = pObject;
            }
            else {
                objects = pObject;
            }

            delete unreached;
        }
    }
}
//< Garbage Collection sweep
void VM::removeString(void)
{
    for (std::map<std::string, ObjString*>::iterator it = strings.begin(); it != strings.end();) {
        if (!it->second->isMarked) {
            it = strings.erase(it);
        }
        else {
            it++;
        }
    }
}
//> Garbage Collection collect-garbage
void VM::collectGarbage()
{
//> log-before-collect
#ifdef DEBUG_LOG_GC
    printf("-- gc begin\n");
    //> log-before-size
    size_t before = bytesAllocated;
//< log-before-size
#endif
    //< log-before-collect
    //> call-mark-roots

    markRoots();
    //< call-mark-roots
    //> call-trace-references
    traceReferences();
    //< call-trace-references
    removeString();
    //> call-sweep
    sweep();
    //< call-sweep
    //> update-next-gc

    nextGC = bytesAllocated * GC_HEAP_GROW_FACTOR;
    //< update-next-gc
    //> log-after-collect

#ifdef DEBUG_LOG_GC
    printf("-- gc end\n");
    //> log-collected-amount
    printf("   collected %zu bytes (from %zu to %zu) next at %zu\n", before - bytesAllocated, before, bytesAllocated,
           nextGC);
//< log-collected-amount
#endif
    //< log-after-collect
}

//> Calls and Functions clock-native
static Value clockNative(int argCount, Value* args)
{
    (void)argCount;
    (void)args;
    return NUMBER_VAL((double)clock() / CLOCKS_PER_SEC);
}
//< Calls and Functions clock-native
//> reset-stack
void VM::resetStack()
{
    stackTop = stack;
    //> Calls and Functions reset-frame-count
    frameCount = 0;
    //< Calls and Functions reset-frame-count
    //> Closures init-open-upvalues
    openUpvalues = NULL;
    //< Closures init-open-upvalues
}
//< reset-stack
//> Types of Values runtime-error
void VM::runtimeError(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    /* Types of Values runtime-error < Calls and Functions runtime-error-temp
      size_t instruction = ip - chunk->code - 1;
      int line = chunk->lines[instruction];
    */
    /* Calls and Functions runtime-error-temp < Calls and Functions runtime-error-stack
      CallFrame* frame = &frames[frameCount - 1];
      size_t instruction = frame->ip - frame->function->chunk.code - 1;
      int line = frame->function->chunk.lines[instruction];
    */
    /* Types of Values runtime-error < Calls and Functions runtime-error-stack
      fprintf(stderr, "[line %d] in script\n", line);
    */
    //> Calls and Functions runtime-error-stack
    for (int i = frameCount - 1; i >= 0; i--) {
        CallFrame* frame = &frames[i];
        /* Calls and Functions runtime-error-stack < Closures runtime-error-function
            ObjFunction* function = frame->function;
        */
        //> Closures runtime-error-function
        ObjFunction* function = frame->closure->function;
        //< Closures runtime-error-function
        size_t instruction = frame->ip - function->chunk.code - 1;
        fprintf(stderr, "[line %d] in ", // [minus]
                function->chunk.getLine(instruction));
        if (function->name.empty()) {
            fprintf(stderr, "script\n");
        }
        else {
            fprintf(stderr, "%s()\n", function->name.c_str());
        }
    }

    //< Calls and Functions runtime-error-stack
    resetStack();
}
//< Types of Values runtime-error
//> Calls and Functions define-native
void VM::defineNative(const char* name, NativeFn function)
{
    push(OBJ_VAL(newString(name)));
    push(OBJ_VAL(newNative(function)));
    globals.set(AS_STRING(stack[0])->chars, stack[1]);
    pop();
    pop();
}
//< Calls and Functions define-native

VM::VM()
{
    compiler = CompilerFactory::instance()->create(this);
    //> call-reset-stack
    resetStack();
    //< call-reset-stack
    //> Strings init-objects-root
    objects = NULL;
    //< Strings init-objects-root
    //> Garbage Collection init-gc-fields
    bytesAllocated = 0;
    nextGC = 1024 * 1024;
    //< Garbage Collection init-gc-fields
    //> Hash Tables init-strings
    // initTable(&strings);
    //< Hash Tables init-strings
    //> Methods and Initializers init-init-string

    initString = newString("init");
    //< Methods and Initializers init-init-string
    //> Calls and Functions define-native-clock

    defineNative("clock", clockNative);
    //< Calls and Functions define-native-clock
}

VM::~VM()
{
    //> Hash Tables free-strings
    // freeTable(&strings);
    //< Hash Tables free-strings
    //> Methods and Initializers clear-init-string
    initString = NULL;
    //< Methods and Initializers clear-init-string
    //> Strings call-free-objects
    freeObjects();
    //< Strings call-free-objects
}
//> push
void VM::push(Value value)
{
    *stackTop = value;
    stackTop++;
}
//< push
//> pop
Value VM::pop()
{
    stackTop--;
    return *stackTop;
}
//< pop
//> Types of Values peek
Value VM::peek(int distance)
{
    return stackTop[-1 - distance];
}
//< Types of Values peek
/* Calls and Functions call < Closures call-signature
static bool call(ObjFunction* function, int argCount) {
*/
//> Calls and Functions call
//> Closures call-signature
bool VM::call(ObjClosure* closure, int argCount)
{
    //< Closures call-signature
    /* Calls and Functions check-arity < Closures check-arity
      if (argCount != function->arity) {
        runtimeError("Expected %d arguments but got %d.",
            function->arity, argCount);
    */
    //> Closures check-arity
    if (argCount != closure->function->arity) {
        runtimeError("Expected %d arguments but got %d.", closure->function->arity, argCount);
        //< Closures check-arity
        //> check-arity
        return false;
    }

    //< check-arity
    //> check-overflow
    if (frameCount == FRAMES_MAX) {
        runtimeError("Stack overflow.");
        return false;
    }

    //< check-overflow
    CallFrame* frame = &frames[frameCount++];
    /* Calls and Functions call < Closures call-init-closure
      frame->function = function;
      frame->ip = function->chunk.code;
    */
    //> Closures call-init-closure
    frame->closure = closure;
    frame->ip = closure->function->chunk.code;
    //< Closures call-init-closure
    frame->slots = stackTop - argCount - 1;
    return true;
}
//< Calls and Functions call
//> Calls and Functions call-value
bool VM::callValue(Value callee, int argCount)
{
    if (IS_OBJ(callee)) {
        switch (OBJ_TYPE(callee)) {
            //> Methods and Initializers call-bound-method
        case OBJ_BOUND_METHOD: {
            ObjBoundMethod* bound = AS_BOUND_METHOD(callee);
            //> store-receiver
            stackTop[-argCount - 1] = bound->receiver;
            //< store-receiver
            return call(bound->method, argCount);
        }
            //< Methods and Initializers call-bound-method
            //> Classes and Instances call-class
        case OBJ_CLASS: {
            ObjClass* klass = AS_CLASS(callee);
            stackTop[-argCount - 1] = OBJ_VAL(newInstance(klass));
            //> Methods and Initializers call-init
            Value initializer;
            if (klass->methods.get(initString->chars, &initializer)) {
                return call(AS_CLOSURE(initializer), argCount);
                //> no-init-arity-error
            }
            else if (argCount != 0) {
                runtimeError("Expected 0 arguments but got %d.", argCount);
                return false;
                //< no-init-arity-error
            }
            //< Methods and Initializers call-init
            return true;
        }
            //< Classes and Instances call-class
            //> Closures call-value-closure
        case OBJ_CLOSURE:
            return call(AS_CLOSURE(callee), argCount);
            //< Closures call-value-closure
            /* Calls and Functions call-value < Closures call-value-closure
                  case OBJ_FUNCTION: // [switch]
                    return call(AS_FUNCTION(callee), argCount);
            */
            //> call-native
        case OBJ_NATIVE: {
            NativeFn native = AS_NATIVE(callee);
            Value result = native(argCount, stackTop - argCount);
            stackTop -= argCount + 1;
            push(result);
            return true;
        }
            //< call-native
        default:
            break; // Non-callable object type.
        }
    }
    runtimeError("Can only call functions and classes.");
    return false;
}
//< Calls and Functions call-value
//> Methods and Initializers invoke-from-class
bool VM::invokeFromClass(ObjClass* klass, ObjString* name, int argCount)
{
    Value method;
    if (!klass->methods.get(name->chars, &method)) {
        runtimeError("Undefined property '%s'.", name->chars.c_str());
        return false;
    }
    return call(AS_CLOSURE(method), argCount);
}
//< Methods and Initializers invoke-from-class
//> Methods and Initializers invoke
bool VM::invoke(ObjString* name, int argCount)
{
    Value receiver = peek(argCount);
    //> invoke-check-type

    if (!IS_INSTANCE(receiver)) {
        runtimeError("Only instances have methods.");
        return false;
    }

    //< invoke-check-type
    ObjInstance* instance = AS_INSTANCE(receiver);
    //> invoke-field

    Value value;
    if (instance->fields.get(name->chars, &value)) {
        stackTop[-argCount - 1] = value;
        return callValue(value, argCount);
    }

    //< invoke-field
    return invokeFromClass(instance->klass, name, argCount);
}
//< Methods and Initializers invoke
//> Methods and Initializers new-bound-method
ObjBoundMethod* VM::newBoundMethod(Value receiver, ObjClosure* method)
{
    collect(0, sizeof(ObjBoundMethod));
    ObjBoundMethod* ret = new ObjBoundMethod(receiver, method);
    ret->next = objects;
    objects = ret;
    return ret;
}
//< Methods and Initializers new-bound-method
//> Classes and Instances new-class
ObjClass* VM::newClass(std::string name)
{
    collect(0, sizeof(ObjClass));
    ObjClass* ret = new ObjClass(name);
    ret->next = objects;
    objects = ret;
    return ret;
}
//< Classes and Instances new-class
//> Closures new-closure
ObjClosure* VM::newClosure(ObjFunction* function)
{
    collect(0, sizeof(ObjClosure));
    ObjClosure* ret = new ObjClosure(function);
    ret->next = objects;
    objects = ret;
    return ret;
}
//< Closures new-closure
//> Classes and Instances new-instance
ObjInstance* VM::newInstance(ObjClass* klass)
{
    collect(0, sizeof(ObjInstance));
    ObjInstance* ret = new ObjInstance(klass);
    ret->next = objects;
    objects = ret;
    return ret;
}
//< Classes and Instances new-instance
//> Calls and Functions new-native
ObjNative* VM::newNative(NativeFn function)
{
    collect(0, sizeof(ObjNative));
    ObjNative* ret = new ObjNative(function);
    ret->next = objects;
    objects = ret;
    return ret;
}
//< Calls and Functions new-native
//> Closures new-upvalue
ObjUpvalue* VM::newUpvalue(Value* slot)
{
    collect(0, sizeof(ObjUpvalue));
    ObjUpvalue* ret = new ObjUpvalue(slot);
    ret->next = openUpvalues;

    return ret;
}
//< Closures new-upvalue
//> Calls and Functions new-function
ObjFunction* VM::newFunction()
{
    collect(0, sizeof(ObjFunction));
    ObjFunction* ret = new ObjFunction();
    ret->next = objects;
    objects = ret;
    return ret;
}
//< Calls and Functions new-function

/* Strings allocate-string < Hash Tables allocate-string
static ObjString* allocateString(char* chars, int length) {
*/
//> allocate-string
//> Hash Tables allocate-string
ObjString* VM::allocateString(std::string chars)
{
    collect(0, sizeof(ObjString));
    ObjString* ret = new ObjString(chars);
    push(OBJ_VAL(ret));
    ret->next = objects;
    objects = ret;
    //< Garbage Collection push-string
    strings[chars] = ret;
    LAX_LOG("%s is registered", chars.c_str());
    //> Garbage Collection pop-string
    pop();
    return ret;
}
//< allocate-string
//> take-string
ObjString* VM::newString(std::string chars)
{
    //> take-string-intern
    std::map<std::string, ObjString*>::iterator found = strings.find(chars);
    ObjString* interned = (found == strings.end()) ? NULL : found->second;
    if (interned != NULL) {
        LAX_LOG("%s is found", chars.c_str());
        return interned;
    }
    LAX_LOG("%s is not found", chars.c_str());

    //< take-string-intern
    return allocateString(chars);
}
//< take-string
ObjString* VM::newString(const char* chars, int length)
{
    std::string str(chars, length);
    return newString(str);
}

//> Methods and Initializers bind-method
bool VM::bindMethod(ObjClass* klass, ObjString* name)
{
    Value method;
    if (!klass->methods.get(name->chars, &method)) {
        runtimeError("Undefined property '%s'.", name->chars.c_str());
        return false;
    }

    ObjBoundMethod* bound = newBoundMethod(peek(0), AS_CLOSURE(method));
    pop();
    push(OBJ_VAL(bound));
    return true;
}
//< Methods and Initializers bind-method
//> Closures capture-upvalue
ObjUpvalue* VM::captureUpvalue(Value* local)
{
    //> look-for-existing-upvalue
    ObjUpvalue* prevUpvalue = NULL;
    ObjUpvalue* upvalue = openUpvalues;
    while (upvalue != NULL && upvalue->location > local) {
        prevUpvalue = upvalue;
        upvalue = upvalue->next;
    }

    if (upvalue != NULL && upvalue->location == local) {
        return upvalue;
    }

    //< look-for-existing-upvalue
    ObjUpvalue* createdUpvalue = newUpvalue(local);
    //> insert-upvalue-in-list
    createdUpvalue->next = upvalue;

    if (prevUpvalue == NULL) {
        openUpvalues = createdUpvalue;
    }
    else {
        prevUpvalue->next = createdUpvalue;
    }

    //< insert-upvalue-in-list
    return createdUpvalue;
}
//< Closures capture-upvalue
//> Closures close-upvalues
void VM::closeUpvalues(Value* last)
{
    while (openUpvalues != NULL && openUpvalues->location >= last) {
        ObjUpvalue* upvalue = openUpvalues;
        upvalue->closed = *upvalue->location;
        upvalue->location = &upvalue->closed;
        openUpvalues = upvalue->next;
    }
}
//< Closures close-upvalues
//> Methods and Initializers define-method
void VM::defineMethod(ObjString* name)
{
    Value method = peek(0);
    ObjClass* klass = AS_CLASS(peek(1));
    klass->methods.set(name->chars, method);
    pop();
}
//< Methods and Initializers define-method
//> Types of Values is-falsey
bool VM::isFalsey(Value value)
{
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}
//< Types of Values is-falsey
//> Strings concatenate
void VM::concatenate()
{
    /* Strings concatenate < Garbage Collection concatenate-peek
      ObjString* b = AS_STRING(pop());
      ObjString* a = AS_STRING(pop());
    */
    //> Garbage Collection concatenate-peek
    ObjString* b = AS_STRING(peek(0));
    ObjString* a = AS_STRING(peek(1));
    //< Garbage Collection concatenate-peek

    std::string chars = a->chars;
    chars += b->chars;

    ObjString* result = newString(chars);
    //> Garbage Collection concatenate-pop
    pop();
    pop();
    //< Garbage Collection concatenate-pop
    push(OBJ_VAL(result));
}
//< Strings concatenate
//> run
InterpretResult VM::run(void)
{
    //> Calls and Functions run
    CallFrame* frame = &frames[frameCount - 1];

/* A Virtual Machine run < Calls and Functions run
#define READ_BYTE() (*ip++)
*/
#define READ_BYTE() (*frame->ip++)
/* A Virtual Machine read-constant < Calls and Functions run
#define READ_CONSTANT() (chunk->constants.values[READ_BYTE()])
*/

/* Jumping Back and Forth read-short < Calls and Functions run
#define READ_SHORT() \
    (ip += 2, (uint16_t)((ip[-2] << 8) | ip[-1]))
*/
#define READ_SHORT() (frame->ip += 2, (uint16_t)((frame->ip[-2] << 8) | frame->ip[-1]))

/* Calls and Functions run < Closures read-constant
#define READ_CONSTANT() \
    (frame->function->chunk.constants.values[READ_BYTE()])
*/
//> Closures read-constant
#define READ_CONSTANT() (frame->closure->function->chunk.constants[READ_BYTE()])
//< Closures read-constant

//< Calls and Functions run
//> Global Variables read-string
#define READ_STRING() AS_STRING(READ_CONSTANT())
//< Global Variables read-string
/* A Virtual Machine binary-op < Types of Values binary-op
#define BINARY_OP(op) \
    do { \
      double b = pop(); \
      double a = pop(); \
      push(a op b); \
    } while (false)
*/
//> Types of Values binary-op
#define BINARY_OP(valueType, op)                                                                                       \
    do {                                                                                                               \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {                                                              \
            runtimeError("Operands must be numbers.");                                                                 \
            return INTERPRET_RUNTIME_ERROR;                                                                            \
        }                                                                                                              \
        double b = AS_NUMBER(pop());                                                                                   \
        double a = AS_NUMBER(pop());                                                                                   \
        push(valueType(a op b));                                                                                       \
    } while (false)
    //< Types of Values binary-op

    for (;;) {
//> trace-execution
#ifdef DEBUG_TRACE_EXECUTION
        //> trace-stack
        printf("          ");
        ValueUtil util;
        for (Value* slot = stack; slot < stackTop; slot++) {
            printf("[ ");
            util.print(*slot);
            printf(" ]");
        }
        printf("\n");
        //< trace-stack
        /* A Virtual Machine trace-execution < Calls and Functions trace-execution
            disassembleInstruction(chunk,
                                   (int)(ip - chunk->code));
        */
        /* Calls and Functions trace-execution < Closures disassemble-instruction
            disassembleInstruction(&frame->function->chunk,
                (int)(frame->ip - frame->function->chunk.code));
        */
        //> Closures disassemble-instruction
        debug.disassembleInstruction(&frame->closure->function->chunk,
                                     (int)(frame->ip - frame->closure->function->chunk.code));
//< Closures disassemble-instruction
#endif

        //< trace-execution
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            //> op-constant
        case OP_CONSTANT: {
            Value constant = READ_CONSTANT();
            /* A Virtual Machine op-constant < A Virtual Machine push-constant
                    printValue(constant);
                    printf("\n");
            */
            //> push-constant
            push(constant);
            //< push-constant
            break;
        }
            //< op-constant
            //> Types of Values interpret-literals
        case OP_NIL:
            push(NIL_VAL);
            break;
        case OP_TRUE:
            push(BOOL_VAL(true));
            break;
        case OP_FALSE:
            push(BOOL_VAL(false));
            break;
            //< Types of Values interpret-literals
            //> Global Variables interpret-pop
        case OP_POP:
            pop();
            break;
            //< Global Variables interpret-pop
            //> Local Variables interpret-get-local
        case OP_GET_LOCAL: {
            uint8_t slot = READ_BYTE();
            /* Local Variables interpret-get-local < Calls and Functions push-local
                    push(stack[slot]); // [slot]
            */
            //> Calls and Functions push-local
            push(frame->slots[slot]);
            //< Calls and Functions push-local
            break;
        }
            //< Local Variables interpret-get-local
            //> Local Variables interpret-set-local
        case OP_SET_LOCAL: {
            uint8_t slot = READ_BYTE();
            /* Local Variables interpret-set-local < Calls and Functions set-local
                    stack[slot] = peek(0);
            */
            //> Calls and Functions set-local
            frame->slots[slot] = peek(0);
            //< Calls and Functions set-local
            break;
        }
            //< Local Variables interpret-set-local
            //> Global Variables interpret-get-global
        case OP_GET_GLOBAL: {
            ObjString* name = READ_STRING();
            Value value;
            if (!globals.get(name->chars, &value)) {
                runtimeError("Undefined variable '%s'.", name->chars.c_str());
                return INTERPRET_RUNTIME_ERROR;
            }
            push(value);
            break;
        }
            //< Global Variables interpret-get-global
            //> Global Variables interpret-define-global
        case OP_DEFINE_GLOBAL: {
            ObjString* name = READ_STRING();
            globals.set(name->chars, peek(0));
            pop();
            break;
        }
            //< Global Variables interpret-define-global
            //> Global Variables interpret-set-global
        case OP_SET_GLOBAL: {
            ObjString* name = READ_STRING();
            if (globals.set(name->chars, peek(0))) {
                globals.remove(name->chars); // [delete]
                runtimeError("Undefined variable '%s'.", name->chars.c_str());
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
            //< Global Variables interpret-set-global
            //> Closures interpret-get-upvalue
        case OP_GET_UPVALUE: {
            uint8_t slot = READ_BYTE();
            push(*frame->closure->upvalues[slot]->location);
            break;
        }
            //< Closures interpret-get-upvalue
            //> Closures interpret-set-upvalue
        case OP_SET_UPVALUE: {
            uint8_t slot = READ_BYTE();
            *frame->closure->upvalues[slot]->location = peek(0);
            break;
        }
            //< Closures interpret-set-upvalue
            //> Classes and Instances interpret-get-property
        case OP_GET_PROPERTY: {
            //> get-not-instance
            if (!IS_INSTANCE(peek(0))) {
                runtimeError("Only instances have properties.");
                return INTERPRET_RUNTIME_ERROR;
            }

            //< get-not-instance
            ObjInstance* instance = AS_INSTANCE(peek(0));
            ObjString* name = READ_STRING();

            Value value;
            if (instance->fields.get(name->chars, &value)) {
                pop(); // Instance.
                push(value);
                break;
            }
            //> get-undefined

            //< get-undefined
            /* Classes and Instances get-undefined < Methods and Initializers get-method
                    runtimeError("Undefined property '%s'.", name->chars);
                    return INTERPRET_RUNTIME_ERROR;
            */
            //> Methods and Initializers get-method
            if (!bindMethod(instance->klass, name)) {
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
            //< Methods and Initializers get-method
        }
            //< Classes and Instances interpret-get-property
            //> Classes and Instances interpret-set-property
        case OP_SET_PROPERTY: {
            //> set-not-instance
            if (!IS_INSTANCE(peek(1))) {
                runtimeError("Only instances have fields.");
                return INTERPRET_RUNTIME_ERROR;
            }

            //< set-not-instance
            ObjInstance* instance = AS_INSTANCE(peek(1));
            instance->fields.set(READ_STRING()->chars, peek(0));
            Value value = pop();
            pop();
            push(value);
            break;
        }
            //< Classes and Instances interpret-set-property
            //> Superclasses interpret-get-super
        case OP_GET_SUPER: {
            ObjString* name = READ_STRING();
            ObjClass* superclass = AS_CLASS(pop());

            if (!bindMethod(superclass, name)) {
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
            //< Superclasses interpret-get-super
            //> Types of Values interpret-equal
        case OP_EQUAL: {
            Value b = pop();
            Value a = pop();
            ValueUtil util;
            push(BOOL_VAL(util.equal(a, b)));
            break;
        }
            //< Types of Values interpret-equal
            //> Types of Values interpret-comparison
        case OP_GREATER:
            BINARY_OP(BOOL_VAL, >);
            break;
        case OP_LESS:
            BINARY_OP(BOOL_VAL, <);
            break;
            //< Types of Values interpret-comparison
            /* A Virtual Machine op-binary < Types of Values op-arithmetic
                  case OP_ADD:      BINARY_OP(+); break;
                  case OP_SUBTRACT: BINARY_OP(-); break;
                  case OP_MULTIPLY: BINARY_OP(*); break;
                  case OP_DIVIDE:   BINARY_OP(/); break;
            */
            /* A Virtual Machine op-negate < Types of Values op-negate
                  case OP_NEGATE:   push(-pop()); break;
            */
            /* Types of Values op-arithmetic < Strings add-strings
                  case OP_ADD:      BINARY_OP(NUMBER_VAL, +); break;
            */
            //> Strings add-strings
        case OP_ADD: {
            if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
                concatenate();
            }
            else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
                double b = AS_NUMBER(pop());
                double a = AS_NUMBER(pop());
                push(NUMBER_VAL(a + b));
            }
            else {
                runtimeError("Operands must be two numbers or two strings.");
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
            //< Strings add-strings
            //> Types of Values op-arithmetic
        case OP_SUBTRACT:
            BINARY_OP(NUMBER_VAL, -);
            break;
        case OP_MULTIPLY:
            BINARY_OP(NUMBER_VAL, *);
            break;
        case OP_DIVIDE:
            BINARY_OP(NUMBER_VAL, /);
            break;
            //< Types of Values op-arithmetic
            //> Types of Values op-not
        case OP_NOT:
            push(BOOL_VAL(isFalsey(pop())));
            break;
            //< Types of Values op-not
            //> Types of Values op-negate
        case OP_NEGATE:
            if (!IS_NUMBER(peek(0))) {
                runtimeError("Operand must be a number.");
                return INTERPRET_RUNTIME_ERROR;
            }
            push(NUMBER_VAL(-AS_NUMBER(pop())));
            break;
            //< Types of Values op-negate
            //> Global Variables interpret-print
        case OP_PRINT: {
            ValueUtil util;
            util.print(pop());
            printf("\n");
            break;
        }
            //< Global Variables interpret-print
            //> Jumping Back and Forth op-jump
        case OP_JUMP: {
            uint16_t offset = READ_SHORT();
            /* Jumping Back and Forth op-jump < Calls and Functions jump
                    ip += offset;
            */
            //> Calls and Functions jump
            frame->ip += offset;
            //< Calls and Functions jump
            break;
        }
            //< Jumping Back and Forth op-jump
            //> Jumping Back and Forth op-jump-if-false
        case OP_JUMP_IF_FALSE: {
            uint16_t offset = READ_SHORT();
            /* Jumping Back and Forth op-jump-if-false < Calls and Functions jump-if-false
                    if (isFalsey(peek(0))) ip += offset;
            */
            //> Calls and Functions jump-if-false
            if (isFalsey(peek(0)))
                frame->ip += offset;
            //< Calls and Functions jump-if-false
            break;
        }
            //< Jumping Back and Forth op-jump-if-false
            //> Jumping Back and Forth op-loop
        case OP_LOOP: {
            uint16_t offset = READ_SHORT();
            /* Jumping Back and Forth op-loop < Calls and Functions loop
                    ip -= offset;
            */
            //> Calls and Functions loop
            frame->ip -= offset;
            //< Calls and Functions loop
            break;
        }
            //< Jumping Back and Forth op-loop
            //> Calls and Functions interpret-call
        case OP_CALL: {
            int argCount = READ_BYTE();
            if (!callValue(peek(argCount), argCount)) {
                return INTERPRET_RUNTIME_ERROR;
            }
            //> update-frame-after-call
            frame = &frames[frameCount - 1];
            //< update-frame-after-call
            break;
        }
            //< Calls and Functions interpret-call
            //> Methods and Initializers interpret-invoke
        case OP_INVOKE: {
            ObjString* method = READ_STRING();
            int argCount = READ_BYTE();
            if (!invoke(method, argCount)) {
                return INTERPRET_RUNTIME_ERROR;
            }
            frame = &frames[frameCount - 1];
            break;
        }
            //< Methods and Initializers interpret-invoke
            //> Superclasses interpret-super-invoke
        case OP_SUPER_INVOKE: {
            ObjString* method = READ_STRING();
            int argCount = READ_BYTE();
            ObjClass* superclass = AS_CLASS(pop());
            if (!invokeFromClass(superclass, method, argCount)) {
                return INTERPRET_RUNTIME_ERROR;
            }
            frame = &frames[frameCount - 1];
            break;
        }
            //< Superclasses interpret-super-invoke
            //> Closures interpret-closure
        case OP_CLOSURE: {
            ObjFunction* function = AS_FUNCTION(READ_CONSTANT());
            ObjClosure* closure = newClosure(function);
            push(OBJ_VAL(closure));
            //> interpret-capture-upvalues
            for (int i = 0; i < closure->upvalueCount; i++) {
                uint8_t isLocal = READ_BYTE();
                uint8_t index = READ_BYTE();
                if (isLocal) {
                    closure->upvalues[i] = captureUpvalue(frame->slots + index);
                }
                else {
                    closure->upvalues[i] = frame->closure->upvalues[index];
                }
            }
            //< interpret-capture-upvalues
            break;
        }
            //< Closures interpret-closure
            //> Closures interpret-close-upvalue
        case OP_CLOSE_UPVALUE:
            closeUpvalues(stackTop - 1);
            pop();
            break;
            //< Closures interpret-close-upvalue
        case OP_RETURN: {
            /* A Virtual Machine print-return < Global Variables op-return
                    printValue(pop());
                    printf("\n");
            */
            /* Global Variables op-return < Calls and Functions interpret-return
                    // Exit interpreter.
            */
            /* A Virtual Machine run < Calls and Functions interpret-return
                    return INTERPRET_OK;
            */
            //> Calls and Functions interpret-return
            Value result = pop();
            //> Closures return-close-upvalues
            closeUpvalues(frame->slots);
            //< Closures return-close-upvalues
            frameCount--;
            if (frameCount == 0) {
                pop();
                return INTERPRET_OK;
            }

            stackTop = frame->slots;
            push(result);
            frame = &frames[frameCount - 1];
            break;
            //< Calls and Functions interpret-return
        }
            //> Classes and Instances interpret-class
        case OP_CLASS:
            push(OBJ_VAL(newClass(READ_STRING()->chars)));
            break;
            //< Classes and Instances interpret-class
            //> Superclasses interpret-inherit
        case OP_INHERIT: {
            Value superclass = peek(1);
            //> inherit-non-class
            if (!IS_CLASS(superclass)) {
                runtimeError("Superclass must be a class.");
                return INTERPRET_RUNTIME_ERROR;
            }

            //< inherit-non-class
            ObjClass* subclass = AS_CLASS(peek(0));
            subclass->methods.addAll(AS_CLASS(superclass)->methods);
            pop(); // Subclass.
            break;
        }
            //< Superclasses interpret-inherit
            //> Methods and Initializers interpret-method
        case OP_METHOD:
            defineMethod(READ_STRING());
            break;
            //< Methods and Initializers interpret-method
        }
    }

#undef READ_BYTE
//> Jumping Back and Forth undef-read-short
#undef READ_SHORT
//< Jumping Back and Forth undef-read-short
//> undef-read-constant
#undef READ_CONSTANT
//< undef-read-constant
//> Global Variables undef-read-string
#undef READ_STRING
//< Global Variables undef-read-string
//> undef-binary-op
#undef BINARY_OP
    //< undef-binary-op
}
//< run
//> omit
void VM::hack(bool b)
{
    // Hack to avoid unused function error. run() is not used in the
    // scanning chapter.
    run();
    if (b)
        hack(false);
}
//< omit
//> interpret
/* A Virtual Machine interpret < Scanning on Demand vm-interpret-c
InterpretResult VM::interpret(Chunk* chunk) {
  chunk = chunk;
  ip = chunk->code;
  return run();
*/
//> Scanning on Demand vm-interpret-c
InterpretResult VM::interpret(const char* source)
{
    /* Scanning on Demand vm-interpret-c < Compiling Expressions interpret-chunk
      compile(source);
      return INTERPRET_OK;
    */
    /* Compiling Expressions interpret-chunk < Calls and Functions interpret-stub
      Chunk chunk;
      initChunk(&chunk);

      if (!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
      }

      chunk = &chunk;
      ip = chunk->code;
    */
    //> Calls and Functions interpret-stub
    ObjFunction* function = compiler->compile(source);
    if (function == NULL)
        return INTERPRET_COMPILE_ERROR;

    push(OBJ_VAL(function));
    //< Calls and Functions interpret-stub
    /* Calls and Functions interpret-stub < Calls and Functions interpret
      CallFrame* frame = &frames[frameCount++];
      frame->function = function;
      frame->ip = function->chunk.code;
      frame->slots = stack;
    */
    /* Calls and Functions interpret < Closures interpret
      call(function, 0);
    */
    //> Closures interpret
    ObjClosure* closure = newClosure(function);
    pop();
    push(OBJ_VAL(closure));
    call(closure, 0);
    //< Closures interpret
    //< Scanning on Demand vm-interpret-c
    //> Compiling Expressions interpret-chunk

    /* Compiling Expressions interpret-chunk < Calls and Functions end-interpret
      InterpretResult result = run();

      freeChunk(&chunk);
      return result;
    */
    //> Calls and Functions end-interpret
    return run();
    //< Calls and Functions end-interpret
    //< Compiling Expressions interpret-chunk
}
//> Strings free-objects
void VM::freeObjects()
{
    Obj* object = objects;
    while (object != NULL) {
        Obj* next = object->next;
        delete object;
        object = next;
    }
    //> Garbage Collection free-gray-stack

    //< Garbage Collection free-gray-stack
}
//< Strings free-objects
//< interpret
