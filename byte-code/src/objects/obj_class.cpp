#include "object.hpp"

namespace sce {
ObjClass::ObjClass(std::string pName) : Obj(OBJ_CLASS), name(pName)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjClass), type);
#endif
}

std::string ObjClosure::stringify(void)
{
    return function->stringify();
}

void ObjClosure::blaken(void)
{
}

ObjClass::~ObjClass()
{
}

std::string ObjClass::stringify(void)
{
    return name;
}

void ObjClass::blaken(void)
{
}
} // namespace sce
