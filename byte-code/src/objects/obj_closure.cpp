#include "object.hpp"

ObjClosure::ObjClosure(ObjFunction* pFunction) : Obj(OBJ_CLOSURE), function(pFunction)
{
    upvalues.reserve(function->upvalueCount);
    upvalueCount = function->upvalueCount;
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjClosure), type);
#endif
}
