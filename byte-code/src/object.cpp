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

template <typename T>
static inline T rectify_index(T index, T imax)
{
    if (index >= 0) {
        if (index >= imax) {
            throw std::runtime_error("Index is invalid.");
        }
    }
    else {
        if (-index > imax) {
            throw std::runtime_error("Index is invalid.");
        }
        index = imax + index;
    }
    return index;
}

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

Value Obj::add(Value v, ObjectFactory* factory)
{
    (void)v;
    (void)factory;
    throw std::runtime_error("unsupported operation +");
}

Value Obj::sub(Value v, ObjectFactory* factory)
{
    (void)v;
    (void)factory;
    throw std::runtime_error("unsupported operation -");
}

Value Obj::mul(Value v, ObjectFactory* factory)
{
    (void)v;
    (void)factory;
    throw std::runtime_error("unsupported operation *");
}

Value Obj::div(Value v, ObjectFactory* factory)
{
    (void)v;
    (void)factory;
    throw std::runtime_error("unsupported operation /");
}

Value Obj::mod(Value v, ObjectFactory* factory)
{
    (void)v;
    (void)factory;
    throw std::runtime_error("unsupported operation %");
}

Value Obj::pow(Value v, ObjectFactory* factory)
{
    (void)v;
    (void)factory;
    throw std::runtime_error("unsupported operation ^");
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

Value ObjString::add(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_STRING(v)) {
        ObjString* b = AS_STRING(v);
        std::string rchars = chars;
        rchars += b->chars;
        ObjString* result = factory->newString(rchars.c_str());
        r = OBJ_VAL(result);
    }
    else if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        std::stringstream ss;
        ss << chars << b;
        ObjString* c = factory->newString(ss.str().c_str());
        r = OBJ_VAL(c);
    }
    else if (IS_COMPLEX(v)) {
        ObjComplex* b = AS_COMPLEX(v);
        std::string s = chars + b->stringify();
        ObjString* c = factory->newString(s.c_str());
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operator +");
    }
    return r;
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

Value ObjComplex::add(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value.real() + b, value.imag()));
        r = OBJ_VAL(c);
    }
    else if (IS_COMPLEX(v)) {
        ObjComplex* b = AS_COMPLEX(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value + b->value));
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation +");
    }
    return r;
}

Value ObjComplex::sub(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value.real() - b, value.imag()));
        r = OBJ_VAL(c);
    }
    else if (IS_COMPLEX(v)) {
        ObjComplex* b = AS_COMPLEX(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value - b->value));
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation -");
    }
    return r;
}

Value ObjComplex::mul(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value.real() * b, value.imag() * b));
        r = OBJ_VAL(c);
    }
    else if (IS_COMPLEX(v)) {
        ObjComplex* b = AS_COMPLEX(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value * b->value));
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation *");
    }
    return r;
}

Value ObjComplex::div(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        std::complex<double> t(b, 0);
        ObjComplex* c = factory->newComplex(std::complex<double>(value / t));
        r = OBJ_VAL(c);
    }
    else if (IS_COMPLEX(v)) {
        ObjComplex* b = AS_COMPLEX(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value / b->value));
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation /");
    }
    return r;
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
            ss << ",";
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
            if (IS_STRING(v))
                ss << "\"";
            ss << AS_OBJ(v)->stringify();
            if (IS_STRING(v))
                ss << "\"";
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
        if (it != container.begin())
            ss << ",";
        ss << "\"" << it->first << "\":";
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
            bool isString = IS_STRING(v);
            if (isString)
                ss << "\"";
            ss << AS_OBJ(v)->stringify();
            if (isString)
                ss << "\"";
        }
    }
    ss << "}";
    return ss.str();
}

void ObjMap::blaken(void)
{
}

Value ObjMap::get(std::string index)
{
    std::map<std::string, Value>::iterator it = container.find(index);
    if (it == container.end()) {
        throw std::runtime_error("Index is invalid.");
    }
    return it->second;
}

