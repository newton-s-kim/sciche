#pragma once

#include <cstddef>
#include <string>

namespace sciche {
class streamer {
public:
    virtual void fetch(void) = 0;
    virtual char getch(void) = 0;
    virtual void unch(void) = 0;
    virtual size_t pos(void) = 0;
    virtual void set_prompt(std::string str) = 0;
};
}; // namespace sciche
