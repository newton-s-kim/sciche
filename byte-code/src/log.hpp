#pragma once

#include <cstdio>
#include <stdexcept>

#define THROW_UNIMPLEMENTED throw std::runtime_error("not implemented");

#define THROW_NOT_SUPPORTED throw std::runtime_error("not supported");

#define RETURN_NULL return nullptr;

#ifdef LAX_DEBUG
#define LAX_LOG(...)                                                                                                   \
    {                                                                                                                  \
        fprintf(stderr, "%s(%d):%s  ", __FILE__, __LINE__, __FUNCTION__);                                              \
        fprintf(stderr, ##__VA_ARGS__);                                                                                \
        fprintf(stderr, "\n");                                                                                         \
    }
#else // QP_DEBUG
#define LAX_LOG(...)
#endif // QP_DEBUG
