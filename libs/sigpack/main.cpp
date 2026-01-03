#include "sp.hpp"

extern "C" void sigpack_functions(std::vector<std::string>& names, std::vector<NativeFn>& functions)
{
    names.assign({});
    functions.assign({});
}
extern "C" void sigpack_symbols(std::vector<std::string>& names, std::vector<NativeClass*>& syms)
{
    names.assign({"sigpack"});
    syms.assign({new SigpackInterface()});
}
extern "C" void sigpack_constants(std::vector<std::string>& names, std::vector<double>& consts)
{
    names.assign({});
    consts.assign({});
}
