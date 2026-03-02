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

namespace sce {
class GlobalMemoryInterface {
public:
    virtual void define(size_t address, Value value) = 0;
};

class CompilerInterface {
public:
    //> Calls and Functions compile-h
    virtual ObjFunction* compile(const char* source) = 0;
    //< Calls and Functions compile-h
    //> Garbage Collection mark-compiler-roots-h
    virtual void markCompilerRoots(std::function<void(Obj*)> callback) = 0;
    //< Garbage Collection mark-compiler-roots-h
    virtual uint16_t newGlobalAddress(std::string) = 0;
    virtual std::string undefinedSymbol(uint16_t addr) = 0;
};

class CompilerFactory {
public:
    virtual CompilerInterface* create(ObjectFactory* factory, GlobalMemoryInterface* global) = 0;
    static CompilerFactory* instance(void);
};
} // namespace sce
#endif
