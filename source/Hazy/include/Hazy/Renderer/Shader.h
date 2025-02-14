#pragma once
#include "Hazy/Renderer/Interface.h"
#include <hazy_pch.h>

namespace Hazy {

    /**
     * @brief 着色器程序接口，确保在操作着色器程序时有上下文
     * @warning 请勿直接构造一个着色器程序对象，请使用Context的create模板函数来创建
     */
    class Shader {
    public:
        virtual ~Shader() { }
        virtual void bind() = 0;
        virtual void unbind() = 0;

        /**
         * @brief 设置矩阵统一变量
         * @param name 变量名
         * @param mat  矩阵
         * @return Shader& 此着色器程序引用
         * @warning 需要在绑定此着色器且当前处于此着色器所属的上下文
         * @throw std::logic_error 未找到此变量
         */
        virtual Shader& setMat4(const std::string& name, const glm::mat4& mat) = 0;

        /**
         * @brief 设置向量统一变量
         * @param name 变量名
         * @param vec  向量
         * @return Shader& 此着色器程序引用
         * @warning 需要在绑定此着色器且当前处于此着色器所属的上下文
         * @throw std::logic_error 未找到此变量
         */
        virtual Shader& setVec4(const std::string& name, const glm::vec4& vec) = 0;

        /**
         * @brief 设置整数统一变量
         * @param name 变量名
         * @param value  值
         * @return Shader& 此着色器程序引用
         * @warning 需要在绑定此着色器且当前处于此着色器所属的上下文
         * @throw std::logic_error 未找到此变量
         */
        virtual Shader& setInt(const std::string& name, int value) = 0;

        /**
         * @brief 设置浮点数统一向量
         * @param name 变量名
         * @param vec 值
         * @return Shader& 此着色器程序引用
         * @warning 需要在绑定此着色器且当前处于此着色器所属的上下文
         * @throw std::logic_error 未找到此变量
         */
        virtual Shader& setVec3(const std::string& name, const glm::vec3& vec) = 0;

        /**
         * @brief 设置浮点数统一变量
         * @param name 变量名
         * @param value 值
         * @return Shader& 此着色器程序引用
         * @warning 需要在绑定此着色器且当前处于此着色器所属的上下文
         * @throw std::logic_error 未找到此变量
         */
        virtual Shader& setFloat(const std::string& name, float value) = 0;
    };
    using ShaderLock = BindLock<Shader>;

    /**
     * @brief OpenGL着色器
     */
    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string& vertPath, const std::string& fragPath, const std::string& geomPath);
        OpenGLShader(const std::string& vertPath, const std::string& fragPath);
        virtual ~OpenGLShader();
        virtual void bind() override;
        virtual void unbind() override;

        virtual Shader& setMat4(const std::string& name, const glm::mat4& value) override;
        virtual Shader& setVec4(const std::string& name, const glm::vec4& value) override;
        virtual Shader& setVec3(const std::string& name, const glm::vec3& value) override;
        virtual Shader& setInt(const std::string& name, int value) override;
        virtual Shader& setFloat(const std::string& name, float value) override;
    private:

        int findUniformLocation(const std::string& name);
        uint32_t compileShader(const std::string& path, uint32_t type);
        void linkProgram(uint32_t vertShader, uint32_t fragShader, uint32_t geomShader = 0);

    private:

        uint32_t m_shaderID;

        // 着色器的 uniform 变量布局在CPU的缓存（cache）
        std::unordered_map<std::string, int> m_uniformCached;
    };
}