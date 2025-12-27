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
// clang-format on

static Value map_size(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    (void)factory;
    (void)argc;
    (void)argv;
    ObjMap* map = (ObjMap*)obj;
    return NUMBER_VAL(map->container.size());
}

// clang-format off
std::map<std::string, NativeBooundFn> s_map_apis = {
    {"size", map_size}
};
// clang-format on

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
    if (1 != argc)
        throw std::runtime_error("invalid arguments.");
    if (!IS_NUMBER(argv[0]))
        throw std::runtime_error("number is expected.");
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

static Value col_randn(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    (void)factory;
    (void)argc;
    (void)argv;
    ObjCol* col = (ObjCol*)obj;
    col->value.randn();
    return NUMBER_VAL(0);
}

static Value col_add(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    (void)factory;
    ObjCol* col = (ObjCol*)obj;
    size_t sz = col->value.n_elem;
    col->value.resize(sz + argc);
    for (int i = 0; i < argc; i++) {
        if (!IS_NUMBER(argv[i]))
            throw std::runtime_error("number is expected.");
        col->value[sz + i] = AS_NUMBER(argv[i]);
    }
    return NUMBER_VAL(0);
}

// clang-format off
std::map<std::string, NativeBooundFn> s_col_apis = {
    {"size", col_size},
    {"resize", col_resize},
    {"zeros", col_zeros},
    {"add", col_add},
    {"t", col_transpose},
    {"randn", col_randn}
};
// clang-format on

std::map<std::string, NativeBooundFn> s_row_apis = {};

static Value mat_col(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    (void)factory;
    Value value = 0;
    ObjMat* mat = (ObjMat*)obj;
    if (2 == argc) {
        if (!IS_NUMBER(argv[0]))
            throw std::runtime_error("number is expected.");
        if (IS_COL(argv[1])) {
            mat->value.col(AS_NUMBER(argv[0])) = AS_COL(argv[1])->value;
        }
        else if (IS_MAT(argv[1])) {
            mat->value.col(AS_NUMBER(argv[0])) = AS_MAT(argv[1])->value;
        }
        else {
            throw std::runtime_error("vec is expected.");
        }
    }
    else if (1 == argc) {
        if (!IS_NUMBER(argv[0]))
            throw std::runtime_error("number is expected.");
        ObjCol* col = factory->newCol();
        col->value = mat->value.col(AS_NUMBER(argv[0]));
        value = OBJ_VAL(col);
    }
    else {
        throw std::runtime_error("invalid number of arguments");
    }
    return value;
}

static Value mat_row(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    (void)factory;
    Value value = 0;
    ObjMat* mat = (ObjMat*)obj;
    if (2 == argc) {
        if (!IS_NUMBER(argv[0]))
            throw std::runtime_error("number is expected.");
        if (!IS_ROW(argv[1]))
            throw std::runtime_error("rowvec is expected.");
        mat->value.row(AS_NUMBER(argv[0])) = AS_ROW(argv[1])->value;
    }
    else if (1 == argc) {
        if (!IS_NUMBER(argv[0]))
            throw std::runtime_error("number is expected.");
        ObjRow* row = factory->newRow();
        row->value = mat->value.row(AS_NUMBER(argv[0]));
        value = OBJ_VAL(row);
    }
    else {
        throw std::runtime_error("invalid number of arguments");
    }
    return value;
}

static Value mat_set(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    (void)factory;
    Value value = 0;
    ObjMat* mat = (ObjMat*)obj;
    size_t last_cols = 0, rows = 0, cols = 0;
    for (int i = 0; i < argc; i++) {
        if (IS_NIL(argv[i])) {
            cols = 0;
            if (0 == rows) {
                last_cols = cols;
            }
            else if (last_cols != cols) {
                throw std::runtime_error("rows are not the same");
            }
            rows++;
        }
        else if (!IS_NUMBER(argv[i]))
            throw std::runtime_error("number is expected");
        else
            cols++;
    }
    rows++;
    if (0 == rows || 0 == cols)
        throw std::runtime_error("invalid dimension");
    mat->value.resize(rows, cols);
    size_t idx = 0;
    for (size_t rcnt = 0; rcnt < rows; rcnt++) {
        for (size_t ccnt = 0; ccnt < cols; ccnt++) {
            if (IS_NIL(argv[idx]))
                break;
            mat->value(rcnt, ccnt) = AS_NUMBER(argv[idx]);
        }
    }
    return value;
}

static Value mat_transpose(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    if (0 != argc)
        throw std::runtime_error("invalid number of arguments");
    (void)argv;
    ObjMat* mat = (ObjMat*)obj;
    ObjMat* tmat = factory->newMat();
    tmat->value = mat->value.t();
    return OBJ_VAL(tmat);
}

// clang-format off
std::map<std::string, NativeBooundFn> s_mat_apis = {
    {"col", mat_col},
    {"row", mat_row},
    {"set", mat_set},
    {"t", mat_transpose}
};
// clang-format on

static Value cube_slice(ObjectFactory* factory, Obj* obj, int argc, Value* argv)
{
    (void)factory;
    Value value = 0;
    ObjCube* cube = (ObjCube*)obj;
    if (2 == argc) {
        if (!IS_NUMBER(argv[0]))
            throw std::runtime_error("number is expected.");
        if (!IS_MAT(argv[1]))
            throw std::runtime_error("rowvec is expected.");
        cube->value.slice(AS_NUMBER(argv[0])) = AS_MAT(argv[1])->value;
    }
    else if (1 == argc) {
        if (!IS_NUMBER(argv[0]))
            throw std::runtime_error("number is expected.");
        ObjMat* mat = factory->newMat();
        mat->value = cube->value.slice(AS_NUMBER(argv[0]));
        value = OBJ_VAL(mat);
    }
    else {
        throw std::runtime_error("invalid number of arguments");
    }
    return value;
}

// clang-format off
std::map<std::string, NativeBooundFn> s_cube_apis = {
    {"slice", cube_slice}
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
    case OBJ_MAP:
        it = s_map_apis.find(name);
        if (it != s_map_apis.end())
            fn = it->second;
        break;
    case OBJ_COL:
        it = s_col_apis.find(name);
        if (it != s_col_apis.end())
            fn = it->second;
        break;
    case OBJ_MAT:
        it = s_mat_apis.find(name);
        if (it != s_mat_apis.end())
            fn = it->second;
        break;
    case OBJ_CUBE:
        it = s_cube_apis.find(name);
        if (it != s_cube_apis.end())
            fn = it->second;
        break;
        break;
    default:
        break;
    }
    return fn;
}
