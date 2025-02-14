#pragma once
#include <hazy_pch.h>

namespace Hazy {
    class VertexArray;
    class Shader;
    class Camera;
    struct PointLight;
    class Mesh;
    class Model;
    struct Renderable;

    enum setting : uint8_t {
        DepthTest,    // 开启深度测试
        DebugOutput,  // 开启调试输出
        CullFace,     // 开启面剔除
        Blend,        // 开启混合
        DepthWrite,   // 开启深度写入
        StencilTest,  // 开启模板测试
        StencilWrite, // 开启模板写入

        Count
    };

    /**
     * @brief 渲染器
     */
    class HAZY_API Renderer {
    public:

        static std::string s_viewProjectMatrixName;        // viewProj矩阵在着色器中的名字，尽量小于15个字符以确保触发小字符串优化
        static std::string s_modelMatrixName;              // model矩阵在着色器中的名字，尽量小于15个字符以确保触发小字符串优化
        static std::string s_texture2DSamplerPrefix;       // texture2D在着色器中的名字前缀，尽量小于13个字符以确保触发小字符串优化
        static std::string s_texture3DSamplerPrefix;       // texture3D在着色器中的名字前缀，尽量小于15个字符以确保触发小字符串优化
        static std::string s_lightColorName;               // 光源颜色在着色器中的名字，尽量小于15个字符以确保触发小字符串优化
        static std::string s_lightPositionName;            // 光源位置在着色器中的名字，尽量小于15个字符以确保触发小字符串优化
        
    public:
        virtual ~Renderer() = default;
        /**
         * @brief 清除颜色，不需要渲染上下文
         * @param color 设置的清屏颜色
         */
        virtual void clearColor(glm::vec4 color) = 0;

        /**
         * @brief 使用设置的颜色进行清屏，需要渲染上下文
         */
        virtual void clear() = 0;

        /**
         * @brief 调整窗口大小，需要渲染上下文
         * @param width 宽度
         * @param height 高度
         */
        virtual void resize(uint32_t width, uint32_t height) = 0;

        /**
         * @brief 开始一个渲染场景，往渲染队列中添加元素，不需要渲染上下文
         * @return 本身的引用，便于链式调用
         */
        virtual Renderer& beginScene(Camera& camera, PointLight& light) = 0;

        /**
         * @brief 结束一个渲染场景，开始绘制，需要渲染上下文
         */
        virtual void endScene() = 0;

        /**
         * @brief 提交一个模型到渲染队列，不需要渲染上下文
         * @param model 要提交的模型
         * @return 本身的引用，便于链式调用
         */
        virtual Renderer& submit(const Model& model) = 0;

        virtual void drawCall(VertexArray& vertexArray) = 0;

        /**
         * @brief 创建一个渲染器，不需要渲染上下文
         * @tparam API 指定渲染器API
         * @return Ref<Renderer> 创建出来的渲染器
         */
        template <API api>
        inline static Ref<Renderer> create();


    protected:

        Camera* m_camera = nullptr;
        PointLight* m_light = nullptr;
    };

    /**
     * @brief 结束渲染场景的标识符
     */
    struct EndScene { };
    constexpr EndScene endScene {};

    /**
     * @brief 重载运算符，使得渲染器可以直接使用<<操作符提交渲染对象
     * @param renderer 渲染器
     * @param model 要提交的模型
     * @return Renderer& 本渲染器
     */
    inline Renderer& operator<<(Renderer& renderer, Model& model) {
        return renderer.submit(model);
    }

    /**
     * @brief 结束一个渲染场景，开始绘制，需要渲染上下文
     * @param renderer 渲染器
     * @param endScene 占位参数
     */
    inline void operator<<(Renderer& renderer, const EndScene&) {
        renderer.endScene();
    }

    class HAZY_API OpenGLRenderer : public Renderer {
    public:
        
        OpenGLRenderer();
        virtual ~OpenGLRenderer() = default;
        virtual void clearColor(glm::vec4 color) override;
        virtual void clear() override;
        virtual void resize(uint32_t width, uint32_t height) override;
        virtual void drawCall(VertexArray& vertexArray) override;
        virtual Renderer& beginScene(Camera& camera, PointLight& light) override;
        virtual void endScene() override;
        virtual Renderer& submit(const Model& model) override;

    protected:
        
        bool m_colorDirty = false;
        glm::vec4 m_clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

        std::queue<Model*> m_models;
    };

    template <API api>
    Ref<Renderer> Renderer::create() {
        if constexpr (api == API::OpenGL) return Ref<Renderer>(new OpenGLRenderer());
        else static_assert(false, "Unsupported API type");
    }
}