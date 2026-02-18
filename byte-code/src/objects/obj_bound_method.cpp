#include "object.hpp"

ObjBoundMethod::ObjBoundMethod(Value pReceiver, ObjClosure* pMethod)
    : Obj(OBJ_BOUND_METHOD), receiver(pReceiver), method(pMethod)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjBoundMethod), type);
#endif
}

std::string ObjBoundMethod::stringify(void)
{
    return method->function->stringify();
}

void ObjBoundMethod::blaken(void)
{
}
