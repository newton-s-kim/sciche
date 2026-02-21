#pragma once

#include "object.hpp"

#include <string>

namespace sce {
typedef void (*libfn)(std::vector<std::string>&, std::vector<NativeFn>&);
typedef void (*libsym)(std::vector<std::string>&, std::vector<NativeClass*>&);
typedef void (*libconst)(std::vector<std::string>&, std::vector<double>&);

class dl {
private:
    void* m_handle;
    libfn m_fn;
    libsym m_sym;
    libconst m_const;
    std::string m_fn_name;
    std::string m_sym_name;
    std::string m_const_name;
    std::string m_name;

public:
    dl(std::string path, std::string name);
    ~dl();
    void functions(std::vector<std::string>& names, std::vector<NativeFn>& functions);
    void symbols(std::vector<std::string>& names, std::vector<NativeClass*>& klasses);
    void constants(std::vector<std::string>& names, std::vector<double>& consts);
};
} // namespace sce
