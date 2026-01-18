#pragma once

#include "object.hpp"
#include "sigpack-1.2.7/sigpack/sigpack.h"

class FirFilter : public NativeObject {
public:
    sp::FIR_filt<double, double, double> fir_filt;

protected:
    std::map<std::string, NativeObjectBoundFn>& m_apis;
    std::map<std::string, NativeObjectBoundProperty>& m_properties;

public:
    FirFilter();
    ~FirFilter();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
    Value property(ObjectFactory* factory, std::string name);
};
