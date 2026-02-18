#include "object.hpp"

ObjNative::ObjNative(NativeFn pFunction) : Obj(OBJ_NATIVE), function(pFunction)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjNative), type);
#endif
}

std::string ObjNative::stringify(void)
{
    return "<native fn>";
}

void ObjNative::blaken(void)
{
}
