#include "object.hpp"

static Value asinNative(ObjectFactory* factory, int argc, Value* args)
{
    Value value = 0;
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    if (IS_NUMBER(args[0])) {
        value = NUMBER_VAL(asin(AS_NUMBER(args[0])));
    }
    else if (IS_COL(args[0])) {
        ObjCol* c = factory->newCol();
        c->value = asin(AS_COL(args[0])->value);
        value = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("number is expected");
    }
    return value;
}

static Value atanNative(ObjectFactory* factory, int argc, Value* args)
{
    Value value = 0;
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    if (IS_NUMBER(args[0])) {
        value = NUMBER_VAL(atan(AS_NUMBER(args[0])));
    }
    else if (IS_COL(args[0])) {
        ObjCol* c = factory->newCol();
        c->value = atan(AS_COL(args[0])->value);
        value = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("number is expected");
    }
    return value;
}

static Value acosNative(ObjectFactory* factory, int argc, Value* args)
{
    Value value = 0;
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    if (IS_NUMBER(args[0])) {
        value = NUMBER_VAL(acos(AS_NUMBER(args[0])));
    }
    else if (IS_COL(args[0])) {
        ObjCol* c = factory->newCol();
        c->value = acos(AS_COL(args[0])->value);
        value = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("number is expected");
    }
    return value;
}

static Value sinNative(ObjectFactory* factory, int argc, Value* args)
{
    Value value = 0;
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    if (IS_NUMBER(args[0])) {
        value = NUMBER_VAL(sin(AS_NUMBER(args[0])));
    }
    else if (IS_COL(args[0])) {
        ObjCol* c = factory->newCol();
        c->value = sin(AS_COL(args[0])->value);
        value = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("number is expected");
    }
    return value;
}

static Value cosNative(ObjectFactory* factory, int argc, Value* args)
{
    Value value = 0;
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    if (IS_NUMBER(args[0])) {
        return NUMBER_VAL(cos(AS_NUMBER(args[0])));
    }
    else if (IS_COL(args[0])) {
        ObjCol* c = factory->newCol();
        c->value = cos(AS_COL(args[0])->value);
        value = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("number is expected");
    }
    return value;
}

static Value tanNative(ObjectFactory* factory, int argc, Value* args)
{
    Value value = 0;
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    if (IS_NUMBER(args[0])) {
        return NUMBER_VAL(tan(AS_NUMBER(args[0])));
    }
    else if (IS_COL(args[0])) {
        ObjCol* c = factory->newCol();
        c->value = tan(AS_COL(args[0])->value);
        value = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("number is expected");
    }
    return value;
}

static Value sqrtNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    Value value = 0;
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    if (IS_NUMBER(args[0])) {
        double v = AS_NUMBER(args[0]);
        if (0 <= v) {
            value = NUMBER_VAL(sqrt(AS_NUMBER(args[0])));
        }
        else {
            ObjComplex* cmplx = factory->newComplex(std::complex<double>(0, sqrt(-AS_NUMBER(args[0]))));
            value = OBJ_VAL(cmplx);
        }
    }
    else {
        throw std::runtime_error("number is expected");
    }
    return value;
}

static Value cbrtNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    Value value = 0;
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    if (IS_NUMBER(args[0])) {
        value = NUMBER_VAL(cbrt(AS_NUMBER(args[0])));
    }
    else {
        std::runtime_error("number is expected");
    }
    return value;
}

static Value absNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    if (IS_NUMBER(args[0])) {
        return NUMBER_VAL(fabs(AS_NUMBER(args[0])));
    }
    else if (IS_COMPLEX(args[0])) {
        return NUMBER_VAL(abs(AS_COMPLEX(args[0])->value));
    }
    else if (IS_MAT(args[0])) {
        ObjMat* m = factory->newMat();
        m->value = abs(AS_MAT(args[0])->value);
        return OBJ_VAL(m);
    }
    else {
        throw std::runtime_error("number is expected");
    }
    return NUMBER_VAL(0);
}

