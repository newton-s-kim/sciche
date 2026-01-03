#include "sndwav.hpp"

SndWav::SndWav(std::string path, int mode) : m_handle(NULL)
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

Value SndWav::invoke(ObjectFactory* factory, std::string name, int argc, Value* argv)
{
    (void)argc;
    (void)argv;
    Value value = 0;
    if ("read" == name) {
        if (NULL == m_handle)
            throw std::runtime_error("sound file is not opened.");
        ObjMat* mat = factory->newMat();
        mat->value.resize(m_info.channels, m_info.frames);
        sf_read_double(m_handle, mat->value.memptr(), m_info.frames * m_info.channels);
        value = OBJ_VAL(mat);
    }
    else if ("close" == name) {
        if (m_handle) {
            sf_close(m_handle);
            m_handle = NULL;
        }
    }
    else {
        throw std::runtime_error("invalid mothod");
    }
    return value;
}

Value SndWav::property(ObjectFactory* factory, std::string name)
{
    (void)factory;
    (void)name;
    Value value = 0;
    {
        throw std::runtime_error("invalid property");
    }
    return value;
}
