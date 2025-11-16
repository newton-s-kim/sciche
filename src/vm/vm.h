#pragma once

#include <string>

namespace sciche {
class virtual_machine {
public:
    virtual_machine();
    ~virtual_machine();
    void run(void);
};
}; // namespace sciche
