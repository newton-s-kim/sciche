#include "object.hpp"

#include <sndfile.h>

class SndWav : public NativeClass {
private:
    SNDFILE* m_handle;
    SF_INFO m_info;

public:
    SndWav(std::string path, int mode);
    ~SndWav();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
    Value property(ObjectFactory* factory, std::string name);
};
