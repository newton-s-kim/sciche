#include "object.hpp"

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

Value ObjMat::add(Value v, ObjectFactory* factory)
{
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

Value ObjMat::sub(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_MAT(v)) {
        ObjMat* b = AS_MAT(v);
        ObjMat* m = factory->newMat();
        m->value = value - b->value;
        r = OBJ_VAL(m);
    }
    else {
        throw std::runtime_error("unsupported operation -");
    }
    return r;
}

Value ObjMat::mul(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_MAT(v)) {
        ObjMat* b = AS_MAT(v);
        ObjMat* m = factory->newMat();
        m->value = value * b->value;
        r = OBJ_VAL(m);
    }
    else {
        throw std::runtime_error("unsupported operation *");
    }
    return r;
}

// clang-format off
std::map<std::string, NativeClassBoundFn> s_mat_class_apis;

//TODO: refactoring to fillout
static Value mat_class_prop_default(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_DEFAULT);
}

static Value mat_class_prop_zeros(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_ZEROS);
}

static Value mat_class_prop_ones(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_ONES);
}

static Value mat_class_prop_eye(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_EYE);
}

static Value mat_class_prop_randu(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_RANDU);
}

static Value mat_class_prop_randn(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_RANDN);
}

std::map<std::string, NativeClassBoundProperty> s_mat_class_constants = {
    {"default", mat_class_prop_default},
    {"zeros", mat_class_prop_zeros},
    {"ones", mat_class_prop_ones},
    {"eye", mat_class_prop_eye},
    {"randu", mat_class_prop_randu},
    {"randn", mat_class_prop_randn}
};
// clang-format on

matNative::matNative() : NativeClass(s_mat_class_apis, s_mat_class_constants)
{
}

Value matNative::call(ObjectFactory* factory, int argc, Value* args)
{
    ObjFillType fill_type = OBJ_FILL_DEFAULT;
    size_t rows = 0, cols = 0;
    switch (argc) {
    case 3:
        if (!IS_NUMBER(args[2])) {
            throw std::runtime_error("number is expected");
        }
        fill_type = (ObjFillType)AS_NUMBER(args[2]);
        if (!IS_NUMBER(args[0])) {
            throw std::runtime_error("number is expected");
        }
        if (!IS_NUMBER(args[1])) {
            throw std::runtime_error("number is expected");
        }
        rows = AS_NUMBER(args[0]);
        cols = AS_NUMBER(args[1]);
        break;
    case 2:
        if (!IS_NUMBER(args[0]))
            throw std::runtime_error("number is expected");
        if (!IS_NUMBER(args[1]))
            throw std::runtime_error("number is expected");
        rows = AS_NUMBER(args[0]);
        cols = AS_NUMBER(args[1]);
        break;
    case 0:
        break;
    default:
        throw std::runtime_error("invalid arguments");
        break;
    }
    return OBJ_VAL(factory->newMat(rows, cols, fill_type));
}
