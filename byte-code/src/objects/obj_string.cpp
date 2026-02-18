#include "object.hpp"

ObjString::ObjString(std::string pChars) : Obj(OBJ_STRING), chars(pChars)
{
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)this, sizeof(ObjString), type);
#endif
}

std::string ObjString::stringify(void)
{
    return chars;
}

void ObjString::blaken(void)
{
}

Value ObjString::add(Value v, ObjectFactory* factory)
{
    Value r = NIL_VAL;
    if (IS_STRING(v)) {
        ObjString* b = AS_STRING(v);
        std::string rchars = chars;
        rchars += b->chars;
        ObjString* result = factory->newString(rchars.c_str());
        r = OBJ_VAL(result);
    }
    else if (IS_NUMBER(v)) {
        double b = AS_NUMBER(v);
        std::stringstream ss;
        ss << chars << b;
        ObjString* c = factory->newString(ss.str().c_str());
        r = OBJ_VAL(c);
    }
    else if (IS_COMPLEX(v)) {
        ObjComplex* b = AS_COMPLEX(v);
        std::string s = chars + b->stringify();
        ObjString* c = factory->newString(s.c_str());
        r = OBJ_VAL(c);
    }
    else {
        throw std::runtime_error("unsupported operator +");
    }
    return r;
}
