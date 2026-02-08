#include "primitive.hpp"
#include "log.hpp"

#include <map>
#include <string>

typedef Value (*PrimitiveBoundFn)(ObjectFactory* factory, Value value, int argc, Value* argv);
typedef Value (*PrimitiveBoundProperty)(ObjectFactory* factory, Value value);

static Value list_size(ObjectFactory* factory, Value value)
{
    (void)factory;
    if (!IS_LIST(value))
        throw std::runtime_error("list is expected.");
    ObjList* list = AS_LIST(value);
    return NUMBER_VAL(list->container.size());
}

static Value list_add(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)factory;
    if (!IS_LIST(value))
        throw std::runtime_error("list is expected.");
    ObjList* list = AS_LIST(value);
    for (int idx = 0; idx < argc; idx++) {
        list->container.push_back(argv[idx]);
    }
    return NUMBER_VAL(argc);
}
static Value list_each(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    if (!IS_LIST(value))
        throw std::runtime_error("list is expected.");
    ObjList* list = AS_LIST(value);
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_CLOSURE(argv[0]))
        throw std::runtime_error("closure is expected.");
    for (size_t idx = 0; idx < list->container.size(); idx++) {
        factory->callFunction(argv[0], 1, &list->container[idx], true);
    }
    return NIL_VAL;
}
static Value list_clear(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)factory;
    (void)argv;
    if (!IS_LIST(value))
        throw std::runtime_error("list is expected.");
    ObjList* list = AS_LIST(value);
    if (0 != argc)
        throw std::runtime_error("invalid number of arguments.");
    list->container.clear();
    return NIL_VAL;
}
static Value list_indexof(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    ValueUtil util;
    int ret = -1;
    (void)factory;
    if (!IS_LIST(value))
        throw std::runtime_error("list is expected.");
    ObjList* list = AS_LIST(value);
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments.");
    for (size_t idx = 0; idx < list->container.size(); idx++) {
        if (util.equal(list->container[idx], argv[0])) {
            ret = idx;
            break;
        }
    }
    return NUMBER_VAL(ret);
}
static Value list_contains(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    int idx = AS_NUMBER(list_indexof(factory, value, argc, argv));
    bool ret = (idx < 0) ? false : true;
    return BOOL_VAL(ret);
}
static Value list_insert(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)factory;
    if (!IS_LIST(value))
        throw std::runtime_error("list is expected.");
    ObjList* list = AS_LIST(value);
    if (2 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_NUMBER(argv[0]))
        throw std::runtime_error("Index must be a number.");
    double idx = AS_NUMBER(argv[0]);
    if (idx < 0)
        idx += list->container.size() - 1;
    if (idx < 0)
        throw std::runtime_error("Index out of bounds.");
    if (0 != idx - (int)idx)
        throw std::runtime_error("Index must be an integer.");
    if (idx > 0 && idx >= (int)list->container.size())
        throw std::runtime_error("Index out of bounds.");
    if (0 == idx) {
        if (0 == list->container.size()) {
            list->container.push_back(argv[1]);
        }
        else {
            list->container.insert(list->container.begin(), argv[1]);
        }
    }
    else if (idx + 1 == list->container.size()) {
        list->container.push_back(argv[1]);
    }
    else {
        list->container.insert(list->container.begin(), idx, argv[1]);
    }
    return argv[1];
}

// clang-format off
std::map<std::string, PrimitiveBoundFn> s_list_apis = {
    {"add", list_add},
    {"each", list_each},
    {"clear", list_clear},
    {"indexOf", list_indexof},
    {"contains", list_contains},
    {"insert", list_insert},
};

std::map<std::string, PrimitiveBoundProperty> s_list_props = {
    {"size", list_size},
};
// clang-format on

static Value map_remove(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)factory;
    if (!IS_MAP(value))
        throw std::runtime_error("map is expected.");
    ObjMap* map = AS_MAP(value);
    if (1 != argc)
        throw std::runtime_error("invalid number of arguments.");
    if (!IS_STRING(argv[0]))
        throw std::runtime_error("Key must be a string.");
    map->container.erase(AS_STRING(argv[0])->chars);
    return NIL_VAL;
}

static Value map_size(ObjectFactory* factory, Value value)
{
    (void)factory;
    if (!IS_MAP(value))
        throw std::runtime_error("map is expected.");
    ObjMap* map = AS_MAP(value);
    return NUMBER_VAL(map->container.size());
}

// clang-format off
std::map<std::string, PrimitiveBoundFn> s_map_apis = {
    {"remove", map_remove}
};

std::map<std::string, PrimitiveBoundProperty> s_map_props = {
    {"size", map_size}
};
// clang-format on

