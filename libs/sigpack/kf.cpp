#include "kf.hpp"

#include <stdexcept>
#include <string>

namespace sce {
static Value kf_set_trans_mat(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_MAT(argv[0]))
        throw std::runtime_error("mat is expected.");
    kalmanFilter* kf = (kalmanFilter*)obj;
    kf->kalman.set_trans_mat(AS_MAT(argv[0])->value);
    return NIL_VAL;
}

static Value kf_set_meas_mat(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_MAT(argv[0]))
        throw std::runtime_error("mat is expected.");
    kalmanFilter* kf = (kalmanFilter*)obj;
    kf->kalman.set_meas_mat(AS_MAT(argv[0])->value);
    return NIL_VAL;
}

static Value kf_set_err_cov(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_MAT(argv[0]))
        throw std::runtime_error("mat is expected.");
    kalmanFilter* kf = (kalmanFilter*)obj;
    kf->kalman.set_err_cov(AS_MAT(argv[0])->value);
    return NIL_VAL;
}

static Value kf_set_proc_noise(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_MAT(argv[0]))
        throw std::runtime_error("mat is expected.");
    kalmanFilter* kf = (kalmanFilter*)obj;
    kf->kalman.set_proc_noise(AS_MAT(argv[0])->value);
    return NIL_VAL;
}

static Value kf_set_meas_noise(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_MAT(argv[0]))
        throw std::runtime_error("mat is expected.");
    kalmanFilter* kf = (kalmanFilter*)obj;
    kf->kalman.set_meas_noise(AS_MAT(argv[0])->value);
    return NIL_VAL;
}

static Value kf_predict(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    (void)argv;
    if (0 != argc)
        throw std::runtime_error("invalid number of arguments");
    kalmanFilter* kf = (kalmanFilter*)obj;
    kf->kalman.predict();
    return NIL_VAL;
}

static Value kf_update(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_COL(argv[0]))
        throw std::runtime_error("vec is expected.");
    kalmanFilter* kf = (kalmanFilter*)obj;
    kf->kalman.update(AS_COL(argv[0])->value);
    return NIL_VAL;
}

static Value kf_get_state_vec(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)argv;
    if (0 != argc)
        throw std::runtime_error("invalid number of arguments");
    kalmanFilter* kf = (kalmanFilter*)obj;
    ObjCol* col = factory->newCol();
    col->value = kf->kalman.get_state_vec();
    return OBJ_VAL(col);
}

static Value kf_get_err(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)argv;
    if (0 != argc)
        throw std::runtime_error("invalid number of arguments");
    kalmanFilter* kf = (kalmanFilter*)obj;
    ObjCol* col = factory->newCol();
    col->value = kf->kalman.get_err();
    return OBJ_VAL(col);
}

static Value kf_get_err_cov(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)argv;
    if (0 != argc)
        throw std::runtime_error("invalid number of arguments");
    kalmanFilter* kf = (kalmanFilter*)obj;
    ObjMat* mat = factory->newMat();
    mat->value = kf->kalman.get_err_cov();
    return OBJ_VAL(mat);
}

static Value kf_rts_smooth(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)factory;
    if (4 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_MAT(argv[0]))
        throw std::runtime_error("mat is expected.");
    if (!IS_CUBE(argv[1]))
        throw std::runtime_error("cube is expected.");
    if (!IS_MAT(argv[2]))
        throw std::runtime_error("mat is expected.");
    if (!IS_CUBE(argv[3]))
        throw std::runtime_error("cube is expected.");
    kalmanFilter* kf = (kalmanFilter*)obj;
    kf->kalman.rts_smooth(AS_MAT(argv[0])->value, AS_CUBE(argv[1])->value, AS_MAT(argv[2])->value,
                          AS_CUBE(argv[3])->value);
    return NIL_VAL;
}

// clang-format off
std::unordered_map<std::string_view, NativeObjectBoundFn> s_kf_apis = {
    {"set_trans_mat", kf_set_trans_mat},
    {"set_meas_mat", kf_set_meas_mat},
    {"set_err_cov", kf_set_err_cov},
    {"set_proc_noise", kf_set_proc_noise},
    {"set_meas_noise", kf_set_meas_noise},
    {"predict", kf_predict},
    {"update", kf_update},
    {"get_state_vec", kf_get_state_vec},
    {"get_err", kf_get_err},
    {"get_err_cov", kf_get_err_cov},
    {"rts_smooth", kf_rts_smooth}
};

std::unordered_map<std::string_view, NativeObjectBoundProperty> s_kf_properties;
// clang-format on

kalmanFilter::kalmanFilter(int n, int m, int l) : NativeObject(s_kf_apis, s_kf_properties), kalman(n, m, l)
{
}

kalmanFilter::~kalmanFilter()
{
}
} // namespace sce
