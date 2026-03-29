#include "dictionary.hpp"

#include "common.hpp"

#include "log.hpp"

namespace sce {

typedef struct {
    const char* name;
    size_t length;
} DictWord;

DictWord s_member_dictionary[] = {
#include "members.dct"
};

const char* Dictionary::get(size_t index)
{
    return s_member_dictionary[index].name;
}

bool Dictionary::identify(const char* name, size_t length, uint16_t* address)
{
    // LAX_LOG("size=%ld", MEMBER_DICITONARY_SIZE);
    int low = 0;
    int high = MEMBER_DICITONARY_SIZE - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        // LAX_LOG("mid=%d", mid);
        DictWord* dct = s_member_dictionary + mid;
        int cmp = compare(name, length, dct->name, dct->length);
        if (0 == cmp) {
            LAX_LOG("%s is found at %d", dct->name, mid);
            *address = (uint16_t)mid;
            return true;
        }
        if (0 < cmp)
            high = mid - 1;
        if (0 > cmp)
            low = mid + 1;
    }
    return false;
}

} // namespace sce
