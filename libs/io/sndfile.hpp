#include "object.hpp"

class SndFile : public NativeClass {
private:
public:
    SndFile();
    ~SndFile();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
    Value property(ObjectFactory* factory, std::string name);
};