void ObjMap::set(std::string index, Value value)
{
    container[index] = value;
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
    index = rectify_index(index, (int)value.n_elem);
    LAX_LOG("vec[%d] is peeked.", index);
    return NUMBER_VAL(value[index]);
}

void ObjCol::set(int index, Value v)
{
    index = rectify_index(index, (int)value.n_elem);
    LAX_LOG("vec[%d] is set.", index);
    value[index] = AS_NUMBER(v);
}

Value ObjCol::add(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_COL(v)) {
        ObjCol* b = AS_COL(v);
        ObjCol* c = factory->newCol();
        c->value = value + b->value;
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation +");
    }
    return r;
}

Value ObjCol::sub(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_COL(v)) {
        ObjCol* b = AS_COL(v);
        ObjCol* c = factory->newCol();
        c->value = value - b->value;
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation -");
    }
    return r;
}

Value ObjCol::mul(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        ObjCol* c = factory->newCol();
        c->value = value * b;
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation *");
    }
    return r;
}

Value ObjCol::div(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        ObjCol* c = factory->newCol();
        c->value = value / b;
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation /");
    }
    return r;
}

Value ObjCol::mod(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_COL(v)) {
        ObjCol* b = AS_COL(v);
        ObjCol* c = factory->newCol();
        c->value = value % b->value;
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation /");
    }
    return r;
}

Value ObjCol::pow(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        ObjCol* c = factory->newCol();
        c->value = arma::pow(value, b);
        r = OBJ_VAL(c);
    }
    else if (IS_COL(v)) {
        ObjCol* b = AS_COL(v);
        ObjCol* c = factory->newCol();
        c->value = value % b->value;
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation /");
    }
    return r;
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
    index = rectify_index(index, (int)value.n_elem);
    LAX_LOG("vec[%d] is peeked.", index);
    return NUMBER_VAL(value[index]);
}

void ObjRow::set(int index, Value v)
{
    index = rectify_index(index, (int)value.n_elem);
    value[index] = AS_NUMBER(v);
}

Value ObjRow::add(Value v, ObjectFactory* factory)
{
    Value ret = NIL_VAL;
    if (IS_ROW(v)) {
        ObjRow* b = AS_ROW(v);
        ObjRow* r = factory->newRow();
        r->value = value + b->value;
        ret = OBJ_VAL(r);
    }
    else if (IS_MAT(v)) {
        ObjMat* b = AS_MAT(v);
        ObjRow* r = factory->newRow();
        r->value = value + b->value;
        ret = OBJ_VAL(r);
    }
    else {
        throw std::runtime_error("unsupported operator +");
    }
    return ret;
}

Value ObjRow::sub(Value v, ObjectFactory* factory)
{
    Value ret = NIL_VAL;
    if (IS_ROW(v)) {
        ObjRow* b = AS_ROW(v);
        ObjRow* r = factory->newRow();
        r->value = value - b->value;
        ret = OBJ_VAL(r);
    }
    else if (IS_MAT(v)) {
        ObjMat* b = AS_MAT(v);
        ObjRow* r = factory->newRow();
        r->value = value - b->value;
        ret = OBJ_VAL(r);
    }
    else {
        throw std::runtime_error("unsupported operator -");
    }
    return ret;
}

Value ObjRow::mul(Value v, ObjectFactory* factory)
{
    Value ret = NIL_VAL;
    if (IS_ROW(v)) {
        ObjRow* b = AS_ROW(v);
        ObjRow* r = factory->newRow();
        r->value = value * b->value;
        ret = OBJ_VAL(r);
    }
    else if (IS_MAT(v)) {
        ObjMat* b = AS_MAT(v);
        ObjRow* r = factory->newRow();
        r->value = value * b->value;
        ret = OBJ_VAL(r);
    }
    else {
        throw std::runtime_error("unsupported operator *");
    }
    return ret;
}

