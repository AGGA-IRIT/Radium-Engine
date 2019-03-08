#include <Core/Asset/MaterialData.hpp>

namespace Ra {
namespace Core {
namespace Asset {

inline void MaterialData::setName( const std::string& name ) {
    m_name = name;
}

inline std::string MaterialData::getType() const {
    return m_type;
}

inline void MaterialData::setType( const std::string& type ) {
    m_type = type;
}

} // namespace Asset
} // namespace Core
} // namespace Ra
