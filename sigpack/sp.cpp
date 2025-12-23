#include "sp.hpp"

#include "delay.hpp"
#include "fir.hpp"
#include "gplot.hpp"
#include "iir.hpp"

#include "sigpack-1.2.7/sigpack/sigpack.h"

using namespace arma;
using namespace sp;

Value firFiltNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)argc;
    (void)args;
    ObjNativeObject* obj = factory->newNativeObj(new FirFilter());
    return OBJ_VAL(obj);
}

Value iirFiltNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)argc;
    (void)args;
    ObjNativeObject* obj = factory->newNativeObj(new IirFilter());
    return OBJ_VAL(obj);
}

Value fir1Native(ObjectFactory* factory, int argc, Value* args)
{
    ObjCol* vec = factory->newCol();
    if (2 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_NUMBER(args[0]))
        throw std::runtime_error("number is expected.");
    if (!IS_NUMBER(args[1]))
        throw std::runtime_error("number is expected.");
    vec->value = fir1(AS_NUMBER(args[0]), AS_NUMBER(args[1]));
    return OBJ_VAL(vec);
}

Value delayNative(ObjectFactory* factory, int argc, Value* args)
{
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_NUMBER(args[0]))
        throw std::runtime_error("number is expected.");
    ObjNativeObject* obj = factory->newNativeObj(new DelayFilter(AS_NUMBER(args[0])));
    return OBJ_VAL(obj);
}

Value gplotNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)argc;
    (void)args;
    ObjNativeObject* obj = factory->newNativeObj(new gPlot());
    return OBJ_VAL(obj);
}

// TODO: move to VM
Value linspaceNative(ObjectFactory* factory, int argc, Value* args)
{
    ObjCol* vec = factory->newCol();
    if (3 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_NUMBER(args[0]))
        throw std::runtime_error("number is expected.");
    if (!IS_NUMBER(args[1]))
        throw std::runtime_error("number is expected.");
    if (!IS_NUMBER(args[2]))
        throw std::runtime_error("number is expected.");
    vec->value = linspace(AS_NUMBER(args[0]), AS_NUMBER(args[1]), AS_NUMBER(args[2]));
    return OBJ_VAL(vec);
}

Value specgramNative(ObjectFactory* factory, int argc, Value* args)
{
    ObjMat* m = factory->newMat();
    if (3 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_COL(args[0]))
        throw std::runtime_error("vec is expected.");
    if (!IS_NUMBER(args[1]))
        throw std::runtime_error("number is expected.");
    if (!IS_NUMBER(args[2]))
        throw std::runtime_error("number is expected.");
    m->value = specgram(AS_COL(args[0])->value, AS_NUMBER(args[1]), AS_NUMBER(args[2]));
    return OBJ_VAL(m);
}

Value pwelchNative(ObjectFactory* factory, int argc, Value* args)
{
    ObjMat* mat = factory->newMat();
    if (3 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_COL(args[0]))
        throw std::runtime_error("vec is expected.");
    if (!IS_NUMBER(args[1]))
        throw std::runtime_error("number is expected.");
    if (!IS_NUMBER(args[2]))
        throw std::runtime_error("number is expected.");
    mat->value = pwelch(AS_COL(args[0])->value, AS_NUMBER(args[1]), AS_NUMBER(args[2]));
    return OBJ_VAL(mat);
}

Value freqzNative(ObjectFactory* factory, int argc, Value* args)
{
    if (3 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_COL(args[0]))
        throw std::runtime_error("vec is expected.");
    if (!IS_COL(args[1]))
        throw std::runtime_error("vec is expected.");
    if (!IS_NUMBER(args[2]))
        throw std::runtime_error("number is expected.");
    ObjCol* c = factory->newCol();
    c->value = freqz(AS_COL(args[0])->value, AS_COL(args[1])->value, AS_NUMBER(args[2]));
    return OBJ_VAL(c);
}

Value phasezNative(ObjectFactory* factory, int argc, Value* args)
{
    if (3 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_COL(args[0]))
        throw std::runtime_error("vec is expected.");
    if (!IS_COL(args[1]))
        throw std::runtime_error("vec is expected.");
    if (!IS_NUMBER(args[2]))
        throw std::runtime_error("number is expected.");
    ObjCol* c = factory->newCol();
    c->value = phasez(AS_COL(args[0])->value, AS_COL(args[1])->value, AS_NUMBER(args[2]));
    return OBJ_VAL(c);
}
