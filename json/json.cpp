#include "json.hpp"

#include "log.hpp"

#include <iostream>
#include <rapidjson/reader.h>
#include <stack>
#include <stdexcept>

struct MyHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, MyHandler> {
    ObjectFactory* factory;
    std::string key;
    std::stack<Obj*> stack;
    Obj* final_obj = NULL;
    bool Null()
    {
        LAX_LOG("Null()");
        if (stack.empty())
            throw std::runtime_error("no container to add");
        Obj* obj = stack.top();
        if (OBJ_LIST == obj->type) {
            ObjList* list = (ObjList*)obj;
            list->container.push_back(NIL_VAL);
        }
        else if (OBJ_MAP == obj->type) {
            if (key.empty())
                throw std::runtime_error("key is not defined");
            ObjMap* map = (ObjMap*)obj;
            map->container[key] = NIL_VAL;
            key.clear();
        }
        return true;
    }
    bool Bool(bool b)
    {
        LAX_LOG("Bool(%s)", (b ? "true" : "false"));
        if (stack.empty())
            throw std::runtime_error("no container to add");
        Obj* obj = stack.top();
        if (OBJ_LIST == obj->type) {
            ObjList* list = (ObjList*)obj;
            list->container.push_back(BOOL_VAL(b));
        }
        else if (OBJ_MAP == obj->type) {
            if (key.empty())
                throw std::runtime_error("key is not defined");
            ObjMap* map = (ObjMap*)obj;
            map->container[key] = BOOL_VAL(b);
            key.clear();
        }
        return true;
    }
    bool Int(int i)
    {
        LAX_LOG("Int(%d)", i);
        if (stack.empty())
            throw std::runtime_error("no container to add");
        Obj* obj = stack.top();
        if (OBJ_LIST == obj->type) {
            ObjList* list = (ObjList*)obj;
            list->container.push_back(NUMBER_VAL(i));
        }
        else if (OBJ_MAP == obj->type) {
            if (key.empty())
                throw std::runtime_error("key is not defined");
            ObjMap* map = (ObjMap*)obj;
            map->container[key] = NUMBER_VAL(i);
            key.clear();
        }
        return true;
    }
    bool Uint(unsigned u)
    {
        LAX_LOG("Uint(%u)", u);
        if (stack.empty())
            throw std::runtime_error("no container to add");
        Obj* obj = stack.top();
        if (OBJ_LIST == obj->type) {
            ObjList* list = (ObjList*)obj;
            list->container.push_back(NUMBER_VAL(u));
        }
        else if (OBJ_MAP == obj->type) {
            if (key.empty())
                throw std::runtime_error("key is not defined");
            ObjMap* map = (ObjMap*)obj;
            map->container[key] = NUMBER_VAL(u);
            key.clear();
        }
        return true;
    }
    bool Int64(int64_t i)
    {
        LAX_LOG("Int64(%ld)", i);
        if (stack.empty())
            throw std::runtime_error("no container to add");
        Obj* obj = stack.top();
        if (OBJ_LIST == obj->type) {
            ObjList* list = (ObjList*)obj;
            list->container.push_back(NUMBER_VAL(i));
        }
        else if (OBJ_MAP == obj->type) {
            if (key.empty())
                throw std::runtime_error("key is not defined");
            ObjMap* map = (ObjMap*)obj;
            map->container[key] = NUMBER_VAL(i);
            key.clear();
        }
        return true;
    }
    bool Uint64(uint64_t u)
    {
        LAX_LOG("Uint64(%lu)", u);
        if (stack.empty())
            throw std::runtime_error("no container to add");
        Obj* obj = stack.top();
        if (OBJ_LIST == obj->type) {
            ObjList* list = (ObjList*)obj;
            list->container.push_back(NUMBER_VAL(u));
        }
        else if (OBJ_MAP == obj->type) {
            if (key.empty())
                throw std::runtime_error("key is not defined");
            ObjMap* map = (ObjMap*)obj;
            map->container[key] = NUMBER_VAL(u);
            key.clear();
        }
        return true;
    }
    bool Double(double d)
    {
        LAX_LOG("Double(%f)", d);
        if (stack.empty())
            throw std::runtime_error("no container to add");
        Obj* obj = stack.top();
        if (OBJ_LIST == obj->type) {
            ObjList* list = (ObjList*)obj;
            list->container.push_back(NUMBER_VAL(d));
        }
        else if (OBJ_MAP == obj->type) {
            if (key.empty())
                throw std::runtime_error("key is not defined");
            ObjMap* map = (ObjMap*)obj;
            map->container[key] = NUMBER_VAL(d);
            key.clear();
        }
        return true;
    }
    bool String(const char* str, rapidjson::SizeType length, bool copy)
    {
        (void)length;
        (void)copy;
        LAX_LOG("String(%s, %u, %s)", str, length, (copy ? "true" : "false"));
        if (stack.empty())
            throw std::runtime_error("no container to add");
        Obj* obj = stack.top();
        ObjString* s = factory->newString(str);
        if (OBJ_LIST == obj->type) {
            ObjList* list = (ObjList*)obj;
            list->container.push_back(OBJ_VAL(s));
        }
        else if (OBJ_MAP == obj->type) {
            if (key.empty())
                throw std::runtime_error("key is not defined");
            ObjMap* map = (ObjMap*)obj;
            map->container[key] = OBJ_VAL(s);
            key.clear();
        }
        return true;
    }
    bool StartObject()
    {
        LAX_LOG("StartObject()");
        ObjMap* map = factory->newMap();
        if (!stack.empty()) {
            Obj* obj = stack.top();
            if (OBJ_LIST == obj->type) {
                ObjList* list = (ObjList*)obj;
                list->container.push_back(OBJ_VAL(map));
            }
            else if (OBJ_MAP == obj->type) {
                if (key.empty())
                    throw std::runtime_error("key is not defined");
                ObjMap* map = (ObjMap*)obj;
                map->container[key] = OBJ_VAL(map);
                key.clear();
            }
        }
        stack.push(map);
        return true;
    }
    bool Key(const char* str, rapidjson::SizeType length, bool copy)
    {
        (void)length;
        (void)copy;
        LAX_LOG("Key(%s, %u, %s)", str, length, (copy ? "true" : "false"));
        if (stack.empty())
            throw std::runtime_error("no container to add");
        key = str;
        return true;
    }
    bool EndObject(rapidjson::SizeType memberCount)
    {
        (void)memberCount;
        LAX_LOG("EndObject(%u)", memberCount);
        final_obj = stack.top();
        stack.pop();
        return true;
    }
    bool StartArray()
    {
        LAX_LOG("StartArray()");
        ObjList* list = factory->newList();
        if (!stack.empty()) {
            Obj* obj = stack.top();
            if (OBJ_LIST == obj->type) {
                ObjList* list = (ObjList*)obj;
                list->container.push_back(OBJ_VAL(list));
            }
            else if (OBJ_MAP == obj->type) {
                if (key.empty())
                    throw std::runtime_error("key is not defined");
                ObjMap* map = (ObjMap*)obj;
                map->container[key] = OBJ_VAL(list);
                key.clear();
            }
        }
        stack.push(list);
        return true;
    }
    bool EndArray(rapidjson::SizeType elementCount)
    {
        (void)elementCount;
        LAX_LOG("EndArray(%u)", elementCount);
        final_obj = stack.top();
        stack.pop();
        return true;
    }
};

JsonInterface::JsonInterface()
{
}

JsonInterface::~JsonInterface()
{
}

Value JsonInterface::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    Value value = 0;
    (void)factory;
    (void)name;
    (void)argc;
    (void)argv;
    if ("load" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments");
        if (!IS_STRING(argv[0]))
            throw std::runtime_error("string is expected.");
        MyHandler handler;
        handler.factory = factory;
        rapidjson::Reader reader;
        rapidjson::StringStream ss(AS_STRING(argv[0])->chars.c_str());
        reader.Parse(ss, handler);
        return OBJ_VAL(handler.final_obj);
    }
    else if ("dump" == name) {
        if (1 != argc)
            throw std::runtime_error("invalid number of arguments");
    }
    else {
        throw std::runtime_error("invalid mothod");
    }
    return value;
}
