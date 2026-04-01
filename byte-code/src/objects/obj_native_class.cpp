#include "object.hpp"

namespace sce {
NativeClass::NativeClass(std::unordered_map<std::string_view, NativeClassBoundFn>& apis,
                         std::unordered_map<std::string_view, NativeClassBoundProperty>& constants)
    : m_apis(apis), m_constants(constants)
{
    memset(m_direct_apis, 0, sizeof(NativeClassBoundFn) * MEMBER_DICTIONARY_SIZE);
    memset(m_direct_constants, 0, sizeof(NativeClassBoundFn) * MEMBER_DICTIONARY_SIZE);
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
