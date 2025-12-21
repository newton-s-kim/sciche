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

int math_functions(const char** names[], NativeFn* functions[]) {
    static const char* math_names[] = {
	    "sin", "cos", "tan", "sqrt", "abs", "phase"
    };
    static NativeFn math_fns[] = {
	    sinNative, cosNative, tanNative, sqrtNative, absNative, phaseNative
    };
    *names = math_names;
    *functions = math_fns;
    return sizeof(math_names);
}
