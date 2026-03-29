#include "object.hpp"

#include "dictionary.hpp"

namespace sce {
ObjInstance::ObjInstance(ObjClass* pKlass) : Obj(OBJ_INSTANCE), klass(pKlass), fields(NIL_VAL)
{
    // TODO:performance sensitive path
    Dictionary dct;
    size_t sz = dct.size();
    direct_fields = (Value*)malloc(sizeof(Value) * sz);
    for (Value* v = direct_fields; v < direct_fields + sz; v++)
        *v = UNDEF_VAL;
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjInstance), type);
#endif
}

ObjInstance::~ObjInstance()
{
    if (direct_fields)
        free(direct_fields);
}

std::string ObjInstance::stringify(void)
{
    return klass->stringify() + " instance";
}

void ObjInstance::blaken(void)
{
}
} // namespace sce
