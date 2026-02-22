#include "object.hpp"

namespace sce {
ObjClass::ObjClass(nsl::string pName) : Obj(OBJ_CLASS), name(pName)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjClass), type);
#endif
}

ObjClass::~ObjClass()
{
}

std::string ObjClass::stringify(void)
{
    //TODO:return nsl::string
    return name.c_str();
}

void ObjClass::blaken(void)
{
}
} // namespace sce
