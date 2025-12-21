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

void ObjFunction::blaken(void)
{
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

void ObjNative::blaken(void)
{
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

void ObjString::blaken(void)
{
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

void ObjUpvalue::blaken(void)
{
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

void ObjClosure::blaken(void)
{
}

ObjClass::~ObjClass()
{
}

std::string ObjClass::stringify(void)
{
    return name;
}

void ObjClass::blaken(void)
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

std::string ObjInstance::stringify(void)
{
    return klass->stringify() + " instance";
}

void ObjInstance::blaken(void)
{
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

void ObjBoundMethod::blaken(void)
{
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

void ObjComplex::blaken(void)
{
}

ObjList::ObjList() : Obj(OBJ_LIST)
{
}

ObjList::~ObjList()
{
}

std::string ObjList::stringify(void)
{
    std::stringstream ss;
    ss << "[";
    for (std::vector<Value>::iterator it = container.begin(); it != container.end(); it++) {
        if (it != container.begin())
            ss << ", ";
        Value v = *it;
        if (IS_NIL(v)) {
            ss << "nil";
        }
        else if (IS_BOOL(v)) {
            ss << (AS_BOOL(v) ? "true" : "false");
        }
        else if (IS_NUMBER(v)) {
            ss << AS_NUMBER(v);
        }
        else if (IS_OBJ(v)) {
            ss << AS_OBJ(v)->stringify();
        }
    }
    ss << "]";
    return ss.str();
}

void ObjList::blaken(void)
{
}

Value ObjList::get(int index)
{
    if (index >= 0) {
        if (index >= (int)container.size()) {
            throw std::runtime_error("Index is invalid.");
        }
    }
    else {
        if (-index > (int)container.size()) {
            throw std::runtime_error("Index is invalid.");
        }
        index = (int)container.size() + index;
    }
    LAX_LOG("list[%d] is peeked.", index);
    return container[index];
}

void ObjList::set(int index, Value v)
{
    if (index >= 0) {
        if (index >= (int)container.size()) {
            throw std::runtime_error("Index is invalid.");
        }
    }
    else {
        if (-index > (int)container.size()) {
            throw std::runtime_error("Index is invalid.");
        }
        index = container.size() + index;
    }
    container[index] = v;
}

ObjMap::ObjMap() : Obj(OBJ_MAP)
{
}

ObjMap::~ObjMap()
{
}

std::string ObjMap::stringify(void)
{
    std::stringstream ss;
    ss << "{";
    for (std::map<std::string, Value>::iterator it = container.begin(); it != container.end(); it++) {
        ss << it->first << ",";
        Value v = it->second;
        if (IS_NIL(v)) {
            ss << "nil";
        }
        else if (IS_BOOL(v)) {
            ss << (AS_BOOL(v) ? "true" : "false");
        }
        else if (IS_NUMBER(v)) {
            ss << AS_NUMBER(v);
        }
        else if (IS_OBJ(v)) {
            ss << AS_OBJ(v)->stringify();
        }
    }
    ss << "}";
    return ss.str();
}

void ObjMap::blaken(void)
{
}

ObjCol::ObjCol() : Obj(OBJ_COL)
{
}

ObjCol::~ObjCol()
{
}

std::string ObjCol::stringify(void)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void ObjCol::blaken(void)
{
}

Value ObjCol::get(int index)
{
    LAX_LOG("size is %lld", value.n_elem);
    if (index >= 0) {
        if (index >= (int)value.n_elem) {
            throw std::runtime_error("Index is invalid.");
        }
    }
    else {
        if (-index > (int)value.n_elem) {
            throw std::runtime_error("Index is invalid.");
        }
        index = value.size() + index;
    }
    LAX_LOG("vec[%d] is peeked.", index);
    return value[index];
}

void ObjCol::set(int index, Value v)
{
    if (index >= 0) {
        if (index >= (int)value.n_elem) {
            throw std::runtime_error("Index is invalid.");
        }
    }
    else {
        if (-index > (int)value.n_elem) {
            throw std::runtime_error("Index is invalid.");
        }
        index = value.size() + index;
    }
    LAX_LOG("vec[%d] is set.", index);
    value[index] = AS_NUMBER(v);
}

ObjRow::ObjRow() : Obj(OBJ_ROW)
{
}

ObjRow::~ObjRow()
{
}

std::string ObjRow::stringify(void)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void ObjRow::blaken(void)
{
}

Value ObjRow::get(int index)
{
    if (index >= 0) {
        if (index >= (int)value.n_elem) {
            throw std::runtime_error("Index is invalid.");
        }
    }
    else {
        if (-index > (int)value.n_elem) {
            throw std::runtime_error("Index is invalid.");
        }
        index = value.size() + index;
    }
    LAX_LOG("vec[%d] is peeked.", index);
    return value[index];
}

void ObjRow::set(int index, Value v)
{
    if (index >= 0) {
        if (index >= (int)value.n_elem) {
            throw std::runtime_error("Index is invalid.");
        }
    }
    else {
        if (-index > (int)value.n_elem) {
            throw std::runtime_error("Index is invalid.");
        }
        index = value.size() + index;
    }
    value[index] = AS_NUMBER(v);
}

ObjMat::ObjMat() : Obj(OBJ_MAT)
{
}

ObjMat::~ObjMat()
{
}

std::string ObjMat::stringify(void)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void ObjMat::blaken(void)
{
}

ObjNativeObject::ObjNativeObject() : Obj(OBJ_NATIVE_OBJ)
{
}

ObjNativeObject::~ObjNativeObject()
{
    if (klass)
        delete klass;
}

std::string ObjNativeObject::stringify(void)
{
    return "<native obj>";
}

void ObjNativeObject::blaken(void)
{
}
