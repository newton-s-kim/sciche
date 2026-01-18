#include "sp.hpp"

#include "delay.hpp"
#include "fir.hpp"
#include "gplot.hpp"
#include "iir.hpp"
#include "kf.hpp"

#include "sigpack-1.2.7/sigpack/sigpack.h"

using namespace arma;
using namespace sp;

Value firFiltNative(ObjectFactory* factory, NativeClass* klass, int argc, Value* args)
{
    (void)argc;
    (void)args;
    (void)klass;
    ObjNativeObject* obj = factory->newNativeObj(new FirFilter());
    return OBJ_VAL(obj);
}

Value iirFiltNative(ObjectFactory* factory, NativeClass* klass, int argc, Value* args)
{
    (void)argc;
    (void)args;
    (void)klass;
    ObjNativeObject* obj = factory->newNativeObj(new IirFilter());
    return OBJ_VAL(obj);
}

Value fir1Native(ObjectFactory* factory, NativeClass* klass, int argc, Value* args)
{
    (void)klass;
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

Value delayNative(ObjectFactory* factory, NativeClass* klass, int argc, Value* args)
{
    (void)klass;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_NUMBER(args[0]))
        throw std::runtime_error("number is expected.");
    ObjNativeObject* obj = factory->newNativeObj(new DelayFilter(AS_NUMBER(args[0])));
    return OBJ_VAL(obj);
}

Value gplotNative(ObjectFactory* factory, NativeClass* klass, int argc, Value* args)
{
    (void)argc;
    (void)args;
    (void)klass;
    ObjNativeObject* obj = factory->newNativeObj(new gPlot());
    return OBJ_VAL(obj);
}

// TODO: move to VM
Value linspaceNative(ObjectFactory* factory, NativeClass* klass, int argc, Value* args)
{
    (void)klass;
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

Value specgramNative(ObjectFactory* factory, NativeClass* klass, int argc, Value* args)
{
    (void)klass;
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

Value pwelchNative(ObjectFactory* factory, NativeClass* klass, int argc, Value* args)
{
    (void)klass;
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

Value freqzNative(ObjectFactory* factory, NativeClass* klass, int argc, Value* args)
{
    (void)klass;
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

Value phasezNative(ObjectFactory* factory, NativeClass* klass, int argc, Value* args)
{
    (void)klass;
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

Value kfNative(ObjectFactory* factory, NativeClass* klass, int argc, Value* args)
{
    (void)klass;
    if (3 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_NUMBER(args[0]))
        throw std::runtime_error("number is expected.");
    if (!IS_NUMBER(args[1]))
        throw std::runtime_error("number is expected.");
    if (!IS_NUMBER(args[2]))
        throw std::runtime_error("number is expected.");
    ObjNativeObject* obj =
        factory->newNativeObj(new kalmanFilter(AS_NUMBER(args[0]), AS_NUMBER(args[1]), AS_NUMBER(args[2])));
    return OBJ_VAL(obj);
}

// clang-format off
std::map<std::string, NativeClassBoundFn> s_sigpack_apis = {
    {"FIR_filt", firFiltNative},
    {"IIR_filt", iirFiltNative},
    {"fir1", fir1Native},
    {"Delay", delayNative},
    {"gplot", gplotNative},
    {"linspace", linspaceNative},
    {"specgram", specgramNative},
    {"pwelch", pwelchNative},
    {"freqz", freqzNative},
    {"phasez", phasezNative},
    {"KF", kfNative}
};

std::map<std::string, NativeClassBoundProperty> s_sigpack_properties;
// clang-format on

SigpackInterface::SigpackInterface() : m_apis(s_sigpack_apis), m_constants(s_sigpack_properties)
{
}

SigpackInterface::~SigpackInterface()
{
}

Value SigpackInterface::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    std::map<std::string, NativeClassBoundFn>::iterator it = m_apis.find(name);
    if (m_apis.end() == it)
        throw std::runtime_error("undefined method");
    return it->second(factory, this, argc, argv);
}

Value SigpackInterface::call(ObjectFactory* factory, int argc, Value* argv)
{
    (void)factory;
    (void)argc;
    (void)argv;
    throw std::runtime_error("undefined call");
    return NIL_VAL;
}

Value SigpackInterface::constant(ObjectFactory* factory, std::string name)
{
    std::map<std::string, NativeClassBoundProperty>::iterator it = m_constants.find(name);
    if (it == m_constants.end())
        throw std::runtime_error("invalid property");
    return it->second(factory, this);
}
