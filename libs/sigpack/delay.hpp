#pragma once

#include "object.hpp"
#include "sigpack-1.2.7/sigpack/sigpack.h"

class DelayFilter : public NativeObject {
private:
    sp::Delay<double> m_delay;

protected:
    std::map<std::string, NativeObjectBoundFn>& m_apis;
    std::map<std::string, NativeObjectBoundProperty>& m_properties;

public:
    DelayFilter();
    DelayFilter(int nDelay);
    ~DelayFilter();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
    Value property(ObjectFactory* factory, std::string name);
    arma::vec delay(arma::vec input);
};
