#pragma once

#include "object.hpp"
#include "sigpack-1.2.7/sigpack/sigpack.h"

class gPlot : public NativeObject {
private:
    sp::gplot m_gplot;

public:
    gPlot();
    ~gPlot();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
    Value property(ObjectFactory* factory, std::string name);
};
