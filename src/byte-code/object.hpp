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

#include <complex>
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
#define IS_COMPLEX(value) isObjType(value, OBJ_COMPLEX)
//> as-string
#define IS_LIST(value) isObjType(value, OBJ_LIST)
#define IS_MAP(value) isObjType(value, OBJ_MAP)

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
#define AS_COMPLEX(value) ((ObjComplex*)AS_OBJ(value))
//> obj-type
#define AS_LIST(value) ((ObjList*)AS_OBJ(value))
#define AS_MAP(value) ((ObjMap*)AS_OBJ(value))

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
    OBJ_UPVALUE,
    //< Closures obj-type-upvalue
    OBJ_COMPLEX,
    OBJ_LIST,
    OBJ_MAP
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
    virtual ~Obj();
    virtual std::string stringify(void) = 0;
    virtual void blaken(void) = 0;
    void print(void);
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
    std::string stringify(void);
    void blaken(void);
};
//< Calls and Functions obj-function
//> Calls and Functions obj-native

class ObjectFactory;
typedef Value (*NativeFn)(ObjectFactory* factory, int argCount, Value* args);

class ObjNative : public Obj {
public:
    NativeFn function;
    ObjNative(NativeFn pFunction);
    std::string stringify(void);
    void blaken(void);
};
//< Calls and Functions obj-native
//> obj-string

class ObjString : public Obj {
public:
    std::string chars;
    ObjString(std::string pChars);
    std::string stringify(void);
    void blaken(void);
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
    std::string stringify(void);
    void blaken(void);
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
    std::string stringify(void);
    void blaken(void);
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
    std::string stringify(void);
    void blaken(void);
};
//< Classes and Instances obj-class
//> Classes and Instances obj-instance

class ObjInstance : public Obj {
public:
    ObjClass* klass;
    Table fields; // [fields]
    ObjInstance(ObjClass* klass);
    ~ObjInstance();
    std::string stringify(void);
    void blaken(void);
};
//< Classes and Instances obj-instance

//> Methods and Initializers obj-bound-method
class ObjBoundMethod : public Obj {
public:
    Value receiver;
    ObjClosure* method;
    ObjBoundMethod(Value pReceiver, ObjClosure* pMethod);
    std::string stringify(void);
    void blaken(void);
};

class ObjComplex : public Obj {
public:
    std::complex<double> value;
    ObjComplex(const std::complex<double> v);
    ~ObjComplex();
    std::string stringify(void);
    void blaken(void);
};

class ObjList : public Obj {
public:
    std::vector<Value> container;
    ObjList();
    ~ObjList();
    std::string stringify(void);
    void blaken(void);
};

class ObjMap : public Obj {
public:
    std::map<std::string, Value> container;
    ObjMap();
    ~ObjMap();
    std::string stringify(void);
    void blaken(void);
};

//< copy-string-h
//> is-obj-type
static inline bool isObjType(Value value, ObjType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

class ObjectFactory {
public:
    //< Methods and Initializers obj-bound-method
    //> Calls and Functions new-function-h
    virtual ObjFunction* newFunction() = 0;
    //< Calls and Functions new-function-h
    //> take-string-h
    virtual ObjString* newString(std::string chars) = 0;
    virtual ObjString* newString(const char* chars, int length) = 0;
    virtual ObjComplex* newComplex(const std::complex<double> v) = 0;
    //< take-string-h
    virtual void push(Value value) = 0;
    virtual Value pop() = 0;
    virtual ObjList* newList(void) = 0;
    virtual ObjMap* newMap(void) = 0;
};

//< is-obj-type
#endif
