#include "sp.hpp"

#include "sigpack-1.2.7/sigpack/sigpack.h"

using namespace arma;
using namespace sp;

Value firFiltNative(ObjectFactory* factory, int argc, Value* args)
{
}

Value iirFiltNative(ObjectFactory* factory, int argc, Value* args)
{
}

Value fir1Native(ObjectFactory* factory, int argc, Value* args)
{
    ObjCol* vec = factory->newCol();
    if (2 != argc) {
        // TODO: error return
    }
    vec->value = fir1(args[0], args[1]);
    return OBJ_VAL(vec);
}

Value delayNative(ObjectFactory* factory, int argc, Value* args)
{
}
