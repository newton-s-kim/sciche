#pragma once

namespace nsl {
struct cstr_t {
    size_t reference;
    size_t length;
    void* ptr;
};

template <typename T = char>
class basic_string {
private:
    cstr_t* m_str;
    void increaseReference(void);
    void decreaseReference(void);

public:
    basic_string();
    basic_string(basic_string& str);
    basic_string(const T* str);
    ~basic_string();
    const T* c_str(void);
    size_t size(void);
    bool operator ==(basic_string& str);
    bool operator <(basic_string& str);
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
basic_string<T>::basic_string(basic_string& str)
{
    m_str = str.m_str;
    increaseReference();
}

template <typename T>
basic_string<T>::basic_string(const T* str)
{
    m_str = (cstr_t*)malloc(sizeof(c_str) + strlen(str) + 1);
    m_str->ptr = m_str + 1;
    strcpy(str->ptr, str);
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
const T* basic_string<T>::c_str(void)
{
    return (T*)((m_str) ? m_str->ptr : NULL);
}

template <typename T>
bool basic_string<T>::operator ==(basic_string& str) {
	if(m_str == str.m_str) return true;
	else if(!strcmp((T*)m_str->ptr, (T*)str.m_str->ptr)) return true;
	return false;
}

template <typename T>
bool basic_string<T>::operator <(basic_string& str) {
	if(m_str == str.m_str) return false;
	if(!str.m_str) return false;
	if(!m_str) return true;
	if(m_str->length < str.m_str->length) return true;
	return false;
}

using string = basic_string<char>;
}
