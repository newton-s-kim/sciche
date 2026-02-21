//> Chunks of Bytecode chunk-c
#include <stdlib.h>

#include "chunk.hpp"

#include "object.hpp"

//< Strings allocate
#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)
//> grow-array

#define GROW_ARRAY(type, pointer, oldCount, newCount)                                                                  \
    (type*)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))
//< grow-array
//> free-array

#define FREE_ARRAY(type, pointer, oldCount) reallocate(pointer, sizeof(type) * (oldCount), 0)
//< free-array

namespace sce {
Chunk::Chunk() : count(0), capacity(0), lines(NULL), code(NULL)
{
}
//> free-chunk
Chunk::~Chunk()
{
    // GC::instance()->collect(capacity, 0);
    FREE_ARRAY(uint8_t, code, capacity);
    //> chunk-free-lines
    FREE_ARRAY(int, lines, capacity);
    //< chunk-free-lines
}
//< free-chunk
/* Chunks of Bytecode write-chunk < Chunks of Bytecode write-chunk-with-line
void writeChunk(Chunk* chunk, uint8_t byte) {
*/
//> write-chunk
//> write-chunk-with-line
void Chunk::write(uint8_t byte, int line)
{
    //< write-chunk-with-line
    if (capacity < count + 1) {
        int oldCapacity = capacity;
        capacity = GROW_CAPACITY(oldCapacity);
        // GC::instance()->collect(oldCapacity, capacity);
        code = GROW_ARRAY(uint8_t, code, oldCapacity, capacity);
        //> write-chunk-line
        lines = GROW_ARRAY(int, lines, oldCapacity, capacity);
        //< write-chunk-line
    }

    code[count] = byte;
    //> chunk-write-line
    lines[count] = line;
    //< chunk-write-line
    count++;
}
//< write-chunk
//> add-constant
int Chunk::addConstant(Value value)
{
    if (IS_NUMBER(value)) {
        for (size_t index = 0; index < constants.size(); index++) {
            if (!IS_NUMBER(constants[index]))
                continue;
            if (AS_NUMBER(value) == AS_NUMBER(constants[index]))
                return index;
        }
    }
    else if (IS_STRING(value)) {
        for (size_t index = 0; index < constants.size(); index++) {
            if (!IS_STRING(constants[index]))
                continue;
            if (AS_STRING(value)->chars == AS_STRING(constants[index])->chars)
                return index;
        }
    }
    else if (IS_COMPLEX(value)) {
        for (size_t index = 0; index < constants.size(); index++) {
            if (!IS_COMPLEX(constants[index]))
                continue;
            if (AS_COMPLEX(value)->value == AS_COMPLEX(constants[index])->value)
                return index;
        }
    }
    constants.push_back(value);
    return constants.size() - 1;
}

int Chunk::size(void)
{
    return count;
}

void* Chunk::reallocate(void* pointer, size_t oldSize, size_t newSize)
{
    //< Garbage Collection call-collect
    if (oldSize > 0 && newSize == 0) {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, newSize);
    //> out-of-memory
    if (result == NULL)
        exit(1);
    //< out-of-memory
    return result;
}

//< add-constant
} // namespace sce
