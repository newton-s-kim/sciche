#include "object.hpp"

namespace sce {
ObjMat::ObjMat() : Obj(OBJ_MAT)
{
}

ObjMat::~ObjMat()
{
}

std::string ObjMat::stringify(void)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void ObjMat::blaken(void)
{
}

Value ObjMat::get(int row, int col)
{
    row = rectify_index(row, (int)value.n_rows);
    col = rectify_index(col, (int)value.n_cols);
    return NUMBER_VAL(value(row, col));
}

void ObjMat::set(int row, int col, Value v)
{
    row = rectify_index(row, (int)value.n_rows);
    col = rectify_index(col, (int)value.n_cols);
    value(row, col) = AS_NUMBER(v);
}

Value ObjMat::add(Value v, ObjectFactory* factory, bool opposite)
{
    (void)opposite;
    Value r = NIL_VAL;
    if (IS_MAT(v)) {
        ObjMat* b = AS_MAT(v);
        ObjMat* m = factory->newMat();
        m->value = value + b->value;
        r = OBJ_VAL(m);
    }
    else {
        throw std::runtime_error("unsupported operation +");
    }
    return r;
}

Value ObjMat::sub(Value v, ObjectFactory* factory, bool opposite)
{
    Value r = NIL_VAL;
    if (IS_MAT(v)) {
        ObjMat* b = AS_MAT(v);
        ObjMat* m = factory->newMat();
        m->value = (opposite) ? b->value - value : value - b->value;
        r = OBJ_VAL(m);
    }
    else {
        throw std::runtime_error("unsupported operation -");
    }
    return r;
}

Value ObjMat::mul(Value v, ObjectFactory* factory, bool opposite)
{
    Value r = NIL_VAL;
    if (IS_MAT(v)) {
        ObjMat* b = AS_MAT(v);
        ObjMat* m = factory->newMat();
        m->value = (opposite) ? b->value * value : value * b->value;
        r = OBJ_VAL(m);
    }
    else {
        throw std::runtime_error("unsupported operation *");
    }
    return r;
}

} // namespace sce
