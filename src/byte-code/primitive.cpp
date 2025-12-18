#include "primitive.hpp"

#include <map>
#include <string>

static Value list_size(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    (void)factory;
    (void)argc;
    (void)argv;
    ObjList* list = (ObjList*)obj;
    return NUMBER_VAL(list->container.size());
}

static Value list_add(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    (void)factory;
    ObjList* list = (ObjList*)obj;
    for (int idx = 0; idx < argc; idx++) {
        list->container.push_back(argv[idx]);
    }
    return NUMBER_VAL(argc);
}

// clang-format off
std::map<std::string, NativeBooundFn> s_list_apis = {
    {"size", list_size},
    {"add", list_add}
};

static Value col_size(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    (void)factory;
    (void)argc;
    (void)argv;
    ObjCol* col = (ObjCol*)obj;
    double n = col->value.size();
    return NUMBER_VAL(n);
}

std::map<std::string, NativeBooundFn> s_col_apis = {
    {"size", col_size},
};

std::map<std::string, NativeBooundFn> s_row_apis = {
};

std::map<std::string, NativeBooundFn> s_mat_apis = {
};
// clang-format on

NativeBooundFn Primitive::find(ObjType type, std::string name)
{
    NativeBooundFn fn = NULL;
    std::map<std::string, NativeBooundFn>::iterator it;
    switch (type) {
    case OBJ_LIST:
        it = s_list_apis.find(name);
        if (it != s_list_apis.end())
            fn = it->second;
        break;
    default:
        break;
    }
    return fn;
}
