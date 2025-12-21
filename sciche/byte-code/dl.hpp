#pragma once

#include "object.hpp"

#include <string>

typedef void (*libfn)(std::vector<std::string>&, std::vector<NativeFn>&);

class dl {
private:
    void* m_handle;
    libfn m_fn;
    std::string m_fn_name;
    std::string m_name;

public:
    dl(std::string path, std::string name);
    ~dl();
    void functions(std::vector<std::string>& names, std::vector<NativeFn>& functions);
};
