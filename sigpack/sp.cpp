#include "sp.hpp"

#include "delay.hpp"
#include "fir.hpp"
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