static Value col_size(ObjectFactory* factory, Value value)
{
    (void)factory;
    if (!IS_COL(value))
        throw std::runtime_error("vector is expected.");
    ObjCol* col = AS_COL(value);
    double n = col->value.size();
    return NUMBER_VAL(n);
}

static Value col_resize(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)factory;
    if (1 != argc)
        throw std::runtime_error("invalid arguments.");
    if (!IS_NUMBER(argv[0]))
        throw std::runtime_error("number is expected.");
    if (!IS_COL(value))
        throw std::runtime_error("vector is expected.");
    ObjCol* col = AS_COL(value);
    col->value.resize(AS_NUMBER(argv[0]));
    return NUMBER_VAL(0);
}

static Value col_zeros(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)factory;
    (void)argc;
    (void)argv;
    if (!IS_COL(value))
        throw std::runtime_error("vector is expected.");
    ObjCol* col = AS_COL(value);
    col->value.zeros();
    return NUMBER_VAL(0);
}

static Value col_transpose(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)argc;
    (void)argv;
    if (!IS_COL(value))
        throw std::runtime_error("vector is expected.");
    ObjCol* col = AS_COL(value);
    ObjRow* row = factory->newRow();
    row->value = col->value.t();
    return OBJ_VAL(row);
}

static Value col_randn(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)factory;
    (void)argc;
    (void)argv;
    if (!IS_COL(value))
        throw std::runtime_error("vector is expected.");
    ObjCol* col = AS_COL(value);
    col->value.randn();
    return NUMBER_VAL(0);
}

static Value col_add(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)factory;
    if (!IS_COL(value))
        throw std::runtime_error("vector is expected.");
    ObjCol* col = AS_COL(value);
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
std::map<std::string, PrimitiveBoundFn> s_col_apis = {
    {"resize", col_resize},
    {"zeros", col_zeros},
    {"add", col_add},
    {"t", col_transpose},
    {"randn", col_randn}
};
std::map<std::string, PrimitiveBoundProperty> s_col_props = {
    {"size", col_size}
};
// clang-format on

static Value row_transpose(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)argc;
    (void)argv;
    if (!IS_ROW(value))
        throw std::runtime_error("vector is expected.");
    ObjRow* row = AS_ROW(value);
    ObjCol* col = factory->newCol();
    col->value = row->value.t();
    return OBJ_VAL(col);
}

std::map<std::string, PrimitiveBoundFn> s_row_apis = {
    {"t", row_transpose},
};
std::map<std::string, PrimitiveBoundProperty> s_row_props = {};

static Value mat_col(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)factory;
    Value ret = 0;
    if (!IS_MAT(value))
        throw std::runtime_error("mat is expected");
    ObjMat* mat = AS_MAT(value);
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
        ret = OBJ_VAL(col);
    }
    else {
        throw std::runtime_error("invalid number of arguments");
    }
    return ret;
}

static Value mat_row(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)factory;
    Value ret = 0;
    if (!IS_MAT(value))
        throw std::runtime_error("mat is expected");
    ObjMat* mat = AS_MAT(value);
    if (2 == argc) {
        if (!IS_NUMBER(argv[0]))
            throw std::runtime_error("number is expected.");
        if (IS_ROW(argv[1])) {
            mat->value.row(AS_NUMBER(argv[0])) = AS_ROW(argv[1])->value;
        }
        else if (IS_MAT(argv[1])) {
            mat->value.row(AS_NUMBER(argv[0])) = AS_MAT(argv[1])->value;
        }
        else {
            throw std::runtime_error("rowvec is expected.");
        }
    }
    else if (1 == argc) {
        if (!IS_NUMBER(argv[0]))
            throw std::runtime_error("number is expected.");
        ObjRow* row = factory->newRow();
        row->value = mat->value.row(AS_NUMBER(argv[0]));
        ret = OBJ_VAL(row);
    }
    else {
        throw std::runtime_error("invalid number of arguments");
    }
    return ret;
}

static Value mat_rows(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)factory;
    Value ret = 0;
    if (!IS_MAT(value))
        throw std::runtime_error("mat is expected");
    ObjMat* mat = AS_MAT(value);
    if (2 == argc) {
        if (!IS_NUMBER(argv[0]))
            throw std::runtime_error("number is expected.");
        if (!IS_NUMBER(argv[1]))
            throw std::runtime_error("number is expected.");
        ObjMat* r = factory->newMat();
        r->value = mat->value.rows(AS_NUMBER(argv[0]), AS_NUMBER(argv[1]));
        ret = OBJ_VAL(r);
    }
    else {
        throw std::runtime_error("invalid number of arguments");
    }
    return ret;
}

