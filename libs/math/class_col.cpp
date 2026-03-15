#include "mat.hpp"

namespace sce {
static Value vec_class_convolute(ObjectFactory* factory, NativeClass* klass, int argc, Value* argv)
{
    (void)klass;
    if (2 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_COL(argv[0]))
        throw std::runtime_error("vec is expected.");
    if (!IS_COL(argv[1]))
        throw std::runtime_error("vec is expected.");
    ObjCol* col = factory->newCol();
    col->value = arma::conv(AS_COL(argv[0])->value, AS_COL(argv[1])->value);
    return OBJ_VAL(col);
}

static Value vec_class_correlate(ObjectFactory* factory, NativeClass* klass, int argc, Value* argv)
{
    (void)klass;
    ObjCol* col = factory->newCol();
    arma::vec x;
    if (2 == argc) {
        if (!IS_COL(argv[0]))
            throw std::runtime_error("vec is expected.");
        if (!IS_COL(argv[1]))
            throw std::runtime_error("vec is expected.");
        x = AS_COL(argv[1])->value;
    }
    else if (1 == argc) {
        if (!IS_COL(argv[0]))
            throw std::runtime_error("vec is expected.");
        x = AS_COL(argv[0])->value;
    }
    else {
        throw std::runtime_error("invalid number of arguments.");
    }
    col->value.resize(x.size());
    for (size_t idx = 0; idx < x.size(); idx++) {
        arma::vec s = arma::shift(x, idx);
        arma::vec y = arma::cor(AS_COL(argv[0])->value, s);
        col->value[idx] = y[0];
    }
    return OBJ_VAL(col);
}

static Value vec_class_prop_default(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_DEFAULT);
}

static Value vec_class_prop_zeros(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_ZEROS);
}

static Value vec_class_prop_ones(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_ONES);
}

static Value vec_class_prop_eye(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_EYE);
}

static Value vec_class_prop_randu(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_RANDU);
}

static Value vec_class_prop_randn(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(OBJ_FILL_RANDN);
}

// clang-format off
std::unordered_map<std::string_view, NativeClassBoundFn> s_vec_class_apis = {
    {"conv", vec_class_convolute},
    {"corr", vec_class_correlate}
};

std::unordered_map<std::string_view, NativeClassBoundProperty> s_vec_class_constants = {
    {"default", vec_class_prop_default},
    {"zeros", vec_class_prop_zeros},
    {"ones", vec_class_prop_ones},
    {"eye", vec_class_prop_eye},
    {"randu", vec_class_prop_randu},
    {"randn", vec_class_prop_randn}
};
// clang-format on

vecNative::vecNative() : NativeClass(s_vec_class_apis, s_vec_class_constants)
{
}

Value vecNative::call(ObjectFactory* factory, int argc, Value* args)
{
    ObjFillType fill_type = OBJ_FILL_DEFAULT;
    size_t sz = 0;
    switch (argc) {
    case 2:
        if (!IS_NUMBER(args[1])) {
            throw std::runtime_error("number is expected");
        }
        fill_type = (ObjFillType)AS_NUMBER(args[1]);
        if (!IS_NUMBER(args[0])) {
            throw std::runtime_error("number is expected");
        }
        sz = AS_NUMBER(args[0]);
        break;
    case 1:
        if (!IS_NUMBER(args[0])) {
            throw std::runtime_error("number is expected");
        }
        sz = AS_NUMBER(args[0]);
        break;
    case 0:
        break;
    default:
        throw std::runtime_error("invalid arguments");
        break;
    }
    return OBJ_VAL(factory->newCol(sz, fill_type));
}
} // namespace sce
