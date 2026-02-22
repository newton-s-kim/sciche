//> Hash Tables table-h
#ifndef clox_table_h
#define clox_table_h

#include "common.hpp"
#include "value.hpp"
#include <functional>
#include <map>
#include "string.hpp"

namespace sce {
class Table {
private:
    std::map<nsl::string, Value> entries;

public:
    //> table-get-h
    bool get(const nsl::string& key, Value* value);
    //< table-get-h
    //> table-set-h
    bool set(const nsl::string& key, Value value);
    //< table-set-h
    //> table-delete-h
    bool remove(const nsl::string& key);
    //< table-delete-h
    //> Garbage Collection mark-table-h
    void mark(std::function<void(Value)> callback);
    //< Garbage Collection mark-table-h
    //> table-add-all-h
    void addAll(Table& from);
    //< table-add-all-h
};
} // namespace sce
#endif
