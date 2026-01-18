#pragma once

#include "object.hpp"
#include "sigpack-1.2.7/sigpack/sigpack.h"

class gPlot : public NativeObject {
public:
    sp::gplot m_gplot;

public:
    gPlot();
    ~gPlot();
};
