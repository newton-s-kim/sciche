#include "object.hpp"

namespace sce {
ObjCol::ObjCol() : Obj(OBJ_COL)
{
}

ObjCol::~ObjCol()
{
}

std::string ObjCol::stringify(void)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void ObjCol::blaken(void)
{
}

Value ObjCol::get(int index)
{
    LAX_LOG("size is %lld", value.n_elem);
    index = rectify_index(index, (int)value.n_elem);
    LAX_LOG("vec[%d] is peeked.", index);
    return NUMBER_VAL(value[index]);
}

void ObjCol::set(int index, Value v)
{
    index = rectify_index(index, (int)value.n_elem);
    LAX_LOG("vec[%d] is set.", index);
    value[index] = AS_NUMBER(v);
}

Value ObjCol::add(Value v, ObjectFactory* factory, bool opposite)
{
    (void)opposite;
    Value r = NIL_VAL;
    if (IS_COL(v)) {
        ObjCol* b = AS_COL(v);
        ObjCol* c = factory->newCol();
        c->value = value + b->value;
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation +");
    }
    return r;
}

Value ObjCol::sub(Value v, ObjectFactory* factory, bool opposite)
{
    Value r = NIL_VAL;
    if (IS_COL(v)) {
        ObjCol* b = AS_COL(v);
        ObjCol* c = factory->newCol();
        c->value = (opposite) ? b->value - value : value - b->value;
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation -");
    }
    return r;
}

Value ObjCol::mul(Value v, ObjectFactory* factory, bool opposite)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        ObjCol* c = factory->newCol();
        c->value = value * b;
        r = OBJ_VAL(c);
    }
    else if (IS_COL(v)) {
        ObjCol* b = AS_COL(v);
        ObjCol* c = factory->newCol();
        c->value = (opposite) ? b->value * value : value * b->value;
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation *");
    }
    return r;
}

Value ObjCol::div(Value v, ObjectFactory* factory, bool opposite)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        if (opposite)
            throw std::runtime_error("unsupported operation /");
        double b = AS_NUMBER(v);
        ObjCol* c = factory->newCol();
        c->value = value / b;
        r = OBJ_VAL(c);
    }
    else if (IS_COL(v)) {
        ObjCol* b = AS_COL(v);
        ObjCol* c = factory->newCol();
        c->value = (opposite) ? b->value / value : value / b->value;
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation /");
    }
    return r;
}

Value ObjCol::mod(Value v, ObjectFactory* factory, bool opposite)
{
    Value r = NIL_VAL;
    if (IS_COL(v)) {
        ObjCol* b = AS_COL(v);
        ObjCol* c = factory->newCol();
        c->value = (opposite) ? b->value % value : value % b->value;
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation /");
    }
    return r;
}

Value ObjCol::pow(Value v, ObjectFactory* factory, bool opposite)
{
    Value r = NIL_VAL;
    if (IS_NUMBER(v)) {
        if (opposite)
            throw std::runtime_error("unsupported operation ^");
        double b = AS_NUMBER(v);
        ObjCol* c = factory->newCol();
        c->value = arma::pow(value, b);
        r = OBJ_VAL(c);
    }
    else if (IS_COL(v)) {
        ObjCol* b = AS_COL(v);
        ObjCol* c = factory->newCol();
        c->value = (opposite) ? arma::pow(b->value, value) : arma::pow(value, b->value);
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operation /");
    }
    return r;
}
} // namespace sce
