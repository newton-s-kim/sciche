//> Strings object-h
#ifndef clox_object_h
#define clox_object_h

#include "common.hpp"
//> Calls and Functions object-include-chunk
#include "chunk.hpp"
//< Calls and Functions object-include-chunk
//> Classes and Instances object-include-table
#include "table.hpp"
//< Classes and Instances object-include-table
#include "value.hpp"
//> obj-type-macro

#include <vector>

#define OBJ_TYPE(value) (AS_OBJ(value)->type)
//< obj-type-macro
//> is-string

//> Methods and Initializers is-bound-method
#define IS_BOUND_METHOD(value) isObjType(value, OBJ_BOUND_METHOD)
//< Methods and Initializers is-bound-method
//> Classes and Instances is-class
#define IS_CLASS(value) isObjType(value, OBJ_CLASS)
//< Classes and Instances is-class
//> Closures is-closure
#define IS_CLOSURE(value) isObjType(value, OBJ_CLOSURE)
//< Closures is-closure
//> Calls and Functions is-function
#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
//< Calls and Functions is-function
//> Classes and Instances is-instance
#define IS_INSTANCE(value) isObjType(value, OBJ_INSTANCE)
//< Classes and Instances is-instance
//> Calls and Functions is-native
#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
//< Calls and Functions is-native
#define IS_STRING(value) isObjType(value, OBJ_STRING)
//< is-string
//> as-string

//> Methods and Initializers as-bound-method
#define AS_BOUND_METHOD(value) ((ObjBoundMethod*)AS_OBJ(value))
//< Methods and Initializers as-bound-method
//> Classes and Instances as-class
#define AS_CLASS(value) ((ObjClass*)AS_OBJ(value))
//< Classes and Instances as-class
//> Closures as-closure
#define AS_CLOSURE(value) ((ObjClosure*)AS_OBJ(value))
//< Closures as-closure
//> Calls and Functions as-function
#define AS_FUNCTION(value) ((ObjFunction*)AS_OBJ(value))
//< Calls and Functions as-function
//> Classes and Instances as-instance
#define AS_INSTANCE(value) ((ObjInstance*)AS_OBJ(value))
//< Classes and Instances as-instance
//> Calls and Functions as-native
#define AS_NATIVE(value) (((ObjNative*)AS_OBJ(value))->function)
//< Calls and Functions as-native
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)
//< as-string
//> obj-type

typedef enum {
    //> Methods and Initializers obj-type-bound-method
    OBJ_BOUND_METHOD,
    //< Methods and Initializers obj-type-bound-method
    //> Classes and Instances obj-type-class
    OBJ_CLASS,
    //< Classes and Instances obj-type-class
    //> Closures obj-type-closure
    OBJ_CLOSURE,
    //< Closures obj-type-closure
    //> Calls and Functions obj-type-function
    OBJ_FUNCTION,
    //< Calls and Functions obj-type-function
    //> Classes and Instances obj-type-instance
    OBJ_INSTANCE,
    //< Classes and Instances obj-type-instance
    //> Calls and Functions obj-type-native
    OBJ_NATIVE,
    //< Calls and Functions obj-type-native
    OBJ_STRING,
    //> Closures obj-type-upvalue
    OBJ_UPVALUE
    //< Closures obj-type-upvalue
} ObjType;
//< obj-type

class Obj {
public:
    ObjType type;
    //> Garbage Collection is-marked-field
    bool isMarked;
    //< Garbage Collection is-marked-field
    //> next-field
    struct Obj* next;
    //< next-field
    Obj(ObjType objType);
};
//> Calls and Functions obj-function

class ObjFunction : public Obj {
public:
    int arity;
    //> Closures upvalue-count
    int upvalueCount;
    //< Closures upvalue-count
    Chunk chunk;
    std::string name;
    ObjFunction();
    ~ObjFunction();
};
//< Calls and Functions obj-function
//> Calls and Functions obj-native

typedef Value (*NativeFn)(int argCount, Value* args);

class ObjNative : public Obj {
public:
    NativeFn function;
    ObjNative(NativeFn pFunction);
};
//< Calls and Functions obj-native
//> obj-string

class ObjString : public Obj {
public:
    std::string chars;
    ObjString(std::string pChars);
};
//< obj-string
//> Closures obj-upvalue
class ObjUpvalue : public Obj {
public:
    Value* location;
    //> closed-field
    Value closed;
    //< closed-field
    //> next-field
    struct ObjUpvalue* next;
    //< next-field
    ObjUpvalue(Value* slot);
};
//< Closures obj-upvalue
//> Closures obj-closure
class ObjClosure : public Obj {
public:
    ObjFunction* function;
    //> upvalue-fields
    std::vector<ObjUpvalue*> upvalues;
    int upvalueCount;
    //< upvalue-fields
    ObjClosure(ObjFunction* pFunction);
};
//< Closures obj-closure
//> Classes and Instances obj-class

class ObjClass : public Obj {
public:
    std::string name;
    //> Methods and Initializers class-methods
    Table methods;
    //< Methods and Initializers class-methods
    ObjClass(std::string name);
    ~ObjClass();
};
//< Classes and Instances obj-class
//> Classes and Instances obj-instance

class ObjInstance : public Obj {
public:
    ObjClass* klass;
    Table fields; // [fields]
    ObjInstance(ObjClass* klass);
    ~ObjInstance();
};
//< Classes and Instances obj-instance

//> Methods and Initializers obj-bound-method
class ObjBoundMethod : public Obj {
public:
    Value receiver;
    ObjClosure* method;
    ObjBoundMethod(Value pReceiver, ObjClosure* pMethod);
};

class ObjectUtil {
public:
    void printObject(Value value);
};

//< copy-string-h
//> is-obj-type
static inline bool isObjType(Value value, ObjType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

//< is-obj-type
#endif
