#include "fir.hpp"

#include <stdexcept>
#include <string>

FirFilter::FirFilter()
{
}

FirFilter::~FirFilter()
{
}

Value FirFilter::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    (void)factory;
    Value ret = 0;
    if ("set_coeffs" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid arguments.");
        if (!IS_COL(argv[0]))
            throw std::runtime_error("column vector is expected.");
        fir_filt.set_coeffs(AS_COL(argv[0])->value);
    }
    else if ("step" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid arguments.");
        if (!IS_NUMBER(argv[0]))
            throw std::runtime_error("number is expected.");
        double num = fir_filt(AS_NUMBER(argv[0]));
        ret = NUMBER_VAL(num);
    }
    else {
        throw std::runtime_error("non-existent method");
    }
    return ret;
}
