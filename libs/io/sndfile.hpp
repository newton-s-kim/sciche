#include "object.hpp"

#include "sndwav.hpp"

class SndFile : public NativeClass {
public:
    SndFile();
    ~SndFile();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
    Value call(ObjectFactory* factory, int argc, Value* argv);
    Value constant(ObjectFactory* factory, std::string name);
    SndWav* open(std::string path, int mode);
};
