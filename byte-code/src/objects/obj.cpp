#include "object.hpp"

Obj::Obj(ObjType objType) : type(objType), isMarked(false)
{
}

Obj::~Obj()
{
}

void Obj::print(void)
{
    printf("%s", stringify().c_str());
}

Value Obj::add(Value v, ObjectFactory* factory, bool opposite)
{
    (void)v;
    (void)factory;
    (void)opposite;
    throw std::runtime_error("unsupported operation +");
}

Value Obj::sub(Value v, ObjectFactory* factory, bool opposite)
{
    (void)v;
    (void)factory;
    (void)opposite;
    throw std::runtime_error("unsupported operation -");
}

Value Obj::mul(Value v, ObjectFactory* factory, bool opposite)
{
    (void)v;
    (void)factory;
    (void)opposite;
    throw std::runtime_error("unsupported operation *");
}

Value Obj::div(Value v, ObjectFactory* factory, bool opposite)
{
    (void)v;
    (void)factory;
    (void)opposite;
    throw std::runtime_error("unsupported operation /");
}

Value Obj::mod(Value v, ObjectFactory* factory, bool opposite)
{
    (void)v;
    (void)factory;
    (void)opposite;
    throw std::runtime_error("unsupported operation %");
}

Value Obj::pow(Value v, ObjectFactory* factory, bool opposite)
{
    (void)v;
    (void)factory;
    (void)opposite;
    throw std::runtime_error("unsupported operation ^");
}
