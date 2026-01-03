#include "object.hpp"

#include "json.hpp"

extern "C" void scjson_functions(std::vector<std::string>& names, std::vector<NativeFn>& functions)
{
    names.assign({});
    functions.assign({});
}
extern "C" void scjson_symbols(std::vector<std::string>& names, std::vector<NativeClass*>& syms)
{
    names.assign({"json"});
    syms.assign({new JsonInterface()});
}
extern "C" void scjson_constants(std::vector<std::string>& names, std::vector<double>& consts)
{
    names.assign({});
    consts.assign({});
}
