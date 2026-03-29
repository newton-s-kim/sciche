#include "object.hpp"

#include "dictionary.hpp"

namespace sce {
ObjClass::ObjClass(ObjString* pName) : Obj(OBJ_CLASS), name(pName), methods(NIL_VAL)
{
    // TODO:performance sensitive path
    Dictionary dct;
    size_t sz = dct.size();
    direct_methods = (Value*)malloc(sizeof(Value) * sz);
    for (Value* v = direct_methods; v < direct_methods + sz; v++)
        *v = UNDEF_VAL;
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjClass), type);
#endif
}

ObjClass::~ObjClass()
{
    if (direct_methods)
        free(direct_methods);
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
