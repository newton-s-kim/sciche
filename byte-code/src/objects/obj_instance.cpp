#include "object.hpp"

#include "dictionary.hpp"

namespace sce {
ObjInstance::ObjInstance(ObjClass* pKlass) : Obj(OBJ_INSTANCE), klass(pKlass), fields(NIL_VAL)
{
    Dictionary dct;
    direct_fields = new Value[dct.size()];
    for (size_t idx = 0; idx < dct.size(); idx++)
        direct_fields[idx] = UNDEF_VAL;
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjInstance), type);
#endif
}

ObjInstance::~ObjInstance()
{
    if (direct_fields)
        delete direct_fields;
}

std::string ObjInstance::stringify(void)
{
    return klass->stringify() + " instance";
}

void ObjInstance::blaken(void)
{
}
} // namespace sce
