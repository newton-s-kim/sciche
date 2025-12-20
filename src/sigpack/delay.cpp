#include "delay.hpp"

#include <stdexcept>
#include <string>

DelayFilter::DelayFilter()
{
}

DelayFilter::DelayFilter(int nDelay) : delay(nDelay)
{
}

DelayFilter::~DelayFilter()
{
}

Value DelayFilter::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    Value ret = 0;
    if ("delay" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid arguments.");
        if (!IS_COL(argv[0]))
            throw std::runtime_error("column vector is expected.");
        ObjCol* col = factory->newCol();
        col->value = delay.delay(AS_COL(argv[0])->value);
    }
    else {
        throw std::runtime_error("non-existent method");
    }
    return ret;
}
