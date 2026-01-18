#include "fir.hpp"

#include <stdexcept>
#include <string>

static Value fir_set_coeffs(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_COL(argv[0]))
        throw std::runtime_error("column vector is expected.");
    FirFilter* filter = (FirFilter*)obj;
    filter->fir_filt.set_coeffs(AS_COL(argv[0])->value);
    return NIL_VAL;
}

static Value fir_update_coeffs(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_COL(argv[0]))
        throw std::runtime_error("column vector is expected.");
    FirFilter* filter = (FirFilter*)obj;
    filter->fir_filt.update_coeffs(AS_COL(argv[0])->value);
    return NIL_VAL;
}

static Value fir_get_coeffs(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)argv;
    if (0 < argc)
        throw std::runtime_error("invalid number of arguments.");
    FirFilter* filter = (FirFilter*)obj;
    ObjCol* col = factory->newCol();
    col->value = filter->fir_filt.get_coeffs();
    return OBJ_VAL(col);
}

static Value fir_step(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_NUMBER(argv[0]))
        throw std::runtime_error("number is expected.");
    FirFilter* filter = (FirFilter*)obj;
    double num = filter->fir_filt(AS_NUMBER(argv[0]));
    return NUMBER_VAL(num);
}

static Value fir_filter(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    if (1 != argc)
        throw std::runtime_error("invalid arguments.");
    if (!IS_COL(argv[0]))
        throw std::runtime_error("column vector is expected.");
    FirFilter* filter = (FirFilter*)obj;
    ObjCol* col = factory->newCol();
    col->value = filter->fir_filt.filter(AS_COL(argv[0])->value);
    return OBJ_VAL(col);
}

static Value fir_setup_lms(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (2 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_NUMBER(argv[0]))
        throw std::runtime_error("number is expected.");
    if (!IS_NUMBER(argv[1]))
        throw std::runtime_error("number is expected.");
    FirFilter* filter = (FirFilter*)obj;
    filter->fir_filt.setup_lms(AS_NUMBER(argv[0]), AS_NUMBER(argv[1]));
    return NIL_VAL;
}

static Value fir_lms_adapt(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_NUMBER(argv[0]))
        throw std::runtime_error("number is expected.");
    FirFilter* filter = (FirFilter*)obj;
    filter->fir_filt.lms_adapt(AS_NUMBER(argv[0]));
    return NIL_VAL;
}

static Value fir_setup_kalman(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (4 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_NUMBER(argv[0]))
        throw std::runtime_error("number is expected.");
    if (!IS_NUMBER(argv[1]))
        throw std::runtime_error("number is expected.");
    if (!IS_NUMBER(argv[2]))
        throw std::runtime_error("number is expected.");
    if (!IS_NUMBER(argv[3]))
        throw std::runtime_error("number is expected.");
    FirFilter* filter = (FirFilter*)obj;
    filter->fir_filt.setup_kalman(AS_NUMBER(argv[0]), AS_NUMBER(argv[1]), AS_NUMBER(argv[2]), AS_NUMBER(argv[3]));
    return NIL_VAL;
}

static Value fir_kalman_adapt(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_NUMBER(argv[0]))
        throw std::runtime_error("number is expected.");
    FirFilter* filter = (FirFilter*)obj;
    filter->fir_filt.kalman_adapt(AS_NUMBER(argv[0]));
    return NIL_VAL;
}

// clang-format off
std::map<std::string, NativeObjectBoundFn> s_fir_apis = {
    {"set_coeffs", fir_set_coeffs},
    {"update_coeffs", fir_update_coeffs},
    {"get_coeffs", fir_get_coeffs},
    {"step", fir_step},
    {"filter", fir_filter},
    {"setup_lms", fir_setup_lms},
    {"lms_adapt", fir_lms_adapt},
    {"setup_kalman", fir_setup_kalman},
    {"kalman_adapt", fir_kalman_adapt}
};

std::map<std::string, NativeObjectBoundProperty> s_fir_properties;
// clang-format on

FirFilter::FirFilter() : NativeObject(s_fir_apis, s_fir_properties)
{
}

FirFilter::~FirFilter()
{
}

Value FirFilter::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    std::map<std::string, NativeObjectBoundFn>::iterator it = m_apis.find(name);
    if (it == m_apis.end())
        throw std::runtime_error("invalid method");
    return it->second(factory, this, argc, argv);
}

Value FirFilter::property(ObjectFactory* factory, std::string name)
{
    std::map<std::string, NativeObjectBoundProperty>::iterator it = m_properties.find(name);
    if (it == m_properties.end())
        throw std::runtime_error("invalid property");
    return it->second(factory, this);
}