static Value mat_set(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)factory;
    Value ret = 0;
    if (!IS_MAT(value))
        throw std::runtime_error("mat is expected");
    ObjMat* mat = AS_MAT(value);
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
    size_t rcnt = 0, ccnt = 0;
    for (int idx = 0; idx < argc; idx++) {
        if (IS_NIL(argv[idx])) {
            rcnt++;
            ccnt = 0;
        }
        else {
            mat->value(rcnt, ccnt) = AS_NUMBER(argv[idx]);
            ccnt++;
        }
    }
    return ret;
}

static Value mat_transpose(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    if (0 != argc)
        throw std::runtime_error("invalid number of arguments");
    (void)argv;
    if (!IS_MAT(value))
        throw std::runtime_error("mat is expected");
    ObjMat* mat = AS_MAT(value);
    ObjMat* tmat = factory->newMat();
    tmat->value = mat->value.t();
    return OBJ_VAL(tmat);
}

static Value mat_abs(ObjectFactory* factory, Value value)
{
    if (!IS_MAT(value))
        throw std::runtime_error("mat is expected");
    ObjMat* mat = AS_MAT(value);
    ObjMat* r = factory->newMat();
    r->value = abs(mat->value);
    return OBJ_VAL(r);
}

// clang-format off
std::map<std::string, PrimitiveBoundFn> s_mat_apis = {
    {"col", mat_col},
    {"row", mat_row},
    {"rows", mat_rows},
    {"set", mat_set},
    {"t", mat_transpose}
};
std::map<std::string, PrimitiveBoundProperty> s_mat_props = {
    {"abs", mat_abs}
};
// clang-format on

static Value cube_slice(ObjectFactory* factory, Value value, int argc, Value* argv)
{
    (void)factory;
    Value ret = 0;

    if (!IS_CUBE(value))
        throw std::runtime_error("cube is expected");
    ObjCube* cube = AS_CUBE(value);
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
        ret = OBJ_VAL(mat);
    }
    else {
        throw std::runtime_error("invalid number of arguments");
    }
    return ret;
}

// clang-format off
std::map<std::string, PrimitiveBoundFn> s_cube_apis = {
    {"slice", cube_slice}
};
std::map<std::string, PrimitiveBoundProperty> s_cube_props = {
};
// clang-format on

static Value num_ceil(ObjectFactory* factory, Value value)
{
    (void)factory;
    if (!IS_NUMBER(value))
        throw std::runtime_error("Number is expected.");
    int v = ceil(AS_NUMBER(value));
    LAX_LOG("ceil(%f) is %d", AS_NUMBER(value), v);
    return NUMBER_VAL(v);
}

static Value num_floor(ObjectFactory* factory, Value value)
{
    (void)factory;
    if (!IS_NUMBER(value))
        throw std::runtime_error("Number is expected.");
    int v = floor(AS_NUMBER(value));
    LAX_LOG("floor(%f) is %d", AS_NUMBER(value), v);
    return NUMBER_VAL(v);
}

static Value num_round(ObjectFactory* factory, Value value)
{
    (void)factory;
    if (!IS_NUMBER(value))
        throw std::runtime_error("Number is expected.");
    int v = round(AS_NUMBER(value));
    LAX_LOG("round(%f) is %d", AS_NUMBER(value), v);
    return NUMBER_VAL(v);
}

static Value num_sign(ObjectFactory* factory, Value value)
{
    (void)factory;
    if (!IS_NUMBER(value))
        throw std::runtime_error("Number is expected.");
    double v = (AS_NUMBER(value));
    int sgn = 0;
    if (0 < v)
        sgn = 1;
    else if (0 > v)
        sgn = -1;
    LAX_LOG("sng(%f) is %d", AS_NUMBER(value), sgn);
    return NUMBER_VAL(sgn);
}

static Value num_abs(ObjectFactory* factory, Value value)
{
    (void)factory;
    if (!IS_NUMBER(value))
        throw std::runtime_error("Number is expected.");
    double v = fabs(AS_NUMBER(value));
    LAX_LOG("abs(%f) is %f", AS_NUMBER(value), v);
    return NUMBER_VAL(v);
}

static Value num_fraction(ObjectFactory* factory, Value value)
{
    (void)factory;
    if (!IS_NUMBER(value))
        throw std::runtime_error("Number is expected.");
    double v = AS_NUMBER(value);
    double r = v - (int)v;
    LAX_LOG("floor(%f) is %f", v, r);
    return NUMBER_VAL(r);
}

static Value num_truncate(ObjectFactory* factory, Value value)
{
    (void)factory;
    if (!IS_NUMBER(value))
        throw std::runtime_error("Number is expected.");
    long v = trunc(AS_NUMBER(value));
    LAX_LOG("truncate(%f) is %d", AS_NUMBER(value), v);
    return NUMBER_VAL(v);
}

// clang-format off
std::map<std::string, PrimitiveBoundFn> s_number_apis = {
};

