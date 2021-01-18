#include <Core/Resources/Resources.hpp>

#include <Core/Utils/Log.hpp>
#include <algorithm>
#include <cpplocate/cpplocate.h>
#include <filesystem>

namespace Ra {
namespace Core {
namespace Resources {

using namespace Ra::Core::Utils;
std::filesystem::path searchPath( std::string pattern, std::string offset, void* libSymbol ) {
    std::string baseDir = cpplocate::locatePath( pattern, offset, libSymbol );
    std::filesystem::path p( baseDir );
    return std::filesystem::canonical( p );
}

std::string getRadiumResourcesDir() {
    auto basePath =
        searchPath( "Resources/Shaders", "", reinterpret_cast<void*>( getRadiumResourcesDir ) );
    auto resourcesPath = basePath / "Resources" / "";
    return resourcesPath.string();
}

std::string getRadiumPluginsDir() {
    auto basePath = searchPath( "Plugins/lib", "", reinterpret_cast<void*>( getRadiumPluginsDir ) );
    auto pluginsPath = basePath / "Plugins" / "lib" / "";
    return pluginsPath.string();
}

std::string getBaseDir() {
    return cpplocate::getModulePath();
}

std::string getBaseResourcesDir() {
    auto baseDir = cpplocate::locatePath( "Resources", "", nullptr );
    return baseDir + "Resources/";
}

ResourcesLocator::ResourcesLocator( void* symbol, const std::string& offset ) : m_basePath {""} {
    m_basePath = searchPath( "/", "", symbol ) + offset;
}

ResourcesLocator::ResourcesLocator( void* symbol, std::string pattern, const std::string& offset ) {
    m_basePath = searchPath( pattern, offset, symbol ) + pattern;
}

const std::string& ResourcesLocator::getBasePath() {
    return m_basePath;
}
} // namespace Resources
} // namespace Core
} // namespace Ra
