//> Hash Tables table-c
#include <stdlib.h>
#include <string.h>

#include "table.hpp"
#include "value.hpp"

bool Table::get(std::string key, Value* value)
{
    std::map<std::string, Value>::iterator it = entries.find(key);
    if (it == entries.end())
        return false;

    *value = it->second;
    return true;
}
//< table-get
//> table-set
bool Table::set(std::string key, Value value)
{
    std::map<std::string, Value>::iterator it = entries.find(key);
    bool isNewKey = (it != entries.end()) ? false : true;
    entries[key] = value;
    return isNewKey;
}
//< table-set
//> table-delete
bool Table::remove(std::string key)
{
    std::map<std::string, Value>::iterator it = entries.find(key);
    if (it == entries.end())
        return false;

    entries.erase(key);
    return true;
}
//< table-delete
//> table-add-all
void Table::addAll(Table& from)
{
    entries.insert(from.entries.begin(), from.entries.end());
}
//< table-add-all
//> Garbage Collection mark-table
void Table::mark(std::function<void(Value)> callback)
{
    for (std::map<std::string, Value>::iterator it = entries.begin(); it != entries.end(); it++) {
        callback(it->second);
    }
}
//< Garbage Collection mark-table
