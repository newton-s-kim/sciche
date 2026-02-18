#include "object.hpp"

ObjComplex::ObjComplex(const std::complex<double> v) : Obj(OBJ_COMPLEX), value(v)
{
}
ObjComplex::~ObjComplex()
{
}

std::string ObjComplex::stringify(void)
{
    std::stringstream ss;
    double rv = value.real();
    double iv = value.imag();
    if (0 != rv) {
        ss << rv;
        if (iv >= 0) {
            ss << "+" << iv;
        }
        else {
            ss << iv;
        }
    }
    else {
        ss << iv;
    }
    ss << "j";
    return ss.str();
}

void ObjComplex::blaken(void)
{
}

Value ObjComplex::add(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value.real() + b, value.imag()));
        r = OBJ_VAL(c);
    }
    else if (IS_COMPLEX(v)) {
        ObjComplex* b = AS_COMPLEX(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value + b->value));
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation +");
    }
    return r;
}

Value ObjComplex::sub(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value.real() - b, value.imag()));
        r = OBJ_VAL(c);
    }
    else if (IS_COMPLEX(v)) {
        ObjComplex* b = AS_COMPLEX(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value - b->value));
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation -");
    }
    return r;
}

Value ObjComplex::mul(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value.real() * b, value.imag() * b));
        r = OBJ_VAL(c);
    }
    else if (IS_COMPLEX(v)) {
        ObjComplex* b = AS_COMPLEX(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value * b->value));
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation *");
    }
    return r;
}

Value ObjComplex::div(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        std::complex<double> t(b, 0);
        ObjComplex* c = factory->newComplex(std::complex<double>(value / t));
        r = OBJ_VAL(c);
    }
    else if (IS_COMPLEX(v)) {
        ObjComplex* b = AS_COMPLEX(v);
        ObjComplex* c = factory->newComplex(std::complex<double>(value / b->value));
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation /");
    }
    return r;
}