static Value phaseNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    if (IS_COMPLEX(args[0])) {
        return NUMBER_VAL(arg(AS_COMPLEX(args[0])->value));
    }
    else {
        throw std::runtime_error("complex number is expected");
    }
    return NUMBER_VAL(0);
}

static Value logNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    Value r = 0;
    if (IS_NUMBER(args[0])) {
        r = NUMBER_VAL(log(AS_NUMBER(args[0])));
    }
    else if (IS_COL(args[0])) {
        ObjCol* c = AS_COL(args[0]);
        ObjCol* ret = factory->newCol();
        ret->value = log(c->value);
        r = OBJ_VAL(ret);
    }
    else if (IS_MAT(args[0])) {
        ObjMat* m = AS_MAT(args[0]);
        ObjMat* ret = factory->newMat();
        ret->value = log(m->value);
        r = OBJ_VAL(ret);
    }
    else {
        throw std::runtime_error("number is expected.");
    }
    return r;
}

static Value log2Native(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    Value r = 0;
    if (IS_NUMBER(args[0])) {
        r = NUMBER_VAL(log2(AS_NUMBER(args[0])));
    }
    else if (IS_COL(args[0])) {
        ObjCol* c = AS_COL(args[0]);
        ObjCol* ret = factory->newCol();
        ret->value = log2(c->value);
        r = OBJ_VAL(ret);
    }
    else if (IS_MAT(args[0])) {
        ObjMat* m = AS_MAT(args[0]);
        ObjMat* ret = factory->newMat();
        ret->value = log2(m->value);
        r = OBJ_VAL(ret);
    }
    else {
        throw std::runtime_error("number is expected.");
    }
    return r;
}

static Value log10Native(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    Value r = 0;
    if (IS_NUMBER(args[0])) {
        r = NUMBER_VAL(log10(AS_NUMBER(args[0])));
    }
    else if (IS_COL(args[0])) {
        ObjCol* c = AS_COL(args[0]);
        ObjCol* ret = factory->newCol();
        ret->value = log10(c->value);
        r = OBJ_VAL(ret);
    }
    else if (IS_MAT(args[0])) {
        ObjMat* m = AS_MAT(args[0]);
        ObjMat* ret = factory->newMat();
        ret->value = log10(m->value);
        r = OBJ_VAL(ret);
    }
    else {
        throw std::runtime_error("number is expected.");
    }
    return r;
}

static Value expNative(ObjectFactory* factory, int argc, Value* args)
{
    if (1 != argc)
        throw std::runtime_error("invalid arguments");
    Value r = 0;
    if (IS_NUMBER(args[0])) {
        double v = AS_NUMBER(args[0]);
        r = NUMBER_VAL(exp(v));
    }
    else if (IS_COMPLEX(args[0])) {
        ObjComplex* v = AS_COMPLEX(args[0]);
        std::complex<double> cv = exp(v->value);
        ObjComplex* rv = factory->newComplex(cv);
        r = OBJ_VAL(rv);
    }
    else if (IS_COL(args[0])) {
        ObjCol* c = AS_COL(args[0]);
        ObjCol* ret = factory->newCol();
        ret->value = exp(c->value);
        r = OBJ_VAL(ret);
    }
    else if (IS_MAT(args[0])) {
        ObjMat* m = AS_MAT(args[0]);
        ObjMat* ret = factory->newMat();
        ret->value = exp(m->value);
        r = OBJ_VAL(ret);
    }
    else {
        throw std::runtime_error("number is expected.");
    }
    return r;
}

extern "C" void math_functions(std::vector<std::string>& names, std::vector<NativeFn>& functions)
{
    names.assign(
        {"acos", "asin", "atan", "sin", "cos", "tan", "sqrt", "cbrt", "abs", "phase", "log10", "log2", "log", "exp"});
    functions.assign({acosNative, asinNative, atanNative, sinNative, cosNative, tanNative, sqrtNative, cbrtNative,
                      absNative, phaseNative, log10Native, log2Native, logNative, expNative});
}
extern "C" void math_symbols(std::vector<std::string>& names, std::vector<double>& symbols)
{
    names.assign({"pi"});
    symbols.assign({M_PI});
}
