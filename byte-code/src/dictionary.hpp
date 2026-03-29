#pragma once

#include <cstddef>
#include <cstdint>

typedef struct {
    const char* name;
    size_t length;
} DictWord;

namespace sce {
class Dictionary {
private:
    inline int compare(const char* name, size_t ln, const char* reference, size_t lr);

public:
    bool identify(const char* name, size_t length, uint16_t* address);
    const char* get(size_t index);
    size_t size(void);
};

int Dictionary::compare(const char* name, size_t ln, const char* reference, size_t lr)
{
    size_t el = (ln < lr) ? ln : lr;
    const char *a = name, *b = reference;
    size_t idx = 0;
    while (*a && *b && idx < el) {
        if (*a < *b)
            return -1;
        if (*a > *b)
            return 1;
        a++;
        b++;
        idx++;
    }
    if (ln < lr)
        return -1;
    if (ln > lr)
        return 1;
    return 0;
}

} // namespace sce
