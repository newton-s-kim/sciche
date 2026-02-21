#include "object.hpp"

namespace sce {
ObjCube::ObjCube() : Obj(OBJ_CUBE)
{
}

ObjCube::~ObjCube()
{
}

std::string ObjCube::stringify(void)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void ObjCube::blaken(void)
{
}

Value ObjCube::get(int row, int col, int depth)
{
    row = rectify_index(row, (int)value.n_rows);
    col = rectify_index(col, (int)value.n_cols);
    depth = rectify_index(depth, (int)value.n_slices);
    return NUMBER_VAL(value(row, col, depth));
}

void ObjCube::set(int row, int col, int depth, Value v)
{
    row = rectify_index(row, (int)value.n_rows);
    col = rectify_index(col, (int)value.n_cols);
    depth = rectify_index(depth, (int)value.n_slices);
    value(row, col, depth) = AS_NUMBER(v);
}
} // namespace sce
