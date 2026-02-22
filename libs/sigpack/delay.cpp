#include "delay.hpp"

#include <stdexcept>
#include <string>

namespace sce {
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
std::unordered_map<std::string_view, NativeObjectBoundFn> s_delay_apis = {
    {"delay", delay_delay}
};

std::unordered_map<std::string_view, NativeObjectBoundProperty> s_delay_properties;
// clang-format on

DelayFilter::DelayFilter() : NativeObject(s_delay_apis, s_delay_properties)
{
}

DelayFilter::DelayFilter(int nDelay) : NativeObject(s_delay_apis, s_delay_properties), m_delay(nDelay)
{
}

DelayFilter::~DelayFilter()
{
}
} // namespace sce
