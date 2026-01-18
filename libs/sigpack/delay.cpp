#include "delay.hpp"

#include <stdexcept>
#include <string>

static Value delay_delay(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    if (1 != argc)
        throw std::runtime_error("invalid arguments.");
    if (!IS_COL(argv[0]))
        throw std::runtime_error("column vector is expected.");
    DelayFilter* df = (DelayFilter*)obj;
    ObjCol* col = factory->newCol();
    col->value = df->m_delay.delay(AS_COL(argv[0])->value);
    return OBJ_VAL(col);
}

// clang-format off
std::map<std::string, NativeObjectBoundFn> s_delay_apis = {
    {"delay", delay_delay}
};

std::map<std::string, NativeObjectBoundProperty> s_delay_properties;
// clang-format on

DelayFilter::DelayFilter() : m_apis(s_delay_apis), m_properties(s_delay_properties)
{
}

DelayFilter::DelayFilter(int nDelay) : m_delay(nDelay), m_apis(s_delay_apis), m_properties(s_delay_properties)
{
}

DelayFilter::~DelayFilter()
{
}

Value DelayFilter::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    std::map<std::string, NativeObjectBoundFn>::iterator it = m_apis.find(name);
    if (it == m_apis.end())
        throw std::runtime_error("invalid method");
    return it->second(factory, this, argc, argv);
}

Value DelayFilter::property(ObjectFactory* factory, std::string name)
{
    std::map<std::string, NativeObjectBoundProperty>::iterator it = m_properties.find(name);
    if (it == m_properties.end())
        throw std::runtime_error("invalid property");
    return it->second(factory, this);
}
