#include "json.hpp"

#include "log.hpp"

#include <iostream>
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
        // TODO: refactoring
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
        // TODO: refactoring
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
        // TODO: refactoring
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
        // TODO: refactoring
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
        // TODO: refactoring
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
        // TODO: refactoring
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
        // TODO: refactoring
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
        // TODO: refactoring
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
            // TODO: refactoring
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
            // TODO: refactoring
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

static Value json_load(ObjectFactory* factory, NativeClass* klass, int argc, Value* argv)
{
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_STRING(argv[0]))
        throw std::runtime_error("string is expected.");
    JsonInterface* ji = (JsonInterface*)klass;
    return ji->load(AS_STRING(argv[0])->chars, factory);
}

static Value json_dump(ObjectFactory* factory, NativeClass* klass, int argc, Value* argv)
{
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments");
    if (!IS_MAP(argv[0]) && !IS_LIST(argv[0]))
        throw std::runtime_error("either map or list is expected.");
    rapidjson::StringBuffer s;
    JsonInterface* ji = (JsonInterface*)klass;
    ji->dump(s, argv[0]);
    ObjString* str = factory->newString(s.GetString());
    return OBJ_VAL(str);
}

// clang-format off
std::map<std::string, NativeClassBoundFn> s_json_apis = {
    {"load", json_load},
    {"dump", json_dump}
};

std::map<std::string, NativeClassBoundProperty> s_json_constatns;
// clang-format on

JsonInterface::JsonInterface() : NativeClass(s_json_apis, s_json_constatns)
{
}

JsonInterface::~JsonInterface()
{
}

void JsonInterface::write(rapidjson::Writer<rapidjson::StringBuffer>& writer, Value value)
{
    if (IS_MAP(value)) {
        ObjMap* map = AS_MAP(value);
        writer.StartObject();
        for (std::map<std::string, Value>::iterator it = map->container.begin(); it != map->container.end(); it++) {
            writer.Key(it->first.c_str());
            write(writer, it->second);
        }
        writer.EndObject();
    }
    else if (IS_LIST(value)) {
        ObjList* list = AS_LIST(value);
        writer.StartArray();
        for (std::vector<Value>::iterator it = list->container.begin(); it != list->container.end(); it++) {
            write(writer, *it);
        }
        writer.EndArray();
    }
    else if (IS_NUMBER(value)) {
        writer.Double(AS_NUMBER(value));
    }
    else if (IS_STRING(value)) {
        writer.String(AS_STRING(value)->chars.c_str());
    }
    else if (IS_BOOL(value)) {
        writer.Bool(AS_BOOL(value));
    }
}

Value JsonInterface::load(std::string str, ObjectFactory* factory)
{
    MyHandler handler;
    handler.factory = factory;
    rapidjson::Reader reader;
    rapidjson::StringStream ss(str.c_str());
    reader.Parse(ss, handler);
    return OBJ_VAL(handler.final_obj);
}

void JsonInterface::dump(rapidjson::StringBuffer& s, Value value)
{
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    write(writer, value);
}
