#include "object.hpp"

#include "dictionary.hpp"

namespace sce {
ObjClass::ObjClass(ObjString* pName) : Obj(OBJ_CLASS), name(pName), methods(NIL_VAL)
{
    Dictionary dct;
    direct_methods = new Value[dct.size()];
    for (size_t idx = 0; idx < dct.size(); idx++)
        direct_methods[idx] = UNDEF_VAL;
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjClass), type);
#endif
}

ObjClass::~ObjClass()
{
    if (direct_methods)
        delete direct_methods;
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
