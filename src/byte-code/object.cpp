//> Strings object-c
#include <stdio.h>
#include <string.h>

#include "object.hpp"
//> Hash Tables object-include-table
#include "table.hpp"
//< Hash Tables object-include-table
#include "log.hpp"
#include "value.hpp"

Obj::Obj(ObjType objType) : type(objType), isMarked(false)
{
}

ObjFunction::ObjFunction() : Obj(OBJ_FUNCTION)
{
    arity = 0;
    //> Closures init-upvalue-count
    upvalueCount = 0;
    //< Closures init-upvalue-count
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjFunction), type);
#endif
}

ObjFunction::~ObjFunction()
{
}

ObjNative::ObjNative(NativeFn pFunction) : Obj(OBJ_NATIVE), function(pFunction)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjNative), type);
#endif
}

ObjString::ObjString(std::string pChars) : Obj(OBJ_STRING), chars(pChars)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjString), type);
#endif
}

ObjUpvalue::ObjUpvalue(Value* slot) : Obj(OBJ_UPVALUE)
{
    //> init-closed
    closed = NIL_VAL;
    //< init-closed
    location = slot;
    //> init-next
    next = NULL;
    //< init-next
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjUpvalue), type);
#endif
}

ObjClosure::ObjClosure(ObjFunction* pFunction) : Obj(OBJ_CLOSURE), function(pFunction)
{
    upvalues.reserve(function->upvalueCount);
    upvalueCount = function->upvalueCount;
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjClosure), type);
#endif
}

ObjClass::ObjClass(std::string pName) : Obj(OBJ_CLASS), name(pName)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjClass), type);
#endif
}

ObjClass::~ObjClass()
{
}

ObjInstance::ObjInstance(ObjClass* pKlass) : Obj(OBJ_INSTANCE), klass(pKlass)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjInstance), type);
#endif
}

ObjInstance::~ObjInstance()
{
}

ObjBoundMethod::ObjBoundMethod(Value pReceiver, ObjClosure* pMethod)
    : Obj(OBJ_BOUND_METHOD), receiver(pReceiver), method(pMethod)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjBoundMethod), type);
#endif
}

//> Calls and Functions print-function-helper
static void printFunction(ObjFunction* function)
{
    //> print-script
    if (function->name.empty()) {
        printf("<script>");
        return;
    }
    //< print-script
    printf("<fn %s>", function->name.c_str());
}
//< Calls and Functions print-function-helper
//> print-object
void ObjectUtil::printObject(Value value)
{
    switch (OBJ_TYPE(value)) {
        //> Methods and Initializers print-bound-method
    case OBJ_BOUND_METHOD:
        printFunction(AS_BOUND_METHOD(value)->method->function);
        break;
        //< Methods and Initializers print-bound-method
        //> Classes and Instances print-class
    case OBJ_CLASS:
        printf("%s", AS_CLASS(value)->name.c_str());
        break;
        //< Classes and Instances print-class
        //> Closures print-closure
    case OBJ_CLOSURE:
        printFunction(AS_CLOSURE(value)->function);
        break;
        //< Closures print-closure
        //> Calls and Functions print-function
    case OBJ_FUNCTION:
        printFunction(AS_FUNCTION(value));
        break;
        //< Calls and Functions print-function
        //> Classes and Instances print-instance
    case OBJ_INSTANCE:
        printf("%s instance", AS_INSTANCE(value)->klass->name.c_str());
        break;
        //< Classes and Instances print-instance
        //> Calls and Functions print-native
    case OBJ_NATIVE:
        printf("<native fn>");
        break;
        //< Calls and Functions print-native
    case OBJ_STRING:
        printf("%s", AS_CSTRING(value).c_str());
        break;
        //> Closures print-upvalue
    case OBJ_UPVALUE:
        printf("upvalue");
        break;
        //< Closures print-upvalue
    }
}
//< print-object
