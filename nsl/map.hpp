#pragma once

#include "array.hpp"
#include "string.hpp"

#include <functional>

namespace nsl {

template <typename K, typename V>
class map {
private:
    size_t m_count;
    size_t m_capacity;
    V m_nil;
    typedef struct {
        K key;
        V value;
    } Entry;
    Entry* m_entries;
    Entry* findEntry(Entry* entries, size_t capacity, K& key)
    {
        /* Hash Tables find-entry < Optimization initial-index
          uint32_t index = key->hash % capacity;
        */
        //> Optimization initial-index
        uint32_t index = key.hash() & (capacity - 1);
        //< Optimization initial-index
        //> find-entry-tombstone
        Entry* tombstone = NULL;

        //< find-entry-tombstone
        for (;;) {
            Entry* entry = &entries[index];
            /* Hash Tables find-entry < Hash Tables find-tombstone
                if (entry->key == key || entry->key == NULL) {
                  return entry;
                }
            */
            //> find-tombstone
            if (entry->key.empty()) {
                if (entry->value == m_nil) {
                    // Empty entry.
                    return tombstone != NULL ? tombstone : entry;
                }
                else {
                    // We found a tombstone.
                    if (tombstone == NULL)
                        tombstone = entry;
                }
            }
            else if (entry->key == key) {
                // We found the key.
                return entry;
            }
            //< find-tombstone

            /* Hash Tables find-entry < Optimization next-index
                index = (index + 1) % capacity;
            */
            //> Optimization next-index
            index = (index + 1) & (capacity - 1);
            //< Optimization next-index
        }
    }
    void adjustCapacity(size_t capacity);

public:
    map(V nil);
    ~map();
    inline bool get(K key, V* value);
    inline bool set(K key, V value);
    inline bool remove(K& key);
    inline void addAll(map<K, V>& from);
    inline V& find(K& key);
    inline void visit(std::function<void(V&)> callback);
    inline void iterate(std::function<void(nsl::string, V&)> callback);
    inline size_t size(void)
    {
        return m_count;
    }
};

//> max-load
#define TABLE_MAX_LOAD 0.75

//< max-load
template <typename K, typename V>
map<K, V>::map(V nil)
{
    m_count = 0;
    m_capacity = 0;
    m_entries = NULL;
    m_nil = nil;
}
//> free-table
template <typename K, typename V>
map<K, V>::~map()
{
    FREE_ARRAY(Entry, m_entries, m_capacity);
}
//< free-table
//> find-entry
//> omit
// NOTE: The "Optimization" chapter has a manual copy of this function.
// If you change it here, make sure to update that copy.
//< omit
/*
template <typename K, typename V>
Entry* map<K,V>::findEntry(Entry* entries, size_t capacity, K& key) {
}
*/
//< find-entry
//> table-get
template <typename K, typename V>
bool map<K, V>::get(K key, V* value)
{
    if (m_count == 0)
        return false;

    Entry* entry = findEntry(m_entries, m_capacity, key);
    if (entry->key.empty())
        return false;

    *value = entry->value;
    return true;
}
//< table-get
//> table-adjust-capacity
template <typename K, typename V>
void map<K, V>::adjustCapacity(size_t capacity)
{
    Entry* entries = ALLOCATE(Entry, capacity);
    memset((void*)entries, 0, sizeof(Entry) * capacity);
    for (size_t i = 0; i < capacity; i++) {
        // entries[i].key = NULL;
        entries[i].value = m_nil;
    }
    //> re-hash

    //> resize-init-count
    m_count = 0;
    //< resize-init-count
    for (size_t i = 0; i < m_capacity; i++) {
        Entry* entry = &m_entries[i];
        if (entry->key.empty())
            continue;

        Entry* dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        //> resize-increment-count
        m_count++;
        //< resize-increment-count
    }
    //< re-hash

    //> Hash Tables free-old-array
    FREE_ARRAY(Entry, m_entries, m_capacity);
    //< Hash Tables free-old-array
    m_entries = entries;
    m_capacity = capacity;
}
//< table-adjust-capacity
//> table-set
template <typename K, typename V>
bool map<K, V>::set(K key, V value)
{
    //> table-set-grow
    if (m_count + 1 > m_capacity * TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(m_capacity);
        adjustCapacity(capacity);
    }

    //< table-set-grow
    Entry* entry = findEntry(m_entries, m_capacity, key);
    bool isNewKey = entry->key.empty();
    /* Hash Tables table-set < Hash Tables set-increment-count
      if (isNewKey) m_count++;
    */
    //> set-increment-count
    if (isNewKey && entry->value == m_nil)
        m_count++;
    //< set-increment-count

    entry->key = key;
    entry->value = value;
    return isNewKey;
}
//< table-set
//> table-delete
template <typename K, typename V>
bool map<K, V>::remove(K& key)
{
    if (m_count == 0)
        return false;

    // Find the entry.
    Entry* entry = findEntry(m_entries, m_capacity, key);
    if (entry->key.empty())
        return false;

    // Place a tombstone in the entry.
    entry->key.clear();
    entry->value = m_nil;
    return true;
}
//< table-delete
//> table-add-all
template <typename K, typename V>
void map<K, V>::addAll(map<K, V>& from)
{
    for (size_t i = 0; i < from.m_capacity; i++) {
        Entry* entry = &from.m_entries[i];
        if (!entry->key.empty()) {
            set(entry->key, entry->value);
        }
    }
}
//< table-add-all
//> Garbage Collection mark-table
template <typename K, typename V>
void map<K, V>::visit(std::function<void(V&)> callback)
{
    for (size_t i = 0; i < m_capacity; i++) {
        Entry* entry = &m_entries[i];
        if (!entry->key.empty())
            callback(entry->value);
    }
}
template <typename K, typename V>
void map<K, V>::iterate(std::function<void(nsl::string, V&)> callback)
{
    for (size_t i = 0; i < m_capacity; i++) {
        Entry* entry = &m_entries[i];
        if (!entry->key.empty())
            callback(entry->key, entry->value);
    }
}

} // namespace nsl
