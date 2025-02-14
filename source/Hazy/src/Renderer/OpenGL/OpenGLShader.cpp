#include <glad/glad.h>
#include <hazy_pch.h>
#include "Hazy/Renderer/Shader.h"
#include "Hazy/Application.h"

#include "assert.h"
#define CALL(x) x; assert(glGetError() == GL_NO_ERROR)

namespace Hazy {
    OpenGLShader::OpenGLShader(const std::string& vertPath, const std::string& fragPath, const std::string& geomPath) {
        uint32_t vertexShader = compileShader(vertPath, GL_VERTEX_SHADER);
        uint32_t fragmentShader = compileShader(fragPath, GL_FRAGMENT_SHADER);
        uint32_t geometryShader = compileShader(geomPath, GL_GEOMETRY_SHADER);

        linkProgram(vertexShader, fragmentShader, geometryShader);
    }

    OpenGLShader::OpenGLShader(const std::string& vertPath, const std::string& fragPath) {
        uint32_t vertexShader = compileShader(vertPath, GL_VERTEX_SHADER);
        uint32_t fragmentShader = compileShader(fragPath, GL_FRAGMENT_SHADER);

        linkProgram(vertexShader, fragmentShader);
    }

    OpenGLShader::~OpenGLShader() {
        CALL(glDeleteProgram(m_shaderID));
    }

    void OpenGLShader::bind() {
        CALL(glUseProgram(m_shaderID));
    }

    void OpenGLShader::unbind() {
        CALL(glUseProgram(0));
    }

    int OpenGLShader::findUniformLocation(const std::string& name) {
        int location = -1;
        try {
            location = m_uniformCached.at(name);
        }
        catch (std::out_of_range&) {
            CALL(location = glGetUniformLocation(m_shaderID, name.c_str()));
            if (location == -1) {
                throw std::logic_error("uniform \"" + name + "\" not found");
            }
            else {
                m_uniformCached.emplace(name, location);
            }
        }
        return location;
    }

    Shader& OpenGLShader::setMat4(const std::string& name, const glm::mat4& mat) {
        try {
            int location = findUniformLocation(name);
            CALL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)));
        }
        catch (std::logic_error& e) {
            Logger::LogError("Failed to set uniform, because {}", e.what());
            throw e;
        }
        return *this;
    }

    Shader& OpenGLShader::setVec4(const std::string& name, const glm::vec4& vec) {
        try {
            int location = findUniformLocation(name);
            CALL(glUniform4fv(location, 1, glm::value_ptr(vec)));
        }
        catch (std::logic_error& e) {
            Logger::LogError("Failed to set uniform, because {}", e.what());
            throw e;
        }
        return *this;
    }

    Shader& OpenGLShader::setVec3(const std::string& name, const glm::vec3& value) {
        try {
            int location = findUniformLocation(name);
            CALL(glUniform3fv(location, 1, glm::value_ptr(value)));
        }
        catch (std::logic_error& e) {
            Logger::LogError("Failed to set uniform, because {}", e.what());
            throw e;
        }
        return *this;
    }

    Shader& OpenGLShader::setInt(const std::string& name, int value) {
        try {
            int location = findUniformLocation(name);
            CALL(glUniform1i(location, value));
        }
        catch (std::logic_error& e) {
            Logger::LogError("Failed to set uniform, because {}", e.what());
            throw e;
        }
        return *this;
    }

    Shader& OpenGLShader::setFloat(const std::string& name, float value) {
        try {
            int location = findUniformLocation(name);
            CALL(glUniform1f(location, value));
        }
        catch (std::logic_error& e) {
            Logger::LogError("Failed to set uniform, because {}", e.what());
            throw e;
        }
        return *this;
    }

    uint32_t OpenGLShader::compileShader(const std::string& path, uint32_t type) {
        std::ifstream file(path);
        std::stringstream stream;
        if (!file.is_open()) {
            file.close();
            throw std::runtime_error("Failed to open shader file: " + path);
        }
        stream << file.rdbuf();
        file.close();
        std::string source = stream.str();

        int success;
        uint32_t shader = glCreateShader(type);
        const GLchar* src = (const GLchar*)source.c_str();
        CALL(glShaderSource(shader, 1, &src, 0));
        CALL(glCompileShader(shader));
        CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (!success) {
            char infoLog[512]; int length;
            CALL(glGetShaderInfoLog(shader, 512, &length, infoLog));
            CALL(glDeleteShader(shader));
            switch (type) {
            case GL_VERTEX_SHADER:
                throw std::runtime_error("Vertex shader compilation failed: " + std::string(infoLog, length));
            case GL_FRAGMENT_SHADER:
                throw std::runtime_error("Fragment shader compilation failed: " + std::string(infoLog, length));
            case GL_GEOMETRY_SHADER:
                throw std::runtime_error("Geometry shader compilation failed: " + std::string(infoLog, length));
            default:
                throw std::runtime_error("Shader compilation failed: " + std::string(infoLog, length));
            }
        }

        return shader;
    }

    void OpenGLShader::linkProgram(uint32_t vertexShader, uint32_t fragmentShader, uint32_t geometryShader) {
        m_shaderID = glCreateProgram();
        CALL(glAttachShader(m_shaderID, vertexShader));
        CALL(glAttachShader(m_shaderID, fragmentShader));
        if (geometryShader != 0) {
            CALL(glAttachShader(m_shaderID, geometryShader));
        }
        CALL(glLinkProgram(m_shaderID));

        int success;
        CALL(glGetProgramiv(m_shaderID, GL_LINK_STATUS, &success));
        if (!success) {
            char infoLog[512]; int length;
            CALL(glGetProgramInfoLog(m_shaderID, 512, &length, infoLog));
            CALL(glDeleteProgram(m_shaderID));
            throw std::runtime_error("Shader program linking failed: " + std::string(infoLog, length));
        }

        CALL(glDetachShader(m_shaderID, vertexShader));   CALL(glDeleteShader(vertexShader));
        CALL(glDetachShader(m_shaderID, fragmentShader)); CALL(glDeleteShader(fragmentShader));
        if (geometryShader != 0) {
            CALL(glDetachShader(m_shaderID, geometryShader)); CALL(glDeleteShader(geometryShader));
        }
    }

}

#undef CALL