#pragma once

namespace nsl {
#include <stdlib.h>

#define VECTOR_STRIDE 256
template <typename T>
class vector {
private:
    size_t m_capacity;
    size_t m_size;
    T* m_buffer;
    inline void grow(size_t increment = 1);

public:
    vector();
    ~vector();
    inline void push_back(T v);
    inline size_t size(void);
    inline size_t capacity(void);
    inline T& operator[](size_t idx);
    inline void clear(void);
    inline void insert(size_t idx, T v);
    inline void reserve(size_t idx);
};

template <typename T>
vector<T>::vector() : m_capacity(0), m_size(0), m_buffer(NULL)
{
}

template <typename T>
vector<T>::~vector()
{
    clear();
}

template <typename T>
void vector<T>::grow(size_t increment)
{
    m_capacity = ((m_size + increment) / VECTOR_STRIDE + 1) * VECTOR_STRIDE;
    m_buffer = (T*)realloc(m_buffer, sizeof(T) * m_capacity);
}

template <typename T>
void vector<T>::reserve(size_t size)
{
    if (m_size < size)
        size -= m_size;
    grow(size);
}

template <typename T>
void vector<T>::push_back(T v)
{
    if (m_size == m_capacity)
        grow();
    m_buffer[m_size++] = v;
}

template <typename T>
size_t vector<T>::size(void)
{
    return m_size;
}

template <typename T>
size_t vector<T>::capacity(void)
{
    return m_capacity;
}

template <typename T>
void vector<T>::clear(void)
{
    if (m_buffer) {
        free(m_buffer);
        m_buffer = NULL;
    }
    m_size = m_capacity = 0;
}

template <typename T>
void vector<T>::insert(size_t idx, T v)
{
    if (m_size == m_capacity)
        grow();
    if (m_size > idx) {
        memcpy(m_buffer + idx + 1, m_buffer + idx, sizeof(T) * (m_size - idx));
        m_buffer[idx] = v;
        m_size++;
    }
    else if (m_size == idx) {
        m_buffer[m_size++] = v;
    }
    else {
        throw "out of bound";
    }
}

template <typename T>
T& vector<T>::operator[](size_t idx)
{
    if (m_size <= idx)
        throw "out of bound";
    return m_buffer[idx];
}
} // namespace sce
