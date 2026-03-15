#include "mat.hpp"

namespace sce {
// clang-format off
std::unordered_map<std::string_view, NativeClassBoundFn> s_mat_class_apis;

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

std::unordered_map<std::string_view, NativeClassBoundProperty> s_mat_class_constants = {
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
} // namespace sce
