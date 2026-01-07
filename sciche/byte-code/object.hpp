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

#include <armadillo>
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
#define IS_NATIVE_OBJECT(value) isObjType(value, OBJ_NATIVE_OBJ)
#define IS_COL(value) isObjType(value, OBJ_COL)
#define IS_ROW(value) isObjType(value, OBJ_ROW)
#define IS_MAT(value) isObjType(value, OBJ_MAT)
#define IS_CUBE(value) isObjType(value, OBJ_CUBE)
#define IS_THREAD(value) isObjType(value, OBJ_THREAD)

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
#define AS_NATIVE_OBJECT(value) ((ObjNativeObject*)AS_OBJ(value))
#define AS_COL(value) ((ObjCol*)AS_OBJ(value))
#define AS_ROW(value) ((ObjRow*)AS_OBJ(value))
#define AS_MAT(value) ((ObjMat*)AS_OBJ(value))
#define AS_CUBE(value) ((ObjCube*)AS_OBJ(value))
#define AS_THREAD(value) ((ObjThread*)AS_OBJ(value))

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
    OBJ_MAP,
    OBJ_COL,
    OBJ_ROW,
    OBJ_MAT,
    OBJ_CUBE,
    OBJ_NATIVE_OBJ,
    OBJ_THREAD
} ObjType;
//< obj-type

typedef enum {
    OBJ_FILL_DEFAULT = 0,
    OBJ_FILL_ZEROS,
    OBJ_FILL_ONES,
    OBJ_FILL_RANDU,
    OBJ_FILL_RANDN,
    OBJ_FILL_EYE
} ObjFillType;

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
    Value get(int index);
    void set(int index, Value v);
};

class ObjMap : public Obj {
public:
    std::map<std::string, Value> container;
    ObjMap();
    ~ObjMap();
    std::string stringify(void);
    void blaken(void);
    Value get(std::string index);
    void set(std::string index, Value v);
};

class ObjCol : public Obj {
public:
    arma::vec value;
    ObjCol();
    ~ObjCol();
    std::string stringify(void);
    void blaken(void);
    Value get(int index);
    void set(int index, Value v);
};

class ObjRow : public Obj {
public:
    arma::rowvec value;
    ObjRow();
    ~ObjRow();
    std::string stringify(void);
    void blaken(void);
    Value get(int index);
    void set(int index, Value v);
};

class ObjMat : public Obj {
public:
    arma::mat value;
    ObjMat();
    ~ObjMat();
    std::string stringify(void);
    void blaken(void);
    Value get(int row, int col);
    void set(int row, int col, Value v);
};

class ObjCube : public Obj {
public:
    arma::cube value;
    ObjCube();
    ~ObjCube();
    std::string stringify(void);
    void blaken(void);
    Value get(int row, int col, int dep);
    void set(int row, int col, int dep, Value v);
};

class NativeClass {
public:
    virtual ~NativeClass()
    {
    }
    virtual Value invoke(ObjectFactory* factory, std::string name, int argc, Value* argv) = 0;
    virtual Value property(ObjectFactory* factory, std::string name) = 0;
};

class ObjNativeObject : public Obj {
public:
    NativeClass* klass;
    ObjNativeObject();
    ~ObjNativeObject();
    std::string stringify(void);
    void blaken(void);
};

//< stack-max
/* A Virtual Machine stack-max < Calls and Functions frame-max
#define STACK_MAX 256
*/
//> Calls and Functions frame-max
#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)
//< Calls and Functions frame-max
//> Calls and Functions call-frame

class CallFrame {
public:
    /* Calls and Functions call-frame < Closures call-frame-closure
      ObjFunction* function;
    */
    //> Closures call-frame-closure
    ObjClosure* closure;
    //< Closures call-frame-closure
    uint8_t* ip;
    Value* slots;
};
//< Calls and Functions call-frame

class ObjThread : public Obj {
public:
    ObjThread();
    ObjThread(ObjClosure* closure);
    ~ObjThread();
    //> Calls and Functions frame-array
    CallFrame frames[FRAMES_MAX];
    int frameCount;

    //< Calls and Functions frame-array
    //> vm-stack
    Value stack[STACK_MAX];
    Value* stackTop;
    //< vm-stack
    ObjThread* caller;

public:
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
    virtual ObjCol* newCol(size_t size = 0, ObjFillType fill_type = OBJ_FILL_DEFAULT) = 0;
    virtual ObjRow* newRow(size_t size = 0, ObjFillType fill_type = OBJ_FILL_DEFAULT) = 0;
    virtual ObjMat* newMat(size_t rows = 0, size_t cols = 0, ObjFillType fill_type = OBJ_FILL_DEFAULT) = 0;
    virtual ObjCube* newCube(size_t rows = 0, size_t cols = 0, size_t depth = 0,
                             ObjFillType fill_type = OBJ_FILL_DEFAULT) = 0;
    virtual ObjNativeObject* newNativeObj(NativeClass* klass) = 0;
    virtual ObjThread* newThread(void) = 0;
    virtual ObjThread* newThread(ObjClosure* closure) = 0;
    virtual bool loadLibrary(std::string path, std::string name) = 0;
    virtual bool callFunction(Value value, int argc, Value* argv) = 0;
};

//< is-obj-type
#endif
