#pragma once

#include "object.hpp"
#include "sigpack-1.2.7/sigpack/sigpack.h"

namespace sce {
class FirFilter : public NativeObject {
public:
    sp::FIR_filt<double, double, double> fir_filt;

public:
    FirFilter();
    ~FirFilter();
};
} // namespace sce