Value ObjRow::div(Value v, ObjectFactory* factory)
{
    Value ret = NIL_VAL;
    if (IS_ROW(v)) {
        ObjRow* b = AS_ROW(v);
        ObjRow* r = factory->newRow();
        r->value = value / b->value;
        ret = OBJ_VAL(r);
    }
    else if (IS_MAT(v)) {
        ObjMat* b = AS_MAT(v);
        ObjRow* r = factory->newRow();
        r->value = value / b->value;
        ret = OBJ_VAL(r);
    }
    else {
        throw std::runtime_error("unsupported operator /");
    }
    return ret;
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

Value ObjMat::get(int row, int col)
{
    row = rectify_index(row, (int)value.n_rows);
    col = rectify_index(col, (int)value.n_cols);
    return NUMBER_VAL(value(row, col));
}

void ObjMat::set(int row, int col, Value v)
{
    row = rectify_index(row, (int)value.n_rows);
    col = rectify_index(col, (int)value.n_cols);
    value(row, col) = AS_NUMBER(v);
}

Value ObjMat::add(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_MAT(v)) {
        ObjMat* b = AS_MAT(v);
        ObjMat* m = factory->newMat();
        m->value = value + b->value;
        r = OBJ_VAL(m);
    }
    else {
        throw std::runtime_error("unsupported operation +");
    }
    return r;
}

Value ObjMat::sub(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_MAT(v)) {
        ObjMat* b = AS_MAT(v);
        ObjMat* m = factory->newMat();
        m->value = value - b->value;
        r = OBJ_VAL(m);
    }
    else {
        throw std::runtime_error("unsupported operation -");
    }
    return r;
}

Value ObjMat::mul(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_MAT(v)) {
        ObjMat* b = AS_MAT(v);
        ObjMat* m = factory->newMat();
        m->value = value * b->value;
        r = OBJ_VAL(m);
    }
    else {
        throw std::runtime_error("unsupported operation *");
    }
    return r;
}

ObjCube::ObjCube() : Obj(OBJ_CUBE)
{
}

ObjCube::~ObjCube()
{
}

std::string ObjCube::stringify(void)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void ObjCube::blaken(void)
{
}

Value ObjCube::get(int row, int col, int depth)
{
    row = rectify_index(row, (int)value.n_rows);
    col = rectify_index(col, (int)value.n_cols);
    depth = rectify_index(depth, (int)value.n_slices);
    return NUMBER_VAL(value(row, col, depth));
}

void ObjCube::set(int row, int col, int depth, Value v)
{
    row = rectify_index(row, (int)value.n_rows);
    col = rectify_index(col, (int)value.n_cols);
    depth = rectify_index(depth, (int)value.n_slices);
    value(row, col, depth) = AS_NUMBER(v);
}

NativeClass::NativeClass(std::map<std::string, NativeClassBoundFn>& apis,
                         std::map<std::string, NativeClassBoundProperty>& constants)
    : m_apis(apis), m_constants(constants)
{
}

NativeClass::~NativeClass()
{
}

Value NativeClass::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    std::map<std::string, NativeClassBoundFn>::iterator it = m_apis.find(name);
    if (it == m_apis.end())
        throw std::runtime_error("invalid method");
    return it->second(factory, this, argc, argv);
}

Value NativeClass::call(ObjectFactory* factory, int argc, Value* argv)
{
    (void)factory;
    (void)argc;
    (void)argv;
    throw std::runtime_error("invalid call");
    return NIL_VAL;
}

Value NativeClass::constant(ObjectFactory* factory, std::string name)
{
    std::map<std::string, NativeClassBoundProperty>::iterator it = m_constants.find(name);
    if (it == m_constants.end())
        throw std::runtime_error("invalid property");
    return it->second(factory, this);
}

ObjNativeClass::ObjNativeClass() : Obj(OBJ_NATIVE_CLASS), klass(NULL)
{
}

