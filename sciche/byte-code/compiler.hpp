//> Scanning on Demand compiler-h
#ifndef clox_compiler_h
#define clox_compiler_h

//> Strings compiler-include-object
#include "object.hpp"
//< Strings compiler-include-object
#include <complex>
#include <functional>
/* Scanning on Demand compiler-h < Compiling Expressions compile-h
void compile(const char* source);
*/
/* Compiling Expressions compile-h < Calls and Functions compile-h
bool compile(const char* source, Chunk* chunk);
*/

class CompilerInterface {
public:
    //> Calls and Functions compile-h
    virtual ObjFunction* compile(const char* source) = 0;
    //< Calls and Functions compile-h
    //> Garbage Collection mark-compiler-roots-h
    virtual void markCompilerRoots(std::function<void(Obj*)> callback) = 0;
    //< Garbage Collection mark-compiler-roots-h
};

class CompilerFactory {
public:
    virtual CompilerInterface* create(ObjectFactory* factory) = 0;
    static CompilerFactory* instance(void);
};

#endif
