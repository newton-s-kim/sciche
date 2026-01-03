#include "sp.hpp"

Value sigpackNative(ObjectFactory* factory, int argc, Value* args)
{
    (void)args;
    if (0 != argc)
        throw std::runtime_error("invalid number of arguments.");
    ObjNativeObject* obj = factory->newNativeObj(new SigpackInterface());
    return OBJ_VAL(obj);
}

extern "C" void sigpack_functions(std::vector<std::string>& names, std::vector<NativeFn>& functions)
{
    names.assign({"sigpack"});
    functions.assign({sigpackNative});
}
extern "C" void sigpack_symbols(std::vector<std::string>& names, std::vector<NativeClass*>& syms)
{
    names.assign({});
    syms.assign({});
}
extern "C" void sigpack_constants(std::vector<std::string>& names, std::vector<double>& consts)
{
    names.assign({});
    consts.assign({});
}
