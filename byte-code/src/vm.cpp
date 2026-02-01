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

#include <iostream>
#include <sstream>
#include <stack>

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
    case OBJ_COMPLEX:
        markObject(object);
        break;
    case OBJ_LIST: {
        ObjList* list = (ObjList*)object;
        for (size_t i = 0; i < list->container.size(); i++) {
            markValue(list->container[i]);
        }
        break;
    }
    case OBJ_MAP: {
        ObjMap* map = (ObjMap*)object;
        for (std::map<std::string, Value>::iterator it = map->container.begin(); it != map->container.end(); it++) {
            markValue(it->second);
        }
        break;
    }
    case OBJ_NATIVE:
    case OBJ_STRING:
    case OBJ_COL:
    case OBJ_ROW:
    case OBJ_MAT:
    case OBJ_CUBE:
    case OBJ_NATIVE_CLASS:
    case OBJ_NATIVE_OBJ:
        break;
    case OBJ_THREAD: {
        ObjThread* thd = (ObjThread*)object;
        // TODO:refactoring
        for (Value* slot = thd->stack; slot < thd->stackTop; slot++) {
            markValue(*slot);
        }
        //> mark-closures

        for (int i = 0; i < thd->frameCount; i++) {
            markObject((Obj*)thd->frames[i].closure);
        }
        break;
    }
    }
}
//< Garbage Collection blacken-object
//> Garbage Collection mark-roots
void VM::markRoots()
{
    for (ObjThread* thd = thread; thd; thd = thd->caller) {
        markObject(thd);
    }
    for (std::vector<ObjThread*>::iterator it = threadStack.begin(); it != threadStack.end(); it++) {
        markObject(*it);
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
static Value clockNative(ObjectFactory* factory, int argCount, Value* args)
{
    (void)factory;
    (void)argCount;
    (void)args;
    return NUMBER_VAL((double)clock() / CLOCKS_PER_SEC);
}
static Value rangeNative(ObjectFactory* factory, int argCount, Value* args)
{
    double start = 0;
    double end = 0;
    double step = 1;
    if (3 == argCount) {
        if (!IS_NUMBER(args[0]))
            throw std::runtime_error("number is expected.");
        if (!IS_NUMBER(args[1]))
            throw std::runtime_error("number is expected.");
        if (!IS_NUMBER(args[2]))
            throw std::runtime_error("number is expected.");
        start = AS_NUMBER(args[0]);
        end = AS_NUMBER(args[1]);
        step = AS_NUMBER(args[2]);
    }
    else if (2 == argCount) {
        if (!IS_NUMBER(args[0]))
            throw std::runtime_error("number is expected.");
        if (!IS_NUMBER(args[1]))
            throw std::runtime_error("number is expected.");
        start = AS_NUMBER(args[0]);
        end = AS_NUMBER(args[1]);
    }
    else if (1 == argCount) {
        if (!IS_NUMBER(args[0]))
            throw std::runtime_error("number is expected.");
        end = AS_NUMBER(args[0]);
    }
    else {
        throw std::runtime_error("invalid number of arguments.");
    }
    ObjList* list = factory->newList();
    for (double v = start; v <= end; v += step) {
        list->container.push_back(NUMBER_VAL(v));
    }
    return OBJ_VAL(list);
}
/*
static Value listNative(ObjectFactory* factory, int argCount, Value* args)
{
    ObjList* list = factory->newList();
    for (int argc = 0; argc < argCount; argc++)
        list->container.push_back(args[argc]);
    return OBJ_VAL(list);
}
*/
static Value mapNative(ObjectFactory* factory, int argCount, Value* args)
{
    (void)argCount;
    (void)args;
    ObjMap* map = factory->newMap();
    return OBJ_VAL(map);
}
static Value threadNative(ObjectFactory* factory, int argCount, Value* args)
{
    if (1 != argCount)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_CLOSURE(args[0]))
        throw std::runtime_error("Closure is expected.");
    ObjThread* thread = factory->newThread(AS_CLOSURE(args[0]));
    return OBJ_VAL(thread);
}

static Value rowVecNative(ObjectFactory* factory, int argc, Value* args)
{
    ObjFillType fill_type = OBJ_FILL_DEFAULT;
    size_t sz = 0;
    switch (argc) {
    case 2:
        if (!IS_NUMBER(args[1])) {
            throw std::runtime_error("number is expected");
        }
        fill_type = (ObjFillType)AS_NUMBER(args[1]);
        if (!IS_NUMBER(args[0])) {
            throw std::runtime_error("number is expected");
        }
        sz = AS_NUMBER(args[0]);
        break;
    case 1:
        if (!IS_NUMBER(args[0])) {
            throw std::runtime_error("number is expected");
        }
        sz = AS_NUMBER(args[0]);
        break;
    default:
        throw std::runtime_error("invalid arguments");
        break;
    }
    return OBJ_VAL(factory->newRow(sz, fill_type));
}

static Value cubeNative(ObjectFactory* factory, int argc, Value* args)
{
    ObjFillType fill_type = OBJ_FILL_DEFAULT;
    size_t rows = 0, cols = 0, depth = 0;
    switch (argc) {
    case 4:
        if (!IS_NUMBER(args[3])) {
            throw std::runtime_error("number is expected");
        }
        fill_type = (ObjFillType)AS_NUMBER(args[3]);
        if (!IS_NUMBER(args[0]))
            throw std::runtime_error("number is expected");
        if (!IS_NUMBER(args[1]))
            throw std::runtime_error("number is expected");
        if (!IS_NUMBER(args[2]))
            throw std::runtime_error("number is expected");
        rows = AS_NUMBER(args[0]);
        cols = AS_NUMBER(args[1]);
        depth = AS_NUMBER(args[2]);
        break;
    case 3:
        if (!IS_NUMBER(args[0]))
            throw std::runtime_error("number is expected");
        if (!IS_NUMBER(args[1]))
            throw std::runtime_error("number is expected");
        if (!IS_NUMBER(args[2]))
            throw std::runtime_error("number is expected");
        rows = AS_NUMBER(args[0]);
        cols = AS_NUMBER(args[1]);
        depth = AS_NUMBER(args[2]);
        break;
    default:
        throw std::runtime_error("invalid arguments");
        break;
    }
    return OBJ_VAL(factory->newCube(rows, cols, depth, fill_type));
}

//< Calls and Functions clock-native
//> reset-stack
void VM::resetStack()
{
    thread->stackTop = thread->stack;
    //> Calls and Functions reset-frame-count
    thread->frameCount = 0;
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
    for (int i = thread->frameCount - 1; i >= 0; i--) {
        CallFrame* frame = &thread->frames[i];
        /* Calls and Functions runtime-error-stack < Closures runtime-error-function
            ObjFunction* function = frame->function;
        */
        //> Closures runtime-error-function
        ObjFunction* function = thread->frames[thread->frameCount - 1].closure->function;
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
    globals.set(AS_STRING(thread->stack[0])->chars, thread->stack[1]);
    pop();
    pop();
}
void VM::defineSymbol(const char* name, NativeClass* klass)
{
    push(OBJ_VAL(newString(name)));
    push(OBJ_VAL(newNativeClass(klass)));
    globals.set(AS_STRING(thread->stack[0])->chars, thread->stack[1]);
    pop();
    pop();
}
//< Calls and Functions define-native
void VM::defineNumber(const char* name, double v)
{
    push(OBJ_VAL(newString(name)));
    push(NUMBER_VAL(v));
    globals.set(AS_STRING(thread->stack[0])->chars, thread->stack[1]);
    pop();
    pop();
}

VM::VM() : thread(NULL), openUpvalues(NULL), objects(NULL)
{
    thread = new ObjThread();
    compiler = CompilerFactory::instance()->create(this);
    //> call-reset-stack
    resetStack();
    //< call-reset-stack
    //> Strings init-objects-root
    objects = thread;
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
    defineNative("range", rangeNative);
    defineSymbol("List", new listNative);
    defineNative("Map", mapNative);
    defineSymbol("vec", new vecNative);
    defineNative("rowvec", rowVecNative);
    defineSymbol("mat", new matNative);
    defineNative("cube", cubeNative);
    defineNative("thread", threadNative);
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
    for (std::map<std::string, dl*>::iterator it = m_dl.begin(); it != m_dl.end(); it++) {
        delete it->second;
    }
}
//> push
void VM::push(Value value)
{
    *thread->stackTop = value;
    thread->stackTop++;
}
//< push
//> pop
Value VM::pop()
{
    thread->stackTop--;
    return *thread->stackTop;
}
//< pop
//> Types of Values peek
Value VM::peek(int distance)
{
    return thread->stackTop[-1 - distance];
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
    if (thread->frameCount == FRAMES_MAX) {
        runtimeError("Stack overflow.");
        return false;
    }

    //< check-overflow
    CallFrame* frame = &thread->frames[thread->frameCount++];
    /* Calls and Functions call < Closures call-init-closure
      frame->function = function;
      frame->ip = function->chunk.code;
    */
    //> Closures call-init-closure
    frame->closure = closure;
    frame->ip = closure->function->chunk.code;
    //< Closures call-init-closure
    frame->slots = thread->stackTop - argCount - 1;
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
            thread->stackTop[-argCount - 1] = bound->receiver;
            //< store-receiver
            return call(bound->method, argCount);
        }
            //< Methods and Initializers call-bound-method
            //> Classes and Instances call-class
        case OBJ_CLASS: {
            ObjClass* klass = AS_CLASS(callee);
            thread->stackTop[-argCount - 1] = OBJ_VAL(newInstance(klass));
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
            Value result = 0;
            try {
                result = native(this, argCount, thread->stackTop - argCount);
            }
            catch (std::exception& e) {
                runtimeError(e.what());
                return false;
            }
            thread->stackTop -= argCount + 1;
            push(result);
            return true;
        }
        case OBJ_NATIVE_CLASS: {
            ObjNativeClass* native = AS_NATIVE_CLASS(callee);
            Value result = 0;
            try {
                result = native->klass->call(this, argCount, thread->stackTop - argCount);
            }
            catch (std::exception& e) {
                runtimeError(e.what());
                return false;
            }
            thread->stackTop -= argCount + 1;
            push(result);
            return true;
        }
            //< call-native
        default:
            LAX_LOG("non-callable type %d.", OBJ_TYPE(callee));
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

    if (IS_INSTANCE(receiver)) {
        //< invoke-check-type
        ObjInstance* instance = AS_INSTANCE(receiver);
        //> invoke-field

        Value value;
        if (instance->fields.get(name->chars, &value)) {
            thread->stackTop[-argCount - 1] = value;
            return callValue(value, argCount);
        }

        //< invoke-field
        return invokeFromClass(instance->klass, name, argCount);
    }
    else if (IS_NATIVE_CLASS(receiver)) {
        ObjNativeClass* klass = AS_NATIVE_CLASS(receiver);
        Value result = 0;
        try {
            result = klass->klass->invoke(this, name->chars, argCount, thread->stackTop - argCount);
        }
        catch (std::exception& e) {
            runtimeError(e.what());
            return false;
        }
        for (int i = 0; i < argCount; i++)
            pop();
        pop();
        push(result);
        return true;
    }
    else if (IS_NATIVE_OBJECT(receiver)) {
        ObjNativeObject* nobj = AS_NATIVE_OBJECT(receiver);
        Value result = 0;
        try {
            result = nobj->object->invoke(this, name->chars, argCount, thread->stackTop - argCount);
        }
        catch (std::exception& e) {
            runtimeError(e.what());
            return false;
        }
        for (int i = 0; i < argCount; i++)
            pop();
        pop();
        push(result);
        return true;
    }
    else {
        Value result = 0;
        try {
            result = primitive.call(this, receiver, name->chars, argCount, thread->stackTop - argCount);
        }
        catch (std::exception& e) {
            runtimeError(e.what());
            return false;
        }
        for (int i = 0; i < argCount; i++)
            pop();
        pop();
        push(result);
        return true;
    }
    return false;
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
ObjList* VM::newList(void)
{
    collect(0, sizeof(ObjList));
    ObjList* ret = new ObjList();
    ret->next = objects;
    objects = ret;
    return ret;
}
ObjMap* VM::newMap(void)
{
    collect(0, sizeof(ObjMap));
    ObjMap* ret = new ObjMap();
    ret->next = objects;
    objects = ret;
    return ret;
}
ObjCol* VM::newCol(size_t size, ObjFillType fill_type)
{
    collect(0, sizeof(ObjCol));
    ObjCol* ret = new ObjCol();
    if (0 < size) {
        ret->value.resize(size);
        switch (fill_type) {
        case OBJ_FILL_ZEROS:
            ret->value.zeros();
            break;
        case OBJ_FILL_ONES:
            ret->value.ones();
            break;
        case OBJ_FILL_RANDU:
            ret->value.randu();
            break;
        case OBJ_FILL_RANDN:
            ret->value.randn();
            break;
        case OBJ_FILL_EYE:
            ret->value.eye();
            break;
        case OBJ_FILL_DEFAULT:
            break;
        default:
            runtimeError("invalid fill type");
            break;
        }
    }
    ret->next = objects;
    objects = ret;
    return ret;
}
ObjRow* VM::newRow(size_t size, ObjFillType fill_type)
{
    collect(0, sizeof(ObjRow));
    ObjRow* ret = new ObjRow();
    if (0 < size) {
        ret->value.resize(size);
        switch (fill_type) {
        case OBJ_FILL_ZEROS:
            ret->value.zeros();
            break;
        case OBJ_FILL_ONES:
            ret->value.ones();
            break;
        case OBJ_FILL_RANDU:
            ret->value.randu();
            break;
        case OBJ_FILL_RANDN:
            ret->value.randn();
            break;
        case OBJ_FILL_EYE:
            ret->value.eye();
            break;
        case OBJ_FILL_DEFAULT:
            break;
        default:
            runtimeError("invalid fill type");
            break;
        }
    }
    ret->next = objects;
    objects = ret;
    return ret;
}
ObjMat* VM::newMat(size_t rows, size_t cols, ObjFillType fill_type)
{
    collect(0, sizeof(ObjMat));
    ObjMat* ret = new ObjMat();
    if (0 < rows && 0 < cols) {
        ret->value.resize(rows, cols);
        switch (fill_type) {
        case OBJ_FILL_ZEROS:
            ret->value.zeros();
            break;
        case OBJ_FILL_ONES:
            ret->value.ones();
            break;
        case OBJ_FILL_RANDU:
            ret->value.randu();
            break;
        case OBJ_FILL_RANDN:
            ret->value.randn();
            break;
        case OBJ_FILL_EYE:
            ret->value.eye();
            break;
        case OBJ_FILL_DEFAULT:
            break;
        default:
            runtimeError("invalid fill type");
            break;
        }
    }
    ret->next = objects;
    objects = ret;
    return ret;
}
ObjCube* VM::newCube(size_t rows, size_t cols, size_t depth, ObjFillType fill_type)
{
    collect(0, sizeof(ObjCube));
    ObjCube* ret = new ObjCube();
    if (0 < rows && 0 < cols && 0 < depth) {
        ret->value.resize(rows, cols, depth);
        switch (fill_type) {
        case OBJ_FILL_ZEROS:
            ret->value.zeros();
            break;
        case OBJ_FILL_ONES:
            ret->value.ones();
            break;
        case OBJ_FILL_RANDU:
            ret->value.randu();
            break;
        case OBJ_FILL_RANDN:
            ret->value.randn();
            break;
        case OBJ_FILL_DEFAULT:
            break;
        default:
            runtimeError("invalid fill type");
            break;
        }
    }
    ret->next = objects;
    objects = ret;
    return ret;
}
ObjNativeClass* VM::newNativeClass(NativeClass* klass)
{
    collect(0, sizeof(ObjNativeClass));
    ObjNativeClass* ret = new ObjNativeClass();
    ret->klass = klass;
    ret->next = objects;
    objects = ret;
    return ret;
}
ObjNativeObject* VM::newNativeObj(NativeObject* obj)
{
    collect(0, sizeof(ObjNativeObject));
    ObjNativeObject* ret = new ObjNativeObject();
    ret->object = obj;
    ret->next = objects;
    objects = ret;
    return ret;
}
ObjThread* VM::newThread(void)
{
    collect(0, sizeof(ObjThread));
    ObjThread* ret = new ObjThread();
    ret->next = objects;
    objects = ret;
    return ret;
}
ObjThread* VM::newThread(ObjClosure* closure)
{
    collect(0, sizeof(ObjThread));
    ObjThread* ret = new ObjThread(closure);
    ret->next = objects;
    objects = ret;
    return ret;
}
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
ObjString* VM::newString(std::string pchars)
{
    std::string chars;
    for (const char* p = pchars.c_str(); *p; p++) {
        // LAX_LOG("p: %c", *p);
        if ('\\' == *p) {
            p++;
            switch (*p) {
            case '"':
                chars += '"';
                break;
            case '\\':
                chars += '\\';
                break;
            case '%':
                chars += '%';
                break;
            case '0':
                chars += '\0';
                break;
            case 'a':
                chars += '\a';
                break;
            case 'b':
                chars += '\b';
                break;
            case 'e':
                chars += '\e';
                break;
            case 'f':
                chars += '\f';
                break;
            case 'n':
                chars += '\n';
                break;
            case 'r':
                chars += '\r';
                break;
            case 't':
                chars += '\t';
                break;
            }
        }
        else {
            chars += *p;
        }
    }
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
ObjComplex* VM::newComplex(const std::complex<double> v)
{
    collect(0, sizeof(ObjComplex));
    ObjComplex* ret = new ObjComplex(v);
    push(OBJ_VAL(ret));
    ret->next = objects;
    objects = ret;
    pop();
    return ret;
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
    // CallFrame* frame = &thread->frames[frameCount - 1];

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
        CallFrame* frame = &thread->frames[thread->frameCount - 1];
//> trace-execution
#ifdef DEBUG_TRACE_EXECUTION
        //> trace-stack
        printf("          ");
        ValueUtil util;
        for (Value* slot = thread->stack; slot < thread->stackTop; slot++) {
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
            if (IS_INSTANCE(peek(0))) {
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
            }
            else if (IS_NATIVE_CLASS(peek(0))) {
                ObjNativeClass* klass = AS_NATIVE_CLASS(peek(0));
                ObjString* name = READ_STRING();
                Value result = 0;
                try {
                    result = klass->klass->constant(this, name->chars);
                    pop();
                    push(result);
                }
                catch (std::exception& e) {
                    runtimeError(e.what());
                    return INTERPRET_RUNTIME_ERROR;
                }
            }
            else if (IS_NATIVE_OBJECT(peek(0))) {
                ObjNativeObject* nobj = AS_NATIVE_OBJECT(peek(0));
                ObjString* name = READ_STRING();
                Value result = 0;
                try {
                    result = nobj->object->property(this, name->chars);
                    pop();
                    push(result);
                }
                catch (std::exception& e) {
                    runtimeError(e.what());
                    return INTERPRET_RUNTIME_ERROR;
                }
            }
            else {
                ObjString* name = READ_STRING();
                Value result = 0;
                try {
                    result = primitive.property(this, peek(0), name->chars);
                    pop();
                    push(result);
                }
                catch (std::exception& e) {
                    runtimeError(e.what());
                    return INTERPRET_RUNTIME_ERROR;
                }
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
        case OP_GET_ELEMENT: {
            int argCount = READ_BYTE();
            Value value;
            if (1 == argCount) {
                if (IS_NUMBER(peek(0))) {
                    double index = AS_NUMBER(peek(0));
                    if (IS_LIST(peek(1))) {
                        ObjList* list = AS_LIST(peek(1));
                        try {
                            value = list->get((int)index);
                        }
                        catch (std::exception& e) {
                            runtimeError(e.what());
                            return INTERPRET_RUNTIME_ERROR;
                        }
                    }
                    else if (IS_COL(peek(1))) {
                        ObjCol* col = AS_COL(peek(1));
                        try {
                            value = col->get((int)index);
                        }
                        catch (std::exception& e) {
                            runtimeError(e.what());
                            return INTERPRET_RUNTIME_ERROR;
                        }
                    }
                    else if (IS_ROW(peek(1))) {
                        ObjRow* row = AS_ROW(peek(1));
                        try {
                            value = row->get((int)index);
                        }
                        catch (std::exception& e) {
                            runtimeError(e.what());
                            return INTERPRET_RUNTIME_ERROR;
                        }
                    }
                    else {
                        runtimeError("Only container has elements.");
                        return INTERPRET_RUNTIME_ERROR;
                    }
                }
                else if (IS_STRING(peek(0))) {
                    ObjString* index = AS_STRING(peek(0));
                    if (IS_MAP(peek(1))) {
                        ObjMap* map = AS_MAP(peek(1));
                        try {
                            value = map->get(index->chars);
                        }
                        catch (std::exception& e) {
                            runtimeError(e.what());
                            return INTERPRET_RUNTIME_ERROR;
                        }
                    }
                    else {
                        runtimeError("Only container has elements.");
                        return INTERPRET_RUNTIME_ERROR;
                    }
                }
                else {
                    runtimeError("Index is not number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                pop();
                pop();
            }
            else if (2 == argCount) {
                if (!IS_MAT(peek(2))) {
                    runtimeError("Only container has elements.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
                    runtimeError("Index is not number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                ObjMat* mat = AS_MAT(peek(2));
                int row = AS_NUMBER(peek(1));
                int col = AS_NUMBER(peek(0));
                value = mat->get(row, col);
                pop();
                pop();
                pop();
            }
            else if (3 == argCount) {
                if (!IS_CUBE(peek(3))) {
                    runtimeError("Only container has elements.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1)) || !IS_NUMBER(peek(2))) {
                    runtimeError("Index is not number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                ObjCube* cube = AS_CUBE(peek(3));
                int row = AS_NUMBER(peek(2));
                int col = AS_NUMBER(peek(1));
                int depth = AS_NUMBER(peek(0));
                value = cube->get(row, col, depth);
                pop();
                pop();
                pop();
                pop();
            }
            else {
                runtimeError("Invalid index");
                return INTERPRET_RUNTIME_ERROR;
            }
            push(value);
            break;
        }
        case OP_SET_ELEMENT: {
            int argCount = READ_BYTE();
            if (1 == argCount) {
                if (IS_NUMBER(peek(1))) {
                    double index = AS_NUMBER(peek(1));
                    if (IS_LIST(peek(2))) {
                        ObjList* list = AS_LIST(peek(2));
                        try {
                            list->set((int)index, peek(0));
                        }
                        catch (std::exception& e) {
                            runtimeError(e.what());
                            return INTERPRET_RUNTIME_ERROR;
                        }
                    }
                    else if (IS_COL(peek(2))) {
                        if (!IS_NUMBER(peek(0))) {
                            runtimeError("number is expected");
                            return INTERPRET_RUNTIME_ERROR;
                        }
                        ObjCol* col = AS_COL(peek(2));
                        try {
                            col->set((int)index, peek(0));
                        }
                        catch (std::exception& e) {
                            runtimeError(e.what());
                            return INTERPRET_RUNTIME_ERROR;
                        }
                    }
                    else if (IS_ROW(peek(2))) {
                        if (!IS_NUMBER(peek(0))) {
                            runtimeError("number is expected");
                            return INTERPRET_RUNTIME_ERROR;
                        }
                        ObjRow* row = AS_ROW(peek(2));
                        try {
                            row->set((int)index, peek(0));
                        }
                        catch (std::exception& e) {
                            runtimeError(e.what());
                            return INTERPRET_RUNTIME_ERROR;
                        }
                    }
                    else {
                        runtimeError("Only container has elements.");
                        return INTERPRET_RUNTIME_ERROR;
                    }
                }
                else if (IS_STRING(peek(1))) {
                    ObjString* index = AS_STRING(peek(1));
                    if (IS_MAP(peek(2))) {
                        ObjMap* map = AS_MAP(peek(2));
                        try {
                            map->set(index->chars, peek(0));
                        }
                        catch (std::exception& e) {
                            runtimeError(e.what());
                            return INTERPRET_RUNTIME_ERROR;
                        }
                    }
                    else {
                        runtimeError("Only container has elements.");
                        return INTERPRET_RUNTIME_ERROR;
                    }
                }
                else {
                    runtimeError("Index is not number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                pop();
                pop();
            }
            else if (2 == argCount) {
                if (!IS_MAT(peek(3))) {
                    runtimeError("Only container has elements.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                if (!IS_NUMBER(peek(2)) || !IS_NUMBER(peek(1))) {
                    runtimeError("Index is not number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                ObjMat* mat = AS_MAT(peek(3));
                int row = AS_NUMBER(peek(2));
                int col = AS_NUMBER(peek(1));
                mat->set(row, col, peek(0));
                pop();
                pop();
                pop();
            }
            else if (3 == argCount) {
                if (!IS_CUBE(peek(4))) {
                    runtimeError("Only container has elements.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                if (!IS_NUMBER(peek(3)) || !IS_NUMBER(peek(2)) || !IS_NUMBER(peek(1))) {
                    runtimeError("Index is not number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                ObjCube* cube = AS_CUBE(peek(4));
                int row = AS_NUMBER(peek(3));
                int col = AS_NUMBER(peek(2));
                int depth = AS_NUMBER(peek(1));
                cube->set(row, col, depth, peek(0));
                pop();
                pop();
                pop();
                pop();
            }
            else {
                runtimeError("Invalid index");
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
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
            bool calculated = true;
            if (IS_STRING(peek(1))) {
                if (IS_STRING(peek(0))) {
                    concatenate();
                }
                else if (IS_NUMBER(peek(0))) {
                    ObjString* a = AS_STRING(peek(1));
                    double b = AS_NUMBER(peek(0));
                    pop();
                    pop();
                    std::stringstream ss;
                    ss << a->chars << b;
                    ObjString* c = newString(ss.str());
                    push(OBJ_VAL(c));
                }
                else if (IS_COMPLEX(peek(0))) {
                    ObjString* a = AS_STRING(peek(1));
                    ObjComplex* b = AS_COMPLEX(peek(0));
                    pop();
                    pop();
                    ObjString* c = newString(a->chars + b->stringify());
                    push(OBJ_VAL(c));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_NUMBER(peek(1))) {
                if (IS_NUMBER(peek(0))) {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a + b));
                }
                else if (IS_COL(peek(0))) {
                    double a = AS_NUMBER(peek(1));
                    ObjCol* b = AS_COL(peek(0));
                    pop();
                    pop();
                    ObjCol* c = newCol();
                    c->value = a + b->value;
                    push(OBJ_VAL(c));
                }
                else if (IS_COMPLEX(peek(0))) {
                    double a = AS_NUMBER(peek(1));
                    ObjComplex* b = AS_COMPLEX(peek(0));
                    pop();
                    pop();
                    ObjComplex* c = newComplex(std::complex<double>(a + b->value.real(), b->value.imag()));
                    push(OBJ_VAL(c));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_COMPLEX(peek(1))) {
                if (IS_NUMBER(peek(0))) {
                    ObjComplex* a = AS_COMPLEX(peek(1));
                    double b = AS_NUMBER(peek(0));
                    pop();
                    pop();
                    ObjComplex* c = newComplex(std::complex<double>(b + a->value.real(), a->value.imag()));
                    push(OBJ_VAL(c));
                }
                else if (IS_COMPLEX(peek(0))) {
                    ObjComplex* a = AS_COMPLEX(peek(1));
                    ObjComplex* b = AS_COMPLEX(peek(0));
                    pop();
                    pop();
                    ObjComplex* c = newComplex(std::complex<double>(a->value + b->value));
                    push(OBJ_VAL(c));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_COL(peek(1))) {
                if (IS_COL(peek(0))) {
                    ObjCol* a = AS_COL(peek(1));
                    ObjCol* b = AS_COL(peek(0));
                    pop();
                    pop();
                    ObjCol* c = newCol();
                    c->value = a->value + b->value;
                    push(OBJ_VAL(c));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_ROW(peek(1))) {
                if (IS_ROW(peek(0))) {
                    ObjRow* a = AS_ROW(peek(1));
                    ObjRow* b = AS_ROW(peek(0));
                    pop();
                    pop();
                    ObjRow* r = newRow();
                    r->value = a->value + b->value;
                    push(OBJ_VAL(r));
                }
                else if (IS_MAT(peek(0))) {
                    ObjRow* a = AS_ROW(peek(1));
                    ObjMat* b = AS_MAT(peek(0));
                    pop();
                    pop();
                    ObjRow* r = newRow();
                    r->value = a->value + b->value;
                    push(OBJ_VAL(r));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_MAT(peek(1))) {
                if (IS_MAT(peek(0))) {
                    ObjMat* a = AS_MAT(peek(1));
                    ObjMat* b = AS_MAT(peek(0));
                    pop();
                    pop();
                    ObjMat* m = newMat();
                    m->value = a->value + b->value;
                    push(OBJ_VAL(m));
                }
                else {
                    calculated = false;
                }
            }
            else {
                calculated = false;
            }
            if (!calculated) {
                runtimeError("Operands must be two numbers or two strings.");
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
            //< Strings add-strings
            //> Types of Values op-arithmetic
        case OP_SUBTRACT: {
            bool calculated = true;
            if (IS_NUMBER(peek(1))) {
                if (IS_NUMBER(peek(0))) {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a - b));
                }
                else if (IS_COMPLEX(peek(0))) {
                    double a = AS_NUMBER(peek(1));
                    ObjComplex* b = AS_COMPLEX(peek(0));
                    pop();
                    pop();
                    ObjComplex* c = newComplex(std::complex<double>(a - b->value.real(), -b->value.imag()));
                    push(OBJ_VAL(c));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_COMPLEX(peek(1))) {
                if (IS_NUMBER(peek(0))) {
                    ObjComplex* a = AS_COMPLEX(peek(1));
                    double b = AS_NUMBER(peek(0));
                    pop();
                    pop();
                    ObjComplex* c = newComplex(std::complex<double>(a->value.real() - b, a->value.imag()));
                    push(OBJ_VAL(c));
                }
                else if (IS_COMPLEX(peek(0))) {
                    ObjComplex* a = AS_COMPLEX(peek(1));
                    ObjComplex* b = AS_COMPLEX(peek(0));
                    pop();
                    pop();
                    ObjComplex* c = newComplex(std::complex<double>(a->value - b->value));
                    push(OBJ_VAL(c));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_ROW(peek(1))) {
                if (IS_ROW(peek(0))) {
                    ObjRow* a = AS_ROW(peek(1));
                    ObjRow* b = AS_ROW(peek(0));
                    pop();
                    pop();
                    ObjRow* r = newRow();
                    r->value = a->value - b->value;
                    push(OBJ_VAL(r));
                }
                else if (IS_MAT(peek(0))) {
                    ObjRow* a = AS_ROW(peek(1));
                    ObjMat* b = AS_MAT(peek(0));
                    pop();
                    pop();
                    ObjRow* r = newRow();
                    r->value = a->value - b->value;
                    push(OBJ_VAL(r));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_MAT(peek(1))) {
                if (IS_MAT(peek(0))) {
                    ObjMat* a = AS_MAT(peek(1));
                    ObjMat* b = AS_MAT(peek(0));
                    pop();
                    pop();
                    ObjMat* m = newMat();
                    m->value = a->value - b->value;
                    push(OBJ_VAL(m));
                }
                else {
                    calculated = false;
                }
            }
            else {
                calculated = false;
            }
            if (!calculated) {
                runtimeError("Operands must be numbers.");
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
        case OP_MULTIPLY: {
            bool calculated = true;
            if (IS_NUMBER(peek(1))) {
                if (IS_NUMBER(peek(0))) {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a * b));
                }
                else if (IS_COMPLEX(peek(0))) {
                    double a = AS_NUMBER(peek(1));
                    ObjComplex* b = AS_COMPLEX(peek(0));
                    pop();
                    pop();
                    ObjComplex* c = newComplex(std::complex<double>(a * b->value.real(), a * b->value.imag()));
                    push(OBJ_VAL(c));
                }
                else if (IS_COL(peek(0))) {
                    double a = AS_NUMBER(peek(1));
                    ObjCol* b = AS_COL(peek(0));
                    pop();
                    pop();
                    ObjCol* c = newCol();
                    c->value = a * b->value;
                    push(OBJ_VAL(c));
                }
                else if (IS_MAT(peek(0))) {
                    double a = AS_NUMBER(peek(1));
                    ObjMat* b = AS_MAT(peek(0));
                    pop();
                    pop();
                    ObjMat* c = newMat();
                    c->value = a * b->value;
                    push(OBJ_VAL(c));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_COL(peek(1))) {
                if (IS_NUMBER(peek(0))) {
                    ObjCol* a = AS_COL(peek(1));
                    double b = AS_NUMBER(peek(0));
                    pop();
                    pop();
                    ObjCol* c = newCol();
                    c->value = a->value * b;
                    push(OBJ_VAL(c));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_COMPLEX(peek(1))) {
                if (IS_NUMBER(peek(0))) {
                    ObjComplex* a = AS_COMPLEX(peek(1));
                    double b = AS_NUMBER(peek(0));
                    pop();
                    pop();
                    ObjComplex* c = newComplex(std::complex<double>(a->value.real() * b, a->value.imag() * b));
                    push(OBJ_VAL(c));
                }
                else if (IS_COMPLEX(peek(0))) {
                    ObjComplex* a = AS_COMPLEX(peek(1));
                    ObjComplex* b = AS_COMPLEX(peek(0));
                    pop();
                    pop();
                    ObjComplex* c = newComplex(std::complex<double>(a->value * b->value));
                    push(OBJ_VAL(c));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_MAT(peek(1))) {
                if (IS_MAT(peek(0))) {
                    ObjMat* a = AS_MAT(peek(1));
                    ObjMat* b = AS_MAT(peek(0));
                    pop();
                    pop();
                    ObjMat* m = newMat();
                    m->value = a->value * b->value;
                    push(OBJ_VAL(m));
                }
                else {
                    calculated = false;
                }
            }
            else {
                calculated = false;
            }
            if (!calculated) {
                runtimeError("Operands must be numbers.");
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
        case OP_DIVIDE: {
            bool calculated = true;
            if (IS_NUMBER(peek(1))) {
                if (IS_NUMBER(peek(0))) {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a / b));
                }
                else if (IS_COMPLEX(peek(0))) {
                    double a = AS_NUMBER(peek(1));
                    ObjComplex* b = AS_COMPLEX(peek(0));
                    pop();
                    pop();
                    std::complex<double> t(a, 0);
                    ObjComplex* c = newComplex(std::complex<double>(t / b->value));
                    push(OBJ_VAL(c));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_COL(peek(1))) {
                if (IS_NUMBER(peek(0))) {
                    ObjCol* a = AS_COL(peek(1));
                    double b = AS_NUMBER(peek(0));
                    pop();
                    pop();
                    ObjCol* c = newCol();
                    c->value = a->value / b;
                    push(OBJ_VAL(c));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_COMPLEX(peek(1))) {
                if (IS_NUMBER(peek(0))) {
                    ObjComplex* a = AS_COMPLEX(peek(1));
                    double b = AS_NUMBER(peek(0));
                    pop();
                    pop();
                    std::complex<double> t(b, 0);
                    ObjComplex* c = newComplex(std::complex<double>(a->value / t));
                    push(OBJ_VAL(c));
                }
                else if (IS_COMPLEX(peek(0))) {
                    ObjComplex* a = AS_COMPLEX(peek(1));
                    ObjComplex* b = AS_COMPLEX(peek(0));
                    pop();
                    pop();
                    ObjComplex* c = newComplex(std::complex<double>(a->value / b->value));
                    push(OBJ_VAL(c));
                }
                else {
                    calculated = false;
                }
            }
            else {
                calculated = false;
            }
            if (!calculated) {
                runtimeError("Operands must be numbers.");
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
        case OP_MODULO: {
            bool calculated = true;
            if (IS_NUMBER(peek(1))) {
                if (IS_NUMBER(peek(0))) {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    LAX_LOG("fmod(%f, %f) is %f", a, b, fmod(a, b));
                    push(NUMBER_VAL(fmod(a, b)));
                }
                else {
                    calculated = false;
                }
            }
            else if (IS_COL(peek(1))) {
                if (IS_COL(peek(0))) {
                    ObjCol* a = AS_COL(peek(1));
                    ObjCol* b = AS_COL(peek(0));
                    pop();
                    pop();
                    ObjCol* c = newCol();
                    c->value = a->value % b->value;
                    push(OBJ_VAL(c));
                }
                else {
                    calculated = false;
                }
            }
            else {
                calculated = false;
            }
            if (!calculated) {
                runtimeError("Operands must be numbers.");
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        } //< Types of Values op-arithmetic
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
            frame = &thread->frames[thread->frameCount - 1];
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
            frame = &thread->frames[thread->frameCount - 1];
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
            frame = &thread->frames[thread->frameCount - 1];
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
            closeUpvalues(thread->stackTop - 1);
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
            thread->frameCount--;
            if (thread->frameCount == 0) {
                if (NULL == thread->caller) {
                    pop();
                    if (!threadStack.empty()) {
                        thread = threadStack.back();
                        threadStack.pop_back();
                    }
                    return INTERPRET_OK;
                }
                else {
                    thread = thread->caller;
                    continue;
                }
            }

            thread->stackTop = frame->slots;
            push(result);
            frame = &thread->frames[thread->frameCount - 1];
            break;
            //< Calls and Functions interpret-return
        }
            //> Classes and Instances interpret-class
        case OP_CLASS:
            push(OBJ_VAL(newClass(READ_STRING()->chars)));
            break;
            //< Classes and Instances interpret-class
        case OP_LIST: {
            ObjList* list = newList();
            std::stack<Value> stack;
            for (int argc = READ_BYTE(); argc > 0; argc--) {
                stack.push(pop());
            }
            while (!stack.empty()) {
                list->container.push_back(stack.top());
                stack.pop();
            }
            push(OBJ_VAL(list));
            break;
        }
        case OP_MAP:
            push(OBJ_VAL(newMap()));
            break;
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
    return INTERPRET_OK;
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

bool VM::loadLibrary(std::string path, std::string name)
{
    std::map<std::string, dl*>::iterator it = m_dl.find(name);
    if (it != m_dl.end()) {
        return false;
    }
    else {
        dl* l = new dl(path, name);
        {
            std::vector<std::string> names;
            std::vector<NativeFn> fns;
            l->functions(names, fns);
            if (names.size() != fns.size()) {
                delete l;
                return false;
            }
            for (size_t i = 0; i < names.size(); i++) {
                defineNative(names[i].c_str(), fns[i]);
            }
        }
        {
            std::vector<std::string> names;
            std::vector<NativeClass*> klasses;
            l->symbols(names, klasses);
            if (names.size() != klasses.size()) {
                delete l;
                return false;
            }
            for (size_t i = 0; i < names.size(); i++) {
                defineSymbol(names[i].c_str(), klasses[i]);
            }
        }
        {
            std::vector<std::string> names;
            std::vector<double> consts;
            l->constants(names, consts);
            if (names.size() != consts.size()) {
                delete l;
                return false;
            }
            for (size_t i = 0; i < names.size(); i++) {
                defineNumber(names[i].c_str(), consts[i]);
            }
        }
        m_dl[name] = l;
    }
    return true;
}

bool VM::callFunction(Value value, int argc, Value* argv, bool startNew)
{
    ObjThread* thd = newThread();
    if (startNew) {
        threadStack.push_back(thread);
    }
    else {
        thd->caller = thread;
    }
    thread = thd;
    for (int i = 0; i < argc; i++)
        push(argv[i]);
    bool ret = callValue(value, argc);
    run();
    for (int i = 0; i < argc; i++)
        pop();
    return ret;
}
//< interpret
