#include "object.hpp"

#include "dictionary.hpp"

namespace sce {
NativeClass::NativeClass(ObjectFactory* factory, std::unordered_map<std::string_view, NativeClassBoundFn>& apis,
                         std::unordered_map<std::string_view, NativeClassBoundProperty>& constants)
    : m_apis(apis), m_constants(constants)
{
    uint16_t address;
    for (std::unordered_map<std::string_view, NativeClassBoundFn>::iterator it = apis.begin(); it != apis.end(); it++) {
        if (factory->identify((const char*)it->first.data(), it->first.size(), &address)) {
            size_t sz = m_direct_apis.size();
            if (sz <= address) {
                m_direct_apis.resize(address + 1);
                memset(m_direct_apis.m_buffer + sz, 0, sizeof(NativeClassBoundFn) * (address + 1 - sz));
            }
            m_direct_apis[address] = it->second;
        }
    }
    for (std::unordered_map<std::string_view, NativeClassBoundProperty>::iterator it = constants.begin();
         it != constants.end(); it++) {
        if (factory->identify((const char*)it->first.data(), it->first.size(), &address)) {
            size_t sz = m_direct_constants.size();
            if (sz <= address) {
                m_direct_constants.resize(address + 1);
                memset(m_direct_constants.m_buffer + sz, 0, sizeof(NativeClassBoundFn) * (address + 1 - sz));
            }
            m_direct_constants[address] = it->second;
        }
    }
}

NativeClass::~NativeClass()
{
}

Value NativeClass::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    std::unordered_map<std::string_view, NativeClassBoundFn>::iterator it = m_apis.find(name);
    if (it == m_apis.end())
        throw std::runtime_error("invalid method");
    return it->second(factory, this, argc, argv);
}

Value NativeClass::invoke(ObjectFactory* factory, uint16_t name, int argc, Value* argv)
{
    if (m_direct_apis.size() <= name)
        throw std::runtime_error("invalid method");
    NativeClassBoundFn f = m_direct_apis[name];
    if (NULL == f)
        throw std::runtime_error("invalid method");
    return f(factory, this, argc, argv);
}

Value NativeClass::call(ObjectFactory* factory, int argc, Value* argv)
{
    (void)factory;
    (void)argc;
    (void)argv;
    throw std::runtime_error("invalid call");
    return NIL_VAL;
}

Value NativeClass::constant(ObjectFactory* factory, std::string name)
{
    std::unordered_map<std::string_view, NativeClassBoundProperty>::iterator it = m_constants.find(name);
    if (it == m_constants.end())
        throw std::runtime_error("invalid property");
    return it->second(factory, this);
}

Value NativeClass::constant(ObjectFactory* factory, uint16_t name)
{
    if (m_direct_constants.size() <= name)
        throw std::runtime_error("invalid property");
    NativeClassBoundProperty p = m_direct_constants[name];
    if (NULL == p)
        throw std::runtime_error("invalid property");
    return p(factory, this);
}

ObjNativeClass::ObjNativeClass() : Obj(OBJ_NATIVE_CLASS), klass(NULL)
{
}

ObjNativeClass::~ObjNativeClass()
{
    if (klass)
        delete klass;
}

std::string ObjNativeClass::stringify(void)
{
    return "<native obj>";
}

void ObjNativeClass::blaken(void)
{
}
} // namespace sce
