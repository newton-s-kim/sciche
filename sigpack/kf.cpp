#include "kf.hpp"

#include <stdexcept>
#include <string>

kalmanFilter::kalmanFilter(int n, int m, int l) : kalman(n, m, l)
{
}

kalmanFilter::~kalmanFilter()
{
}

Value kalmanFilter::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    Value ret = 0;
    if ("set_trans_mat" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments");
        if (!IS_MAT(argv[0]))
            throw std::runtime_error("mat is expected.");
        kalman.set_trans_mat(AS_MAT(argv[0])->value);
    }
    else if ("set_meas_mat" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments");
        if (!IS_MAT(argv[0]))
            throw std::runtime_error("mat is expected.");
        kalman.set_meas_mat(AS_MAT(argv[0])->value);
    }
    else if ("set_err_cov" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments");
        if (!IS_MAT(argv[0]))
            throw std::runtime_error("mat is expected.");
        kalman.set_err_cov(AS_MAT(argv[0])->value);
    }
    else if ("set_proc_noise" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments");
        if (!IS_MAT(argv[0]))
            throw std::runtime_error("mat is expected.");
        kalman.set_proc_noise(AS_MAT(argv[0])->value);
    }
    else if ("set_meas_noise" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments");
        if (!IS_MAT(argv[0]))
            throw std::runtime_error("mat is expected.");
        kalman.set_meas_noise(AS_MAT(argv[0])->value);
    }
    else if ("predict" == name) {
        if (0 != argc)
            throw std::runtime_error("invalid number of arguments");
        kalman.predict();
    }
    else if ("update" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments");
        if (!IS_COL(argv[0]))
            throw std::runtime_error("vec is expected.");
        kalman.update(AS_COL(argv[0])->value);
    }
    else if ("get_state_vec" == name) {
        if (0 != argc)
            throw std::runtime_error("invalid number of arguments");
        ObjCol* col = factory->newCol();
        col->value = kalman.get_state_vec();
        ret = OBJ_VAL(col);
    }
    else if ("get_err" == name) {
        if (0 != argc)
            throw std::runtime_error("invalid number of arguments");
        ObjCol* col = factory->newCol();
        col->value = kalman.get_err();
        ret = OBJ_VAL(col);
    }
    else if ("get_err_cov" == name) {
        if (0 != argc)
            throw std::runtime_error("invalid number of arguments");
        ObjMat* mat = factory->newMat();
        mat->value = kalman.get_err_cov();
        ret = OBJ_VAL(mat);
    }
    else if ("rts_smooth" == name) {
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
        kalman.rts_smooth(AS_MAT(argv[0])->value, AS_CUBE(argv[1])->value, AS_MAT(argv[2])->value,
                          AS_CUBE(argv[3])->value);
    }
    return ret;
}

Value kalmanFilter::property(ObjectFactory* factory, std::string name)
{
    (void)factory;
    (void)name;
    throw std::runtime_error("invalid property");
}
