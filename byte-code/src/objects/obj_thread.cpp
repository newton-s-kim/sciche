#include "object.hpp"

namespace sce {
ObjThread::ObjThread() : Obj(OBJ_THREAD), frameCount(0), stackTop(stack), caller(NULL)
{
}

ObjThread::ObjThread(ObjClosure* closure) : Obj(OBJ_THREAD), frameCount(0), stackTop(stack), caller(NULL)
{
    CallFrame* frame = &frames[frameCount++];
    frame->closure = closure;
    frame->ip = closure->function->chunk.code;
    frames->slots = stackTop - 1;
}

ObjThread::~ObjThread()
{
}

std::string ObjThread::stringify(void)
{
    return "<thread>";
}

void ObjThread::blaken(void)
{
}
} // namespace sce
