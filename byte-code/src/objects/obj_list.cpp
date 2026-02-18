#include "object.hpp"

ObjList::ObjList() : Obj(OBJ_LIST)
{
}

ObjList::~ObjList()
{
}

std::string ObjList::stringify(void)
{
    std::stringstream ss;
    ss << "[";
    for (std::vector<Value>::iterator it = container.begin(); it != container.end(); it++) {
        if (it != container.begin())
            ss << ",";
        Value v = *it;
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
            if (IS_STRING(v))
                ss << "\"";
            ss << AS_OBJ(v)->stringify();
            if (IS_STRING(v))
                ss << "\"";
        }
    }
    ss << "]";
    return ss.str();
}

void ObjList::blaken(void)
{
}

Value ObjList::get(int index)
{
    if (index >= 0) {
        if (index >= (int)container.size()) {
            throw std::runtime_error("Index is invalid.");
        }
    }
    else {
        if (-index > (int)container.size()) {
            throw std::runtime_error("Index is invalid.");
        }
        index = (int)container.size() + index;
    }
    LAX_LOG("list[%d] is peeked.", index);
    return container[index];
}

void ObjList::set(int index, Value v)
{
    if (index >= 0) {
        if (index >= (int)container.size()) {
            throw std::runtime_error("Index is invalid.");
        }
    }
    else {
        if (-index > (int)container.size()) {
            throw std::runtime_error("Index is invalid.");
        }
        index = container.size() + index;
    }
    container[index] = v;
}

static Value list_class_filled(ObjectFactory* factory, NativeClass* klass, int argc, Value* argv)
{
    (void)klass;
    ObjList* list = factory->newList();
    if (2 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_NUMBER(argv[0]))
        throw std::runtime_error("Size must be a number.");
    double sz = AS_NUMBER(argv[0]);
    if (sz < 0)
        throw std::runtime_error("Size cannot be negative.");
    if (sz - (int)sz)
        throw std::runtime_error("Size must be an integer.");
    for (int idx = 0; idx < sz; idx++)
        list->container.push_back(argv[1]);
    return OBJ_VAL(list);
}

// clang-format off
std::map<std::string, NativeClassBoundFn> s_list_class_apis = {
    {"filled", list_class_filled},
};

std::map<std::string, NativeClassBoundProperty> s_list_class_constants = {
};
// clang-format on

listNative::listNative() : NativeClass(s_list_class_apis, s_list_class_constants)
{
}

Value listNative::call(ObjectFactory* factory, int argc, Value* args)
{
    ObjList* list = factory->newList();
    for (int idx = 0; idx < argc; idx++)
        list->container.push_back(args[idx]);
    return OBJ_VAL(list);
}
