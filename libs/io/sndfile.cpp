#include "sndfile.hpp"

namespace sce {
static Value sndfile_open(ObjectFactory* factory, NativeClass* klass, int argc, Value* argv)
{
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
    SndFile* sf = (SndFile*)klass;
    SndWav* wav = sf->open(path, mode);
    ObjNativeObject* obj = factory->newNativeObj(wav);
    return OBJ_VAL(obj);
}

static Value sndfile_prop_read(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(SFM_READ);
}

static Value sndfile_prop_write(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(SFM_WRITE);
}

static Value sndfile_prop_rdwr(ObjectFactory* factory, NativeClass* klass)
{
    (void)factory;
    (void)klass;
    return NUMBER_VAL(SFM_RDWR);
}

// clang-format off
std::unordered_map<std::string_view, NativeClassBoundFn> s_sndfile_apis = {
    {"open", sndfile_open}
};

std::unordered_map<std::string_view, NativeClassBoundProperty> s_sndfile_constatns = {
    {"READ", sndfile_prop_read},
    {"WRITE", sndfile_prop_write},
    {"RDWR", sndfile_prop_rdwr}
};
// clang-format on

SndFile::SndFile() : NativeClass(s_sndfile_apis, s_sndfile_constatns)
{
}

SndFile::~SndFile()
{
}

SndWav* SndFile::open(std::string path, int mode)
{
    return new SndWav(path, mode);
}
} // namespace sce