ObjNativeClass::~ObjNativeClass()
{
    if (klass)
        delete klass;
}

std::string ObjNativeClass::stringify(void)
{
    return "<native obj>";
}

void ObjNativeClass::blaken(void)
{
}

NativeObject::NativeObject(std::map<std::string, NativeObjectBoundFn>& apis,
                           std::map<std::string, NativeObjectBoundProperty>& properties)
    : m_apis(apis), m_properties(properties)
{
}

NativeObject::~NativeObject()
{
}

Value NativeObject::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    std::map<std::string, NativeObjectBoundFn>::iterator it = m_apis.find(name);
    if (it == m_apis.end())
        throw std::runtime_error("invalid method");
    return it->second(factory, this, argc, argv);
}

Value NativeObject::property(ObjectFactory* factory, std::string name)
{
    std::map<std::string, NativeObjectBoundProperty>::iterator it = m_properties.find(name);
    if (it == m_properties.end())
        throw std::runtime_error("invalid property");
    return it->second(factory, this);
}

ObjNativeObject::ObjNativeObject() : Obj(OBJ_NATIVE_OBJ), object(NULL)
{
}

ObjNativeObject::~ObjNativeObject()
{
    if (object)
        delete object;
}

std::string ObjNativeObject::stringify(void)
{
    return "<native obj>";
}

void ObjNativeObject::blaken(void)
{
}

ObjThread::ObjThread() : Obj(OBJ_THREAD), frameCount(0), stackTop(stack), caller(NULL)
{
}

ObjThread::ObjThread(ObjClosure* closure) : Obj(OBJ_THREAD), frameCount(0), stackTop(stack), caller(NULL)
{
    CallFrame* frame = &frames[frameCount++];
    frame->closure = closure;
    frame->ip = closure->function->chunk.code;
    frames->slots = stackTop - 1;
}

ObjThread::~ObjThread()
{
}

std::string ObjThread::stringify(void)
{
    return "<thread>";
}

void ObjThread::blaken(void)
{
}

static Value list_class_filled(ObjectFactory* factory, NativeClass* klass, int argc, Value* argv)
{
    (void)klass;
    ObjList* list = factory->newList();
    if (2 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_NUMBER(argv[0]))
        throw std::runtime_error("Size must be a number.");
    double sz = AS_NUMBER(argv[0]);
    if (sz < 0)
        throw std::runtime_error("Size cannot be negative.");
    if (sz - (int)sz)
        throw std::runtime_error("Size must be an integer.");
    for (int idx = 0; idx < sz; idx++)
        list->container.push_back(argv[1]);
    return OBJ_VAL(list);
}

// clang-format off
std::map<std::string, NativeClassBoundFn> s_list_class_apis = {
    {"filled", list_class_filled},
};

std::map<std::string, NativeClassBoundProperty> s_list_class_constants = {
};
// clang-format on

listNative::listNative() : NativeClass(s_list_class_apis, s_list_class_constants)
{
}

Value listNative::call(ObjectFactory* factory, int argc, Value* args)
{
    ObjList* list = factory->newList();
    for (int idx = 0; idx < argc; idx++)
        list->container.push_back(args[idx]);
    return OBJ_VAL(list);
}

static Value vec_class_convolute(ObjectFactory* factory, NativeClass* klass, int argc, Value* argv)
{
    (void)klass;
    if (2 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_COL(argv[0]))
        throw std::runtime_error("vec is expected.");
    if (!IS_COL(argv[1]))
        throw std::runtime_error("vec is expected.");
    ObjCol* col = factory->newCol();
    col->value = arma::conv(AS_COL(argv[0])->value, AS_COL(argv[1])->value);
    return OBJ_VAL(col);
}

