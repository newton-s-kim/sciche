#include "object.hpp"

#include <rapidjson/reader.h>
#include <rapidjson/writer.h>

class JsonInterface : public NativeClass {
private:
    void write(rapidjson::Writer<rapidjson::StringBuffer>& writer, Value value);

public:
    JsonInterface();
    ~JsonInterface();
    Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv);
};
