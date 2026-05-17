#pragma once
#include "object.hpp"

#include "sndwav.hpp"

namespace sce {
class SndFile : public NativeClass {
public:
    SndFile(ObjectFactory* factory);
    ~SndFile();
    SndWav* open(std::string path, int mode);
};
} // namespace sce
