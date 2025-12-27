#include "sp.hpp"

extern "C" void sigpack_functions(std::vector<std::string>& names, std::vector<NativeFn>& functions)
{
    names.assign(
        {"FIR_filt", "IIR_filt", "fir1", "Delay", "gplot", "linspace", "specgram", "pwelch", "freqz", "phasez", "KF"});
    functions.assign({firFiltNative, iirFiltNative, fir1Native, delayNative, gplotNative, linspaceNative,
                      specgramNative, pwelchNative, freqzNative, phasezNative, kfNative});
}

extern "C" void sigpack_symbols(std::vector<std::string>& names, std::vector<double>& symbols)
{
    names.assign({});
    symbols.assign({});
}
