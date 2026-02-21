#pragma once

#include "object.hpp"
#include "sigpack-1.2.7/sigpack/sigpack.h"

namespace sce {
class kalmanFilter : public NativeObject {
public:
    sp::KF kalman;

public:
    kalmanFilter(int n, int m, int l);
    ~kalmanFilter();
};
} // namespace sce
