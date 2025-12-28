#include "object.hpp"

class JsonInterface : public NativeClass {
public:
    JsonInterface();
    ~JsonInterface();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
};
