#include "object.hpp"

namespace sce {
ObjMap::ObjMap() : Obj(OBJ_MAP), container(NIL_VAL)
{
}

ObjMap::~ObjMap()
{
}

std::string ObjMap::stringify(void)
{
    bool is_first = true;
    std::stringstream ss;
    ss << "{";
    container.iterate([&](nsl::string first, Value second) {
        if (!is_first) {
            ss << ",";
        }
        else {
            is_first = false;
        }
        ss << "\"" << first.c_str() << "\":";
        Value v = second;
        if (IS_NIL(v)) {
            ss << "nil";
        }
        else if (IS_BOOL(v)) {
            ss << (AS_BOOL(v) ? "true" : "false");
        }
        else if (IS_NUMBER(v)) {
            ss << AS_NUMBER(v);
        }
        else if (IS_OBJ(v)) {
            bool isString = IS_STRING(v);
            if (isString)
                ss << "\"";
            ss << AS_OBJ(v)->stringify();
            if (isString)
                ss << "\"";
        }
    });
    ss << "}";
    return ss.str();
}

void ObjMap::blaken(void)
{
}

Value ObjMap::get(nsl::string index)
{
    Value v;
    if (!container.get(index, &v)) {
        throw std::runtime_error("Index is invalid.");
    }
    return v;
}

void ObjMap::set(nsl::string index, Value value)
{
    container.set(index, value);
}
} // namespace sce
