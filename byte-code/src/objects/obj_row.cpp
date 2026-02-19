#include "object.hpp"

ObjRow::ObjRow() : Obj(OBJ_ROW)
{
}

ObjRow::~ObjRow()
{
}

std::string ObjRow::stringify(void)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void ObjRow::blaken(void)
{
}

Value ObjRow::get(int index)
{
    index = rectify_index(index, (int)value.n_elem);
    LAX_LOG("vec[%d] is peeked.", index);
    return NUMBER_VAL(value[index]);
}

void ObjRow::set(int index, Value v)
{
    index = rectify_index(index, (int)value.n_elem);
    value[index] = AS_NUMBER(v);
}

Value ObjRow::add(Value v, ObjectFactory* factory, bool opposite)
{
    (void)opposite;
    Value ret = NIL_VAL;
    if (IS_ROW(v)) {
        ObjRow* b = AS_ROW(v);
        ObjRow* r = factory->newRow();
        r->value = value + b->value;
        ret = OBJ_VAL(r);
    }
    else if (IS_MAT(v)) {
        ObjMat* b = AS_MAT(v);
        ObjRow* r = factory->newRow();
        r->value = value + b->value;
        ret = OBJ_VAL(r);
    }
    else {
        throw std::runtime_error("unsupported operator +");
    }
    return ret;
}

Value ObjRow::sub(Value v, ObjectFactory* factory, bool opposite)
{
    Value ret = NIL_VAL;
    if (IS_ROW(v)) {
        ObjRow* b = AS_ROW(v);
        ObjRow* r = factory->newRow();
        r->value = (opposite) ? b->value - value : value - b->value;
        ret = OBJ_VAL(r);
    }
    else if (IS_MAT(v)) {
        if (opposite)
            throw std::runtime_error("unsupported operator -");
        ObjMat* b = AS_MAT(v);
        ObjRow* r = factory->newRow();
        r->value = value - b->value;
        ret = OBJ_VAL(r);
    }
    else {
        throw std::runtime_error("unsupported operator -");
    }
    return ret;
}

Value ObjRow::mul(Value v, ObjectFactory* factory, bool opposite)
{
    Value ret = NIL_VAL;
    if (IS_ROW(v)) {
        ObjRow* b = AS_ROW(v);
        ObjRow* r = factory->newRow();
        r->value = value * b->value;
        ret = OBJ_VAL(r);
    }
    else if (IS_MAT(v)) {
        if (opposite)
            throw std::runtime_error("unsupported operator *");
        ObjMat* b = AS_MAT(v);
        ObjRow* r = factory->newRow();
        r->value = value * b->value;
        ret = OBJ_VAL(r);
    }
    else {
        throw std::runtime_error("unsupported operator *");
    }
    return ret;
}

Value ObjRow::div(Value v, ObjectFactory* factory, bool opposite)
{
    Value ret = NIL_VAL;
    if (IS_ROW(v)) {
        ObjRow* b = AS_ROW(v);
        ObjRow* r = factory->newRow();
        r->value = (opposite) ? b->value / value : value / b->value;
        ret = OBJ_VAL(r);
    }
    else if (IS_MAT(v)) {
        if (opposite)
            throw std::runtime_error("unsupported operator /");
        ObjMat* b = AS_MAT(v);
        ObjRow* r = factory->newRow();
        r->value = value / b->value;
        ret = OBJ_VAL(r);
    }
    else {
        throw std::runtime_error("unsupported operator /");
    }
    return ret;
}
