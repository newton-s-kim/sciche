#include "object.hpp"

namespace sce {
NativeObject::NativeObject(std::map<std::string, NativeObjectBoundFn>& apis,
                           std::map<std::string, NativeObjectBoundProperty>& properties)
    : m_apis(apis), m_properties(properties)
{
}

NativeObject::~NativeObject()
{
}

Value NativeObject::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    std::map<std::string, NativeObjectBoundFn>::iterator it = m_apis.find(name);
    if (it == m_apis.end())
        throw std::runtime_error("invalid method");
    return it->second(factory, this, argc, argv);
}

Value NativeObject::property(ObjectFactory* factory, std::string name)
{
    std::map<std::string, NativeObjectBoundProperty>::iterator it = m_properties.find(name);
    if (it == m_properties.end())
        throw std::runtime_error("invalid property");
    return it->second(factory, this);
}

ObjNativeObject::ObjNativeObject() : Obj(OBJ_NATIVE_OBJ), object(NULL)
{
}

ObjNativeObject::~ObjNativeObject()
{
    if (object)
        delete object;
}

std::string ObjNativeObject::stringify(void)
{
    return "<native obj>";
}

void ObjNativeObject::blaken(void)
{
}
} // namespace sce
