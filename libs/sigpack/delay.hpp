#pragma once

#include "object.hpp"
#include "sigpack-1.2.7/sigpack/sigpack.h"

class DelayFilter : public NativeObject {
private:
    sp::Delay<double> delay;

public:
    DelayFilter();
    DelayFilter(int nDelay);
    ~DelayFilter();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
    Value property(ObjectFactory* factory, std::string name);
};
