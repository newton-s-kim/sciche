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

static Value col_resize(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    (void)factory;
    if(1 != argc) throw std::runtime_error("invalid arguments.");
    if(!IS_NUMBER(argv[0])) throw std::runtime_error("number is expected.");
    ObjCol* col = (ObjCol*)obj;
    col->value.resize(AS_NUMBER(argv[0]));
    return NUMBER_VAL(0);
}

static Value col_zeros(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    (void)factory;
    (void)argc;
    (void)argv;
    ObjCol* col = (ObjCol*)obj;
    col->value.zeros();
    return NUMBER_VAL(0);
}

static Value col_transpose(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    (void)argc;
    (void)argv;
    ObjRow* row = factory->newRow();
    ObjCol* col = (ObjCol*)obj;
    row->value = col->value.t();
    return OBJ_VAL(row);
}

std::map<std::string, NativeBooundFn> s_col_apis = {
    {"size", col_size},
    {"resize", col_resize},
    {"zeros", col_zeros},
    {"t", col_transpose}
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
    case OBJ_COL:
        it = s_col_apis.find(name);
        if (it != s_list_apis.end())
            fn = it->second;
        break;
    default:
        break;
    }
    return fn;
}
