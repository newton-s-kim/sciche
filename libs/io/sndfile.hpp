#include "object.hpp"

#include "sndwav.hpp"

class SndFile : public NativeClass {
public:
    SndFile();
    ~SndFile();
    SndWav* open(std::string path, int mode);
};
