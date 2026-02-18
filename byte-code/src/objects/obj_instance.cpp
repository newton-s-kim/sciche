#include "object.hpp"

ObjInstance::ObjInstance(ObjClass* pKlass) : Obj(OBJ_INSTANCE), klass(pKlass)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjInstance), type);
#endif
}

ObjInstance::~ObjInstance()
{
}

std::string ObjInstance::stringify(void)
{
    return klass->stringify() + " instance";
}

void ObjInstance::blaken(void)
{
}
