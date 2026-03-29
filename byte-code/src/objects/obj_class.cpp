#include "object.hpp"

#include "dictionary.hpp"

namespace sce {
ObjClass::ObjClass(ObjString* pName) : Obj(OBJ_CLASS), name(pName), methods(NIL_VAL)
{
    // TODO:performance sensitive path
    memset(direct_methods, 0, MEMBER_DICITONARY_SIZE * sizeof(Value));
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjClass), type);
#endif
}

ObjClass::~ObjClass()
{
}

std::string ObjClass::stringify(void)
{
    // TODO:return nsl::string
    return name->chars;
}

void ObjClass::blaken(void)
{
}
} // namespace sce
