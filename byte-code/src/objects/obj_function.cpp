#include "object.hpp"

ObjFunction::ObjFunction() : Obj(OBJ_FUNCTION)
{
    arity = 0;
    //> Closures init-upvalue-count
    upvalueCount = 0;
    //< Closures init-upvalue-count
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjFunction), type);
#endif
}

ObjFunction::~ObjFunction()
{
}

std::string ObjFunction::stringify(void)
{
    std::string ret;
    //> print-script
    if (name.empty()) {
        ret = "<script>";
        return ret;
    }
    //< print-script
    return "<fn " + name + ">";
}

void ObjFunction::blaken(void)
{
}
