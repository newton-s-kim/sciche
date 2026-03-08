#pragma once

#include <sys/time.h>

namespace nsl {
class timer {
private:
    struct timeval start;
    struct timeval end;

public:
    timer()
    {
    }
    uint64_t duration(void)
    {
        return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    }
    void tick(void)
    {
        gettimeofday(&start, NULL);
    }
    void tock(void)
    {
        gettimeofday(&end, NULL);
    }
};
} // namespace nsl
