#include "object.hpp"

static Value sinNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    (void)argc;
    if (IS_NUMBER(args[0])) {
        return NUMBER_VAL(sin(AS_NUMBER(args[0])));
    }
    return NUMBER_VAL(0);
}

static Value cosNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    (void)argc;
    if (IS_NUMBER(args[0])) {
        return NUMBER_VAL(cos(AS_NUMBER(args[0])));
    }
    return NUMBER_VAL(0);
}

static Value tanNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    (void)argc;
    if (IS_NUMBER(args[0])) {
        return NUMBER_VAL(tan(AS_NUMBER(args[0])));
    }
    return NUMBER_VAL(0);
}

static Value sqrtNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    (void)argc;
    if (IS_NUMBER(args[0])) {
        double v = AS_NUMBER(args[0]);
        if (0 <= v) {
            return NUMBER_VAL(sqrt(AS_NUMBER(args[0])));
        }
        else {
            ObjComplex* cmplx = factory->newComplex(std::complex<double>(0, sqrt(-AS_NUMBER(args[0]))));
            return OBJ_VAL(cmplx);
        }
    }
    return NUMBER_VAL(0);
}

static Value absNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    (void)argc;
    if (IS_NUMBER(args[0])) {
        return NUMBER_VAL(abs(AS_NUMBER(args[0])));
    }
    else if (IS_COMPLEX(args[0])) {
        return NUMBER_VAL(abs(AS_COMPLEX(args[0])->value));
    }
    return NUMBER_VAL(0);
}

static Value phaseNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    (void)argc;
    (void)args;
    if (IS_COMPLEX(args[0])) {
        return NUMBER_VAL(arg(AS_COMPLEX(args[0])->value));
    }
    return NUMBER_VAL(0);
}

static Value log10Native(ObjectFactory* factory, int argc, Value* args)
{
    (void)factory;
    (void)argc;
    (void)args;
    Value r = 0;
    if (IS_NUMBER(args[0])) {
        double v = AS_NUMBER(args[0]);
        if (0 > v) {
            throw std::runtime_error("positive number is expected.");
        }
        r = NUMBER_VAL(log10(v));
    }
    else if (IS_COL(args[0])) {
        ObjCol* c = AS_COL(args[0]);
        ObjCol* ret = factory->newCol();
        ret->value = log10(c->value);
        r = OBJ_VAL(ret);
    }
    else {
        throw std::runtime_error("number is expected.");
    }
    return r;
}

extern "C" void math_functions(std::vector<std::string>& names, std::vector<NativeFn>& functions)
{
    names.assign({"sin", "cos", "tan", "sqrt", "abs", "phase", "log10"});
    functions.assign({sinNative, cosNative, tanNative, sqrtNative, absNative, phaseNative, log10Native});
}
