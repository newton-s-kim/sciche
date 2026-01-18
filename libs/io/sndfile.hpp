#include "object.hpp"

#include "sndwav.hpp"

class SndFile : public NativeClass {
protected:
    std::map<std::string, NativeClassBoundFn>& m_apis;
    std::map<std::string, NativeClassBoundProperty>& m_constants;

public:
    SndFile();
    ~SndFile();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
    Value call(ObjectFactory* factory, int argc, Value* argv);
    Value constant(ObjectFactory* factory, std::string name);
    SndWav* open(std::string path, int mode);
};
