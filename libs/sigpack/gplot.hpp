#pragma once

#include "object.hpp"
#include "sigpack-1.2.7/sigpack/sigpack.h"

class gPlot : public NativeObject {
public:
    sp::gplot m_gplot;

protected:
    std::map<std::string, NativeObjectBoundFn>& m_apis;
    std::map<std::string, NativeObjectBoundProperty>& m_properties;

public:
    gPlot();
    ~gPlot();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
    Value property(ObjectFactory* factory, std::string name);
};
