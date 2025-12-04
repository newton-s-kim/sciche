//> Strings object-c
#include <stdio.h>
#include <string.h>

#include "object.hpp"
//> Hash Tables object-include-table
#include "table.hpp"
//< Hash Tables object-include-table
#include "log.hpp"
#include "value.hpp"

#include <sstream>

Obj::Obj(ObjType objType) : type(objType), isMarked(false)
{
}

Obj::~Obj()
{
}

void Obj::print(void)
{
    printf("%s", stringify().c_str());
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

std::string ObjFunction::stringify(void)
{
    std::string ret;
    //> print-script
    if (name.empty()) {
        ret = "<script>";
        return ret;
    }
    //< print-script
    return "<fn " + name + ">";
}

ObjNative::ObjNative(NativeFn pFunction) : Obj(OBJ_NATIVE), function(pFunction)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjNative), type);
#endif
}

std::string ObjNative::stringify(void)
{
    return "<native fn>";
}

ObjString::ObjString(std::string pChars) : Obj(OBJ_STRING), chars(pChars)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjString), type);
#endif
}

std::string ObjString::stringify(void)
{
    return chars;
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

std::string ObjUpvalue::stringify(void)
{
    return "upvalue";
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

std::string ObjClosure::stringify(void)
{
    return function->stringify();
}

ObjClass::~ObjClass()
{
}

std::string ObjClass::stringify(void)
{
    return name;
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

std::string ObjInstance::stringify(void)
{
    return klass->stringify() + " instance";
}

ObjBoundMethod::ObjBoundMethod(Value pReceiver, ObjClosure* pMethod)
    : Obj(OBJ_BOUND_METHOD), receiver(pReceiver), method(pMethod)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjBoundMethod), type);
#endif
}

std::string ObjBoundMethod::stringify(void)
{
    return method->function->stringify();
}

ObjComplex::ObjComplex(const std::complex<double> v) : Obj(OBJ_COMPLEX), value(v)
{
}
ObjComplex::~ObjComplex()
{
}

std::string ObjComplex::stringify(void)
{
    std::stringstream ss;
    double rv = value.real();
    double iv = value.imag();
    if (0 != rv) {
        ss << rv;
        if (iv >= 0) {
            ss << "+" << iv;
        }
        else {
            ss << iv;
        }
    }
    else {
        ss << iv;
    }
    ss << "j";
    return ss.str();
}