std::map<std::string, PrimitiveBoundProperty> s_number_props = {
    {"ceil", num_ceil},
    {"floor", num_floor},
    {"round", num_round},
    {"fraction", num_fraction},
    {"sign", num_sign},
    {"abs", num_abs},
    {"truncate", num_truncate}
};

std::map<std::string, PrimitiveBoundFn> s_bool_apis = {
};
std::map<std::string, PrimitiveBoundProperty> s_bool_props = {
};
// clang-format on

Value Primitive::call(ObjectFactory* factory, Value value, std::string& name, int argc, Value* argv)
{
    Value ret = 0;
    std::map<std::string, PrimitiveBoundFn>::iterator it;
    if (IS_NUMBER(value)) {
        it = s_number_apis.find(name);
        if (it != s_number_apis.end())
            ret = it->second(factory, value, argc, argv);
        else
            throw std::runtime_error("The method does not exist in number.");
    }
    else if (IS_BOOL(value)) {
        it = s_bool_apis.find(name);
        if (it != s_bool_apis.end())
            ret = it->second(factory, value, argc, argv);
        else
            throw std::runtime_error("The method does not exist in bool.");
    }
    else if (IS_LIST(value)) {
        it = s_list_apis.find(name);
        if (it != s_list_apis.end())
            ret = it->second(factory, value, argc, argv);
        else
            throw std::runtime_error("The method does not exist in List.");
    }
    else if (IS_MAP(value)) {
        it = s_map_apis.find(name);
        if (it != s_map_apis.end())
            ret = it->second(factory, value, argc, argv);
        else
            throw std::runtime_error("The method does not exist in Map.");
    }
    else if (IS_COL(value)) {
        it = s_col_apis.find(name);
        if (it != s_col_apis.end())
            ret = it->second(factory, value, argc, argv);
        else
            throw std::runtime_error("The method does not exist in vec.");
    }
    else if (IS_ROW(value)) {
        it = s_row_apis.find(name);
        if (it != s_row_apis.end())
            ret = it->second(factory, value, argc, argv);
        else
            throw std::runtime_error("The method does not exist in rowvec.");
    }
    else if (IS_MAT(value)) {
        it = s_mat_apis.find(name);
        if (it != s_mat_apis.end())
            ret = it->second(factory, value, argc, argv);
        else
            throw std::runtime_error("The method does not exist in mat.");
    }
    else if (IS_CUBE(value)) {
        it = s_cube_apis.find(name);
        if (it != s_mat_apis.end())
            ret = it->second(factory, value, argc, argv);
        else
            throw std::runtime_error("The method does not exist in cube.");
    }
    else {
        throw std::runtime_error("Only instances have methods.");
    }
    return ret;
}

Value Primitive::property(ObjectFactory* factory, Value value, std::string& name)
{
    (void)factory;
    Value ret = 0;
    std::map<std::string, PrimitiveBoundProperty>::iterator it;
    if (IS_NUMBER(value)) {
        it = s_number_props.find(name);
        if (it != s_number_props.end())
            ret = it->second(factory, value);
        else
            throw std::runtime_error("The property does not exist in number.");
    }
    else if (IS_BOOL(value)) {
        it = s_bool_props.find(name);
        if (it != s_bool_props.end())
            ret = it->second(factory, value);
        else
            throw std::runtime_error("The property does not exist in bool.");
    }
    else if (IS_LIST(value)) {
        it = s_list_props.find(name);
        if (it != s_list_props.end())
            ret = it->second(factory, value);
        else
            throw std::runtime_error("The property does not exist in List.");
    }
    else if (IS_MAP(value)) {
        it = s_map_props.find(name);
        if (it != s_map_props.end())
            ret = it->second(factory, value);
        else
            throw std::runtime_error("The property does not exist in Map.");
    }
    else if (IS_COL(value)) {
        it = s_col_props.find(name);
        if (it != s_col_props.end())
            ret = it->second(factory, value);
        else
            throw std::runtime_error("The property does not exist in vec.");
    }
    else if (IS_ROW(value)) {
        it = s_row_props.find(name);
        if (it != s_row_props.end())
            ret = it->second(factory, value);
        else
            throw std::runtime_error("The property does not exist in rowvec.");
    }
    else if (IS_MAT(value)) {
        it = s_mat_props.find(name);
        if (it != s_mat_props.end())
            ret = it->second(factory, value);
        else
            throw std::runtime_error("The property does not exist in mat.");
    }
    else if (IS_CUBE(value)) {
        it = s_cube_props.find(name);
        if (it != s_mat_props.end())
            ret = it->second(factory, value);
        else
            throw std::runtime_error("The property does not exist in cube.");
    }
    else {
        throw std::runtime_error("Only instances have properties.");
    }
    return ret;
}
