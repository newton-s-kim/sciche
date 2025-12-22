#include "sp.hpp"

extern "C" void sigpack_functions(std::vector<std::string>& names, std::vector<NativeFn>& functions)
{
    names.assign({"FIR_filt", "IIR_filt", "fir1", "Delay", "gplot", "linspace", "specgram", "pwelch"});
    functions.assign({firFiltNative, iirFiltNative, fir1Native, delayNative, gplotNative, linspaceNative,
                      specgramNative, pwelchNative});
}
