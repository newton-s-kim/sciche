#include "sndwav.hpp"

namespace sce {
static Value sndwav_read(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)argc;
    (void)argv;
    ObjMat* mat = factory->newMat();
    SndWav* wav = (SndWav*)obj;
    wav->read(mat->value);
    return OBJ_VAL(mat);
}

static Value sndwav_close(ObjectFactory* factory, NativeObject* obj, int argc, Value* argv)
{
    (void)argc;
    (void)argv;
    (void)factory;
    SndWav* wav = (SndWav*)obj;
    wav->close();
    return NIL_VAL;
}

// clang-format off
std::map<std::string, NativeObjectBoundFn> s_sndwav_apis = {
    {"read", sndwav_read},
    {"close", sndwav_close}
};

std::map<std::string, NativeObjectBoundProperty> s_sndwav_properties;
// clang-format on

SndWav::SndWav(std::string path, int mode) : NativeObject(s_sndwav_apis, s_sndwav_properties), m_handle(NULL)
{
    memset(&m_info, 0, sizeof(m_info));
    m_handle = sf_open(path.c_str(), mode, &m_info);
    if (!m_handle)
        throw std::runtime_error(sf_strerror(m_handle));
}

SndWav::~SndWav()
{
    if (m_handle)
        sf_close(m_handle);
}

int SndWav::read(arma::mat& mat)
{
    if (NULL == m_handle)
        throw std::runtime_error("sound file is not opened.");
    mat.resize(m_info.channels, m_info.frames);
    return sf_read_double(m_handle, mat.memptr(), m_info.frames * m_info.channels);
}

void SndWav::close(void)
{
    if (m_handle) {
        sf_close(m_handle);
        m_handle = NULL;
    }
}
} // namespace sce
