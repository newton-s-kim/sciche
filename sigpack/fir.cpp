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
    Value ret = 0;
    if ("set_coeffs" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments.");
        if (!IS_COL(argv[0]))
            throw std::runtime_error("column vector is expected.");
        fir_filt.set_coeffs(AS_COL(argv[0])->value);
    }
    else if ("get_coeffs" == name) {
        if (0 < argc)
            throw std::runtime_error("invalid number of arguments.");
        ObjCol* col = factory->newCol();
        col->value = fir_filt.get_coeffs();
        ret = OBJ_VAL(col);
    }
    else if ("step" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments.");
        if (!IS_NUMBER(argv[0]))
            throw std::runtime_error("number is expected.");
        double num = fir_filt(AS_NUMBER(argv[0]));
        ret = NUMBER_VAL(num);
    }
    else if ("filter" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid arguments.");
        if (!IS_COL(argv[0]))
            throw std::runtime_error("column vector is expected.");
        ObjCol* col = factory->newCol();
        col->value = fir_filt.filter(AS_COL(argv[0])->value);
        ret = OBJ_VAL(col);
    }
    else if ("setup_lms" == name) {
        if (2 != argc)
            throw std::runtime_error("invalid number of arguments.");
        if (!IS_NUMBER(argv[0]))
            throw std::runtime_error("number is expected.");
        if (!IS_NUMBER(argv[1]))
            throw std::runtime_error("number is expected.");
        fir_filt.setup_lms(AS_NUMBER(argv[0]), AS_NUMBER(argv[1]));
    }
    else if ("lms_adapt" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments.");
        if (!IS_NUMBER(argv[0]))
            throw std::runtime_error("number is expected.");
        fir_filt.lms_adapt(AS_NUMBER(argv[0]));
    }
    else {
        throw std::runtime_error("non-existent method");
    }
    return ret;
}
