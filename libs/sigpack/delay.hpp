#pragma once

#include "object.hpp"
#include "sigpack-1.2.7/sigpack/sigpack.h"

namespace sce {
class DelayFilter : public NativeObject {
public:
    sp::Delay<double> m_delay;

public:
    DelayFilter();
    DelayFilter(int nDelay);
    ~DelayFilter();
};
} // namespace sce
