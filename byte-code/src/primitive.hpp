#pragma once

#include "object.hpp"

#include <string>

namespace sce {

typedef Value (*PrimitiveBoundFn)(ObjectFactory* factory, Value value, int argc, Value* argv);
typedef Value (*PrimitiveBoundProperty)(ObjectFactory* factory, Value value);

class Primitive {
private:
    nsl::map<nsl::string, PrimitiveBoundFn> m_list_apis;
    nsl::map<nsl::string, PrimitiveBoundProperty> m_list_properties;
    nsl::map<nsl::string, PrimitiveBoundFn> m_map_apis;
    nsl::map<nsl::string, PrimitiveBoundProperty> m_map_properties;
    nsl::map<nsl::string, PrimitiveBoundFn> m_col_apis;
    nsl::map<nsl::string, PrimitiveBoundProperty> m_col_properties;
    nsl::map<nsl::string, PrimitiveBoundFn> m_row_apis;
    nsl::map<nsl::string, PrimitiveBoundProperty> m_row_properties;
    nsl::map<nsl::string, PrimitiveBoundFn> m_mat_apis;
    nsl::map<nsl::string, PrimitiveBoundProperty> m_mat_properties;
    nsl::map<nsl::string, PrimitiveBoundFn> m_cube_apis;
    nsl::map<nsl::string, PrimitiveBoundProperty> m_cube_properties;
    nsl::map<nsl::string, PrimitiveBoundFn> m_number_apis;
    nsl::map<nsl::string, PrimitiveBoundProperty> m_number_properties;
    nsl::map<nsl::string, PrimitiveBoundFn> m_bool_apis;
    nsl::map<nsl::string, PrimitiveBoundProperty> m_bool_properties;

public:
    Primitive();
    Value call(ObjectFactory* factory, Value value, nsl::string& name, int argc, Value* argv);
    Value property(ObjectFactory* factory, Value value, nsl::string& name);
};
} // namespace sce
