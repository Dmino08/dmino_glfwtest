#include "graphics/core/Shader.hpp"

#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

#include "core/Logger.hpp"


unsigned int Shader::getUniformLocation(const std::string& name) {
    auto found = uniformLocations_.find(name);
    
    if (found == uniformLocations_.end())
    {
        unsigned int location = glGetUniformLocation(ID_, name.c_str());
        if (location == -1) {
            logger.log(Logger::WARNING, "Shader doesn't have the uniform variable " + name);
        }
        else {
            uniformLocations_.try_emplace(name, location);
        }
        return location;
    }

    return found->second;
}


Shader::Shader(GLuint id) : ID_(id) {}

Shader::~Shader() {
    if (ID_ != 0) {
        glDeleteProgram(ID_);
        ID_ = 0;
    }
}



void Shader::use() const {
    if (ID_ != 0) {
        glUseProgram(ID_);
    }
}


void Shader::uniform1i(const std::string& name, int x) {
    unsigned int location = getUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, x);
    }
}   

void Shader::uniform2i(const std::string& name, int x, int y) {
    unsigned int location = getUniformLocation(name);
    if (location != -1) {    
        glUniform2i(location, x, y);
    }
}  

void Shader::uniform2i(const std::string& name, glm::ivec2 xy) {
    unsigned int location = getUniformLocation(name);
    if (location != -1) {      
        glUniform2i(location, xy.x, xy.y);
    }
}  

void Shader::uniform3i(const std::string& name, int x, int y, int z) {
    unsigned int location = getUniformLocation(name);
    if (location != -1) {      
        glUniform3i(location, x, y, z);
    }
}  

void Shader::uniform3i(const std::string& name, glm::ivec3 xyz) {
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniform3i(location, xyz.x, xyz.y, xyz.z);
    }
}  

void Shader::uniform1f(const std::string& name, float x) {
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniform1f(location, x);
    }
}  

void Shader::uniform2f(const std::string& name, float x, float y) {
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniform2f(location, x, y);
    }
}  

void Shader::uniform2f(const std::string& name, glm::vec2 xy) {
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniform2f(location, xy.x, xy.y);
    }
}  

void Shader::uniform3f(const std::string& name, float x, float y, float z) {
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniform3f(location, x, y, z);
    }
}  

void Shader::uniform3f(const std::string& name, glm::vec3 xyz) {
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniform3f(location, xyz.x, xyz.y, xyz.z);
    }
}  

void Shader::uniformMatrix(const std::string& name, glm::mat4 matrix) {
    unsigned int location = getUniformLocation(name);
    if (location != -1) {     
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}  





std::unique_ptr<Shader> Shader::create(const char* vertex_path, const char* fragment_path) {
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
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << '\n';
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    
    GLint success;
    GLchar infoLog[512];

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';

        return nullptr;
    }
    
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';

        glDeleteShader(vertex);
        return nullptr;
    }

    GLuint id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetShaderiv(id, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        
        return nullptr;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    

    return std::make_unique<Shader>(id);
}
