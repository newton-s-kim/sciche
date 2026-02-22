#include "object.hpp"

namespace sce {
ObjMap::ObjMap() : Obj(OBJ_MAP)
{
}

ObjMap::~ObjMap()
{
}

std::string ObjMap::stringify(void)
{
    std::stringstream ss;
    ss << "{";
    for (std::unordered_map<std::string, Value>::iterator it = container.begin(); it != container.end(); it++) {
        if (it != container.begin())
            ss << ",";
        ss << "\"" << it->first << "\":";
        Value v = it->second;
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
    }
    ss << "}";
    return ss.str();
}

void ObjMap::blaken(void)
{
}

Value ObjMap::get(std::string index)
{
    std::unordered_map<std::string, Value>::iterator it = container.find(index);
    if (it == container.end()) {
        throw std::runtime_error("Index is invalid.");
    }
    return it->second;
}

void ObjMap::set(std::string index, Value value)
{
    container[index] = value;
}
} // namespace sce
