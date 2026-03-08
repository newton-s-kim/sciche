#pragma once

#include "log.hpp"

namespace nsl {

template <typename T = char>
class basic_string {
private:
struct cstr_t {
    size_t reference;
    size_t length;
    uint32_t m_hash;
    T* ptr;
} * m_str;
    void increaseReference(void);
    void decreaseReference(void);
    uint32_t hashString(const char* str, size_t len);

public:
    basic_string();
    basic_string(const basic_string& str);
    basic_string(const T* str);
    ~basic_string();
    inline const T* c_str(void);
    inline size_t size(void);
    inline bool empty(void);
    inline basic_string<T>& operator =(const basic_string& str);
    inline bool operator ==(const basic_string& str);
    inline bool operator <(const basic_string& str) const;
    inline uint32_t hash(void) { return (m_str) ? m_str->m_hash : 0; }
    inline void clear(void) {decreaseReference(); m_str = NULL;}
};

template <typename T>
basic_string<T>::basic_string() : m_str(NULL)
{
}

template <typename T>
void basic_string<T>::increaseReference(void)
{
    m_str->reference++;
}

template <typename T>
void basic_string<T>::decreaseReference(void)
{
    m_str->reference--;
    if (0 == m_str->reference) {
        free(m_str);
        m_str = NULL;
    }
}

template <typename T>
basic_string<T>::basic_string(const basic_string& str)
{
    m_str = str.m_str;
    increaseReference();
}

template <typename T>
basic_string<T>::basic_string(const T* str)
{
    size_t sz = strlen(str);
    m_str = (cstr_t*)malloc(sizeof(cstr_t) + sz + 1);
    m_str->ptr = (T*)(m_str + 1);
    m_str->reference = 1;
    m_str->length = sz;
    m_str->m_hash = hashString(str, sz);
    strcpy((T*)m_str->ptr, str);
}

template <typename T>
basic_string<T>::~basic_string()
{
    if (m_str)
        decreaseReference();
}

template <typename T>
size_t basic_string<T>::size()
{
    return (m_str) ? m_str->length : 0;
}

template <typename T>
bool basic_string<T>::empty(void)
{
    return (m_str && m_str->length) ? false : true;
}


template <typename T>
const T* basic_string<T>::c_str(void)
{
    return (m_str) ? m_str->ptr : NULL;
}

template <typename T>
basic_string<T>& basic_string<T>::operator =(const basic_string& str) {
	if(m_str) decreaseReference();
	m_str = str.m_str;
	if(m_str) increaseReference();
	return *this;
}

template <typename T>
bool basic_string<T>::operator ==(const basic_string& str) {
	if(m_str == str.m_str) return true;
	if(m_str->m_hash != str.m_str->m_hash) return false;
	if(!str.m_str || !m_str) return false;
	if(str.m_str->length != m_str->length) return false;
	return (!strcmp(m_str->ptr, str.m_str->ptr));
}

template <typename T>
bool basic_string<T>::operator <(const basic_string& str) const {
	if(m_str == str.m_str || !str.m_str || !m_str) return false;
	LAX_LOG("strcmp(%s, %s)", m_str->ptr, str.m_str->ptr);
	if(m_str->ptr[0] < str.m_str->ptr[0]) return true;
	return (0 > strcmp(m_str->ptr, str.m_str->ptr));
}

template <typename T>
uint32_t basic_string<T>::hashString(const char* str, size_t len) {
	uint32_t hash = 2166136261u;
	for(size_t i = 0 ; i < len; i++) {
		hash ^= (uint8_t)str[i];
		hash *= 16777619;
	}
	return hash;
}

using string = basic_string<char>;
}
