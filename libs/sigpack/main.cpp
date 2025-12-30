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
    /*
        names.assign(
            {"FIR_filt", "IIR_filt", "fir1", "Delay", "gplot", "linspace", "specgram", "pwelch", "freqz", "phasez",
       "KF"}); functions.assign({firFiltNative, iirFiltNative, fir1Native, delayNative, gplotNative, linspaceNative,
                          specgramNative, pwelchNative, freqzNative, phasezNative, kfNative});
    */
    names.assign({"sigpack"});
    functions.assign({sigpackNative});
}

extern "C" void sigpack_symbols(std::vector<std::string>& names, std::vector<double>& symbols)
{
    names.assign({});
    symbols.assign({});
}