static Value vec_class_correlate(ObjectFactory* factory, NativeClass* klass, int argc, Value* argv)
{
    (void)klass;
    ObjCol* col = factory->newCol();
    arma::vec x;
    if (2 == argc) {
        if (!IS_COL(argv[0]))
            throw std::runtime_error("vec is expected.");
        if (!IS_COL(argv[1]))
            throw std::runtime_error("vec is expected.");
        x = AS_COL(argv[1])->value;
    }
    else if (1 == argc) {
        if (!IS_COL(argv[0]))
            throw std::runtime_error("vec is expected.");
        x = AS_COL(argv[0])->value;
    }
    else {
        throw std::runtime_error("invalid number of arguments.");
    }
    col->value.resize(x.size());
    for (size_t idx = 0; idx < x.size(); idx++) {
        arma::vec s = arma::shift(x, idx);
        arma::vec y = arma::cor(AS_COL(argv[0])->value, s);
        col->value[idx] = y[0];
    }
    return OBJ_VAL(col);
}

static Value vec_class_prop_default(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_DEFAULT);
}

static Value vec_class_prop_zeros(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_ZEROS);
}

static Value vec_class_prop_ones(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_ONES);
}

static Value vec_class_prop_eye(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_EYE);
}

static Value vec_class_prop_randu(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_RANDU);
}

static Value vec_class_prop_randn(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_RANDN);
}

// clang-format off
std::map<std::string, NativeClassBoundFn> s_vec_class_apis = {
    {"conv", vec_class_convolute},
    {"corr", vec_class_correlate}
};

std::map<std::string, NativeClassBoundProperty> s_vec_class_constants = {
    {"default", vec_class_prop_default},
    {"zeros", vec_class_prop_zeros},
    {"ones", vec_class_prop_ones},
    {"eye", vec_class_prop_eye},
    {"randu", vec_class_prop_randu},
    {"randn", vec_class_prop_randn}
};
// clang-format on

vecNative::vecNative() : NativeClass(s_vec_class_apis, s_vec_class_constants)
{
}

Value vecNative::call(ObjectFactory* factory, int argc, Value* args)
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
    case 0:
        break;
    default:
        throw std::runtime_error("invalid arguments");
        break;
    }
    return OBJ_VAL(factory->newCol(sz, fill_type));
}

// clang-format off
std::map<std::string, NativeClassBoundFn> s_mat_class_apis;

std::map<std::string, NativeClassBoundProperty> s_mat_class_constants = {
    {"default", vec_class_prop_default},
    {"zeros", vec_class_prop_zeros},
    {"ones", vec_class_prop_ones},
    {"eye", vec_class_prop_eye},
    {"randu", vec_class_prop_randu},
    {"randn", vec_class_prop_randn}
};
// clang-format on

matNative::matNative() : NativeClass(s_mat_class_apis, s_mat_class_constants)
{
}

Value matNative::call(ObjectFactory* factory, int argc, Value* args)
{
    ObjFillType fill_type = OBJ_FILL_DEFAULT;
    size_t rows = 0, cols = 0;
    switch (argc) {
    case 3:
        if (!IS_NUMBER(args[2])) {
            throw std::runtime_error("number is expected");
        }
        fill_type = (ObjFillType)AS_NUMBER(args[2]);
        if (!IS_NUMBER(args[0])) {
            throw std::runtime_error("number is expected");
        }
        if (!IS_NUMBER(args[1])) {
            throw std::runtime_error("number is expected");
        }
        rows = AS_NUMBER(args[0]);
        cols = AS_NUMBER(args[1]);
        break;
    case 2:
        if (!IS_NUMBER(args[0]))
            throw std::runtime_error("number is expected");
        if (!IS_NUMBER(args[1]))
            throw std::runtime_error("number is expected");
        rows = AS_NUMBER(args[0]);
        cols = AS_NUMBER(args[1]);
        break;
    case 0:
        break;
    default:
        throw std::runtime_error("invalid arguments");
        break;
    }
    return OBJ_VAL(factory->newMat(rows, cols, fill_type));
}
