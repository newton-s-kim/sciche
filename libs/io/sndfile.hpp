#pragma once
#include "object.hpp"

#include "sndwav.hpp"

namespace sce {
class SndFile : public NativeClass {
public:
    SndFile();
    ~SndFile();
    SndWav* open(std::string path, int mode);
};
} // namespace sce
