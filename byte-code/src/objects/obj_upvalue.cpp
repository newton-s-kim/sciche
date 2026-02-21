#include "object.hpp"

namespace sce {
ObjUpvalue::ObjUpvalue(Value* slot) : Obj(OBJ_UPVALUE)
{
    //> init-closed
    closed = NIL_VAL;
    //< init-closed
    location = slot;
    //> init-next
    next = NULL;
    //< init-next
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjUpvalue), type);
#endif
}

std::string ObjUpvalue::stringify(void)
{
    return "upvalue";
}

void ObjUpvalue::blaken(void)
{
}
} // namespace sce
