#include "iir.hpp"

#include <stdexcept>
#include <string>

IirFilter::IirFilter()
{
}

IirFilter::~IirFilter()
{
}

Value IirFilter::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    Value ret = 0;
    if ("set_coeffs" == name) {
        if (2 != argc)
            throw std::runtime_error("invalid arguments.");
        if (!IS_COL(argv[0]))
            throw std::runtime_error("column vector is expected.");
        if (!IS_COL(argv[1]))
            throw std::runtime_error("column vector is expected.");
        iir_filt.set_coeffs(AS_COL(argv[0])->value, AS_COL(argv[1])->value);
    }
    else if ("filter" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid arguments.");
        if (!IS_COL(argv[0]))
            throw std::runtime_error("column vector is expected.");
        ObjCol* col = factory->newCol();
        col->value = iir_filt.filter(AS_COL(argv[0])->value);
        ret = OBJ_VAL(col);
    }
    else {
        throw std::runtime_error("non-existent method");
    }
    return ret;
}
