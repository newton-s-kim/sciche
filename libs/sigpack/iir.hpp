#pragma once

#include "object.hpp"
#include "sigpack-1.2.7/sigpack/sigpack.h"

class IirFilter : public NativeObject {
public:
    sp::IIR_filt<double, double, double> iir_filt;

public:
    IirFilter();
    ~IirFilter();
};
