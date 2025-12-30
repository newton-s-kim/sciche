#include "object.hpp"

#include "json.hpp"

Value jsonNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)args;
    if (0 != argc)
        throw std::runtime_error("invalid number of arguments.");
    ObjNativeObject* obj = factory->newNativeObj(new JsonInterface());
    return OBJ_VAL(obj);
}

extern "C" void scjson_functions(std::vector<std::string>& names, std::vector<NativeFn>& functions)
{
    names.assign({"json"});
    functions.assign({jsonNative});
}
extern "C" void scjson_symbols(std::vector<std::string>& names, std::vector<double>& symbols)
{
    names.assign({});
    symbols.assign({});
}
