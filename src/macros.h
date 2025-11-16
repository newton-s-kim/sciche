#pragma once

#include <cstdio>
#include <stdexcept>

#define THROW_UNIMPLEMENTED throw std::runtime_error("not implemented");

#define THROW_NOT_SUPPORTED throw std::runtime_error("not supported");

#define RETURN_NULL return nullptr;

#ifdef DEBUG
#define LOG(...)                                                                                                       \
    {                                                                                                                  \
        fprintf(stderr, "%s(%d):%s  ", __FILE__, __LINE__, __FUNCTION__);                                              \
        fprintf(stderr, ##__VA_ARGS__);                                                                                \
        fprintf(stderr, "\n");                                                                                         \
    }
#else // DEBUG
#define LOG(...)
#endif // DEBUG
