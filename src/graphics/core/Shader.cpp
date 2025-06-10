#include "graphics/core/Shader.hpp"

#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

#include "core/Logger.hpp"


Shader::~Shader() 
{
    if (id_ != 0) {
        glDeleteProgram(id_);
        id_ = 0;
    }
}



void Shader::use() const 
{
    if (id_ != 0) {
        glUseProgram(id_);
    }
}


void Shader::uniform1i(const std::string& name, int x) 
{
    unsigned int location = getUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, x);
    }
}   

void Shader::uniform2i(const std::string& name, int x, int y) 
{
    unsigned int location = getUniformLocation(name);
    if (location != -1) {    
        glUniform2i(location, x, y);
    }
}  

void Shader::uniform2i(const std::string& name, const glm::ivec2& xy) 
{
    unsigned int location = getUniformLocation(name);
    if (location != -1) {      
        glUniform2i(location, xy.x, xy.y);
    }
}  

void Shader::uniform3i(const std::string& name, int x, int y, int z) 
{
    unsigned int location = getUniformLocation(name);
    if (location != -1) {      
        glUniform3i(location, x, y, z);
    }
}  

void Shader::uniform3i(const std::string& name, const glm::ivec3& xyz) 
{
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniform3i(location, xyz.x, xyz.y, xyz.z);
    }
}  

void Shader::uniform1f(const std::string& name, float x) 
{
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniform1f(location, x);
    }
}  

void Shader::uniform2f(const std::string& name, float x, float y) 
{
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniform2f(location, x, y);
    }
}  

void Shader::uniform2f(const std::string& name, const glm::vec2& xy)
{
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniform2f(location, xy.x, xy.y);
    }
}  

void Shader::uniform3f(const std::string& name, float x, float y, float z) 
{
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniform3f(location, x, y, z);
    }
}  

void Shader::uniform3f(const std::string& name, const glm::vec3& xyz) 
{
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniform3f(location, xyz.x, xyz.y, xyz.z);
    }
}  

void Shader::uniformMat3(const std::string& name, const glm::mat3& matrix) 
{
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void Shader::uniformMat4(const std::string& name, const glm::mat4& matrix) 
{
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}  

void Shader::uniformBlockBinding(const std::string& name, GLuint index) 
{
    unsigned int block = getUniformBlockBinding(name);
    if (block != -1) {     
        glUniformBlockBinding(id_, block, index);
    }    
}


bool Shader::create(const char* vertex_path, const char* fragment_path) 
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        std::stringstream vertexStream, fragmentStream;

        vShaderFile.open(vertex_path);
        fShaderFile.open(fragment_path);

        vertexStream << vShaderFile.rdbuf();
        fragmentStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vertexStream.str();
        fragmentCode = fragmentStream.str();
    }
    catch(const std::ifstream::failure& e)
    {
        core::logger.log(core::Logger::ERROR, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " + std::string(e.what()));
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    
    GLint success;
    GLchar infoLog[512];

    GLuint vertex = createShader(GL_VERTEX_SHADER, vShaderCode, success, infoLog);
    if (vertex == GL_INVALID_INDEX)
        return false;
    
    
    GLuint fragment = createShader(GL_FRAGMENT_SHADER, fShaderCode, success, infoLog);
    if (fragment == GL_INVALID_INDEX)
        return false;
    

    GLuint id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetShaderiv(id, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);

        core::logger.log(core::Logger::ERROR, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog));
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return false;
        
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    id_ = id;
    return true;
}

bool Shader::create(const char* vertex_path, const char* fragment_path, const char* geometry_path) 
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        std::stringstream vertexStream, fragmentStream, geometryStream;

        vShaderFile.open(vertex_path);
        fShaderFile.open(fragment_path);
        gShaderFile.open(geometry_path);

        vertexStream << vShaderFile.rdbuf();
        fragmentStream << fShaderFile.rdbuf();
        geometryStream << gShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();
        gShaderFile.close();

        vertexCode = vertexStream.str();
        fragmentCode = fragmentStream.str();
        geometryCode = geometryStream.str();
    }
    catch(const std::ifstream::failure& e)
    {
        core::logger.log(core::Logger::ERROR, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " + std::string(e.what()));
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    
    GLint success;
    GLchar infoLog[512];

    GLuint vertex = createShader(GL_VERTEX_SHADER, vShaderCode, success, infoLog);
    if (vertex == -1)
        return false;
    
    
    GLuint fragment = createShader(GL_FRAGMENT_SHADER, fShaderCode, success, infoLog);
    if (fragment == -1)
        return false;

    GLuint geometry = createShader(GL_GEOMETRY_SHADER, gShaderCode, success, infoLog);
    if (geometry == -1)
        return false;
    

    GLuint id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glAttachShader(id, geometry);
    glLinkProgram(id);

    glGetShaderiv(id, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);

        core::logger.log(core::Logger::ERROR, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog));
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return false;
        
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    id_ = id;
    return true;
}

GLuint Shader::createShader(GLenum type, const char* code, GLint& success, GLchar* info_log) 
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        switch (type)
        {
            case GL_VERTEX_SHADER:
                core::logger.log(core::Logger::ERROR, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(info_log));
            break;
            case GL_FRAGMENT_SHADER:
                core::logger.log(core::Logger::ERROR, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(info_log));
            break;        
            case GL_GEOMETRY_SHADER:
                core::logger.log(core::Logger::ERROR, "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" + std::string(info_log));
            break; 
            default:
                core::logger.log(core::Logger::ERROR, "ERROR::SHADER::COMPILATION_FAILED\n" + std::string(info_log));
            break;
        }
        return -1;
    }
    return shader;
}

unsigned int Shader::getUniformLocation(const std::string& name) 
{
    auto found = uniform_locations_.find(name);
    
    if (found == uniform_locations_.end())
    {
        unsigned int location = glGetUniformLocation(id_, name.c_str());
        if (location == -1) {
            #ifdef SHADER_LOGGING
                core::logger.log(core::Logger::WARNING, "Shader doesn't have the uniform variable " + name);
            #endif
        }
        else {
            uniform_locations_.try_emplace(name, location);
        }
        return location;
    }

    return found->second;
}

unsigned int Shader::getUniformBlockBinding(const std::string& name) 
{
    auto found = uniform_locations_.find(name);
        
        if (found == uniform_locations_.end())
        {
            unsigned int block_index = glGetUniformBlockIndex(id_, name.c_str());
            if (block_index == -1) {
                #ifdef SHADER_LOGGING
                    core::logger.log(core::Logger::WARNING, "Shader doesn't have the uniform variable " + name);
                #endif
            }
            else {
                uniform_locations_.try_emplace(name, block_index);
            }
            return block_index;
        }

        return found->second;    
}