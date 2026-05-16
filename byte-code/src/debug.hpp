//> Chunks of Bytecode debug-h
#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.hpp"
#include "dictionary.hpp"

#include <string>

namespace sce {
class Debug {
private:
    Dictionary* dct;
    int constantInstruction(const char* name, Chunk* chunk, int offset);
    int constantInstructionDictionary(const char* name, Chunk* chunk, int offset);
    int invokeInstruction(const char* name, Chunk* chunk, int offset);
    int invokeInstructionDictionary(const char* name, Chunk* chunk, int offset);
    int simpleInstruction(const char* name, int offset);
    int byteInstruction(const char* name, Chunk* chunk, int offset);
    int shortInstruction(const char* name, Chunk* chunk, int offset);
    int jumpInstruction(const char* name, int sign, Chunk* chunk, int offset);

public:
    void setDictionary(Dictionary* dictionary);
    void disassembleChunk(Chunk* chunk, std::string name);
    int disassembleInstruction(Chunk* chunk, int offset);
};
} // namespace sce

#endif
