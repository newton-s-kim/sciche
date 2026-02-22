//> Hash Tables table-c
#include <stdlib.h>
#include <string.h>

#include "table.hpp"
#include "value.hpp"

namespace sce {
bool Table::get(const nsl::string& key, Value* value)
{
    std::map<nsl::string, Value>::iterator it = entries.find(key);
    if (it == entries.end())
        return false;

    *value = it->second;
    return true;
}
//< table-get
//> table-set
bool Table::set(const nsl::string& key, Value value)
{
    std::map<nsl::string, Value>::iterator it = entries.find(key);
    bool isNewKey = (it != entries.end()) ? false : true;
    entries[key] = value;
    return isNewKey;
}
//< table-set
//> table-delete
bool Table::remove(const nsl::string& key)
{
    std::map<nsl::string, Value>::iterator it = entries.find(key);
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
    for (std::map<nsl::string, Value>::iterator it = entries.begin(); it != entries.end(); it++) {
        callback(it->second);
    }
}
} // namespace sce
//< Garbage Collection mark-table
