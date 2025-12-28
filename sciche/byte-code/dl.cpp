#include "dl.hpp"

#include <dlfcn.h>

dl::dl(std::string path, std::string name) : m_handle(NULL), m_fn(NULL), m_sym(NULL), m_name(name)
{
    m_handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!m_handle)
        throw std::runtime_error(dlerror());
    m_fn_name = m_name + "_functions";
    m_sym_name = m_name + "_symbols";
}

dl::~dl()
{
    if (m_handle)
        dlclose(m_handle);
}

void dl::functions(std::vector<std::string>& names, std::vector<NativeFn>& functions)
{
    if (NULL == m_fn) {
        m_fn = (libfn)dlsym(m_handle, m_fn_name.c_str());
        if (!m_fn)
            throw std::runtime_error(dlerror());
    }
    m_fn(names, functions);
}

void dl::symbols(std::vector<std::string>& names, std::vector<double>& symbols)
{
    if (NULL == m_sym) {
        m_sym = (libsym)dlsym(m_handle, m_sym_name.c_str());
        if (!m_sym)
            throw std::runtime_error(dlerror());
    }
    m_sym(names, symbols);
}
