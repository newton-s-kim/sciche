#include "object.hpp"

#include "sndfile.hpp"

namespace sce {
extern "C" void io_functions(std::vector<std::string>& names, std::vector<NativeFn>& functions)
{
    names.assign({});
    functions.assign({});
}
extern "C" void io_symbols(std::vector<std::string>& names, std::vector<NativeClass*>& syms)
{
    names.assign({"sndfile"});
    syms.assign({new SndFile()});
}
extern "C" void io_constants(std::vector<std::string>& names, std::vector<double>& consts)
{
    names.assign({});
    consts.assign({});
}
} // namespace sce
