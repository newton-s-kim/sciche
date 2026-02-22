#include "object.hpp"

namespace sce {
ObjClosure::ObjClosure(ObjFunction* pFunction) : Obj(OBJ_CLOSURE), function(pFunction)
{
    upvalues.reserve(function->upvalueCount);
    upvalueCount = function->upvalueCount;
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjClosure), type);
#endif
}

std::string ObjClosure::stringify(void)
{
    return function->stringify();
}

void ObjClosure::blaken(void)
{
}

} // namespace sce
