#include "iir.hpp"

#include <stdexcept>
#include <string>

static Value iir_set_coeffs(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (2 != argc)
        throw std::runtime_error("invalid arguments.");
    if (!IS_COL(argv[0]))
        throw std::runtime_error("column vector is expected.");
    if (!IS_COL(argv[1]))
        throw std::runtime_error("column vector is expected.");
    IirFilter* filter = (IirFilter*)obj;
    filter->iir_filt.set_coeffs(AS_COL(argv[0])->value, AS_COL(argv[1])->value);
    return NIL_VAL;
}

static Value iir_filter(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid arguments.");
    if (!IS_COL(argv[0]))
        throw std::runtime_error("column vector is expected.");
    ObjCol* col = factory->newCol();
    IirFilter* filter = (IirFilter*)obj;
    col->value = filter->iir_filt.filter(AS_COL(argv[0])->value);
    return OBJ_VAL(col);
}

// clang-format off
std::map<std::string, NativeObjectBoundFn> s_iir_apis = {
    {"set_coeffs", iir_set_coeffs},
    {"filter", iir_filter}
};

std::map<std::string, NativeObjectBoundProperty> s_iir_properties;
// clang-format on

IirFilter::IirFilter() : NativeObject(s_iir_apis, s_iir_properties)
{
}

IirFilter::~IirFilter()
{
}

Value IirFilter::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    std::map<std::string, NativeObjectBoundFn>::iterator it = m_apis.find(name);
    if (it == m_apis.end())
        throw std::runtime_error("invalid method");
    return it->second(factory, this, argc, argv);
}

Value IirFilter::property(ObjectFactory* factory, std::string name)
{
    std::map<std::string, NativeObjectBoundProperty>::iterator it = m_properties.find(name);
    if (it == m_properties.end())
        throw std::runtime_error("invalid property");
    return it->second(factory, this);
}
