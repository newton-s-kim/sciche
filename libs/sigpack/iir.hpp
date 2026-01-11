#pragma once

#include "object.hpp"
#include "sigpack-1.2.7/sigpack/sigpack.h"

class IirFilter : public NativeObject {
private:
    sp::IIR_filt<double, double, double> iir_filt;

public:
    IirFilter();
    ~IirFilter();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
    Value property(ObjectFactory* factory, std::string name);
};
