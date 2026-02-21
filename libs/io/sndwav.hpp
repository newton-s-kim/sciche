#pragma once
#include "object.hpp"

#include <sndfile.h>

namespace sce {
class SndWav : public NativeObject {
private:
    SNDFILE* m_handle;
    SF_INFO m_info;

public:
    SndWav(std::string path, int mode);
    ~SndWav();
    int read(arma::mat& mat);
    void close(void);
};
} // namespace sce
