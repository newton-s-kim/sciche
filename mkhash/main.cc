#include "map.hpp"
#include "string.hpp"
#include <cstdint>
#include <memory.h>

#include <cstdio>

int main(int argc, char* argv[])
{
    nsl::map<nsl::string, int> hash(0);
    for (size_t i = 1; i < argc; i++) {
        hash.set(argv[i], i);
    }
    for (size_t i = 0; i < hash.capacity(); i++) {
        nsl::string key = hash.key(i);
        int v = 0;
        hash.get(key, &v);
        printf("{\"%s\", %d},\n", key.c_str(), v);
    }
    return 0;
}
