#ifndef RADIUMENGINE_SHADERPROGRAMANAGER_HPP
#define RADIUMENGINE_SHADERPROGRAMANAGER_HPP

#include <Engine/RaEngine.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <Core/Utils/Singleton.hpp>

namespace globjects {
class File;
class NamedString;
} // namespace globjects

namespace Ra {
namespace Engine {
class ShaderProgram;
class ShaderConfiguration;
}
} // namespace Ra

namespace Ra {
namespace Engine {

/**
 * Manage the set of shaders used by Radium Engine for rendering.
 *
 * This class allows to manage shader programs according to their configuration so that, for a given configuration,
 * there is only one ShaderProgram instance in the system.
 *
 * This manager is a singleton. At the creation of the singleton, one can give parameters that will define the default shader programm
 * example :  Engine::ShaderProgramManager::createInstance( "Shaders/Default.vert.glsl", "Shaders/Default.frag.glsl" );
 *
 */
class RA_ENGINE_API ShaderProgramManager final {
    RA_SINGLETON_INTERFACE( ShaderProgramManager );

  public:
    /**
     * Add a shader program to the program collection according to the given configuration.
     * This method must be called only once an opeGL context is bound.
     * The shader sources corresponding to the configuration will be compiled, linked and verified.
     *
     *
     * @param config the configuration of the programm to add to the collection
     * @return the created shader program. In case of compile/link/verify error, the default shader programm is returned
     * @note ownership on the returned pointer is keep by the manager.
     * @bug, there is no way to know if the returned programm is the default one (except segfault sometimes ...)
     */
    const ShaderProgram* addShaderProgram( const ShaderConfiguration& config );

    /**
     * Get the shader programm corresponding to the given id
     * @param id Name of the programm to retrieve
     * @return the shader programm retrieved, nullptr if the frogram was not in the collection
     */
    const ShaderProgram* getShaderProgram( const std::string& id );

    /**
     * Get the shader programm corresponding to the given configuration
     * @param config Name of the programm to retrieve
     * @return the shader programm retrieved. If no shader programm corresponding to the configuration could be found,
     * return the result of addShaderProgram(config)
     */
    const ShaderProgram* getShaderProgram( const ShaderConfiguration& config );

    /**
     * Get the system default program.
     * Default programm could be defined whe building the singleton of the ShaderProgramManager
     * @return the system default shader programm
     */
    const ShaderProgram* getDefaultShaderProgram() const;

    /**
     * Reload source, recompile, link and validate all the managed programms.
     * Usefull for shaders development
     */
    void reloadAllShaderPrograms();
    /**
     * Programs that did not compiled are temporarilly stored and could be reloaded and compiled when one call this method.
     * If the reloaded programm is ok, it is removed from the set of not compiled program and added to the programm collection.
     */
    void reloadNotCompiledShaderPrograms();

    /**
     * Allows to define named string so that the inclusion mechanism of OpenGL could be efficient.
     * A name string associates the name that will be used by a #define directive in a glsl source file
     * with a file that contains the included glsl source code.
     * @param includepath
     * @param realfile
     */
    void addNamedString( const std::string& includepath, const std::string& realfile );

    /**
     * Reload all the registered name string.
     * Usefull for shader development.
     */
    void reloadNamedString();

  private:
    /// need Initialization after ctr and before use
    ShaderProgramManager( const std::string& vs, const std::string& fs );
    ~ShaderProgramManager();
    void initialize();
    void insertShader( const ShaderConfiguration& config,
                       const std::shared_ptr<ShaderProgram>& shader );

  private:
    std::map<std::string, ShaderConfiguration> m_shaderProgramIds;
    std::map<ShaderConfiguration, std::shared_ptr<ShaderProgram>> m_shaderPrograms;
    std::vector<ShaderConfiguration> m_shaderFailedConfs;

    std::vector<std::unique_ptr<globjects::File>> m_files;
    std::vector<std::unique_ptr<globjects::NamedString>> m_namedStrings;

    std::string m_defaultVsName {};
    std::string m_defaultFsName {};

    const ShaderProgram* m_defaultShaderProgram { nullptr };
};

} // namespace Engine
} // namespace Ra

#endif // RADIUMENGINE_SHADERMANAGER_HPP
