#include "sndfile.hpp"

#include "sndwav.hpp"

SndFile::SndFile()
{
}

SndFile::~SndFile()
{
}

Value SndFile::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    Value value = 0;
    (void)factory;
    (void)name;
    (void)argc;
    (void)argv;
    if ("open" == name) {
        std::string path;
        int mode = SFM_READ;
        if (2 == argc) {
            if (!IS_NUMBER(argv[1]))
                throw std::runtime_error("number is expected");
            mode = AS_NUMBER(argv[1]);
            if (!IS_STRING(argv[0]))
                throw std::runtime_error("string is expected");
            path = AS_STRING(argv[0])->chars;
        }
        else if (1 == argc) {
            if (!IS_STRING(argv[0]))
                throw std::runtime_error("string is expected");
            path = AS_STRING(argv[0])->chars;
        }
        else {
            throw std::runtime_error("invalid number of arguments");
        }
        ObjNativeObject* obj = factory->newNativeObj(new SndWav(path, mode));
        value = OBJ_VAL(obj);
    }
    else {
        throw std::runtime_error("invalid mothod");
    }
    return value;
}

Value SndFile::property(ObjectFactory* factory, std::string name)
{
    Value value = 0;
    (void)factory;
    (void)name;
    if ("READ" == name) {
        value = NUMBER_VAL(SFM_READ);
    }
    else if ("WRITE" == name) {
        value = NUMBER_VAL(SFM_WRITE);
    }
    else if ("RDWR" == name) {
        value = NUMBER_VAL(SFM_RDWR);
    }
    else {
        throw std::runtime_error("invalid property");
    }
    return value;
}
