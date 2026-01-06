#pragma once
#include "object.hpp"

//< stack-max
/* A Virtual Machine stack-max < Calls and Functions frame-max
#define STACK_MAX 256
*/
//> Calls and Functions frame-max
#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)
//< Calls and Functions frame-max
//> Calls and Functions call-frame

class CallFrame {
public:
    /* Calls and Functions call-frame < Closures call-frame-closure
      ObjFunction* function;
    */
    //> Closures call-frame-closure
    ObjClosure* closure;
    //< Closures call-frame-closure
    uint8_t* ip;
    Value* slots;
};
//< Calls and Functions call-frame

class Thread {
public:
    //> Calls and Functions frame-array
    CallFrame frames[FRAMES_MAX];
    int frameCount;

    //< Calls and Functions frame-array
    //> vm-stack
    Value stack[STACK_MAX];
    Value* stackTop;
    //< vm-stack
    Thread* next;
};
