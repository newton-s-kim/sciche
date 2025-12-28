#include "object.hpp"

extern "C" void math_functions(std::vector<std::string>& names, std::vector<NativeFn>& functions)
{
    names.assign({});
    functions.assign({});
}
extern "C" void math_symbols(std::vector<std::string>& names, std::vector<double>& symbols)
{
    names.assign({});
    symbols.assign({});
}
