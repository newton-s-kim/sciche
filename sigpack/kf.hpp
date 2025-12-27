#pragma once

#include "object.hpp"
#include "sigpack-1.2.7/sigpack/sigpack.h"

class kalmanFilter : public NativeClass {
private:
    sp::KF kalman;

public:
    kalmanFilter(int n, int m, int l);
    ~kalmanFilter();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
};
