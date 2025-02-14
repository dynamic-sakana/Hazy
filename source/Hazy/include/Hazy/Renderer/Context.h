#pragma once
#include <hazy_pch.h>

#include "Hazy/Enumerates.h"
#include "Hazy/Renderer/Buffer.h"
#include "Hazy/Renderer/Interface.h"
#include "Hazy/Renderer/Model.h"
#include "Hazy/Renderer/Shader.h"
#include "Hazy/Renderer/Texture.h"
#include "Hazy/Renderer/VertexArray.h"
#include "Hazy/Renderer/Renderer.h"

extern "C" {
    struct GLFWwindow;
    struct GLFWmonitor;
}

namespace Hazy {

    class Window;

    /**
     * @brief 渲染上下文的接口，用于管理渲染相关的资源
     * @note - 在获取资源的时候应该向上下文申请，而不是自己手动创建，这样可以实现资源复用和资源自动清理
     * @note - 不同上下文实例之间的资源是不共享的
     */
    class Context {
        struct Callback {
            std::function<void(Window*)> whenWindowClosed;
            std::function<void(Window*, int, int)> whenWindowResized;
            std::function<void(Window*, int, int)> whenWindowMoved;
            std::function<void(Window*, bool)> whenWindowFocusChanged;

            std::function<void(Window*, Key, int , KeyAction, ModifierKey)> whenKeyTriggered;

            std::function<void(Window*, float, float)> whenMouseMoved;
            std::function<void(Window*, MouseButton, MouseButtonAction, ModifierKey)> whenMouseClicked;
            std::function<void(Window*, float, float)> whenMouseScrolled;
        };
        struct Library {
            std::unordered_map<std::string, Ref<VertexBuffer>> vertexBuffers;
            std::unordered_map<std::string, Ref<IndexBuffer>> indexBuffers;
            std::unordered_map<std::string, Ref<Mesh>> meshes;
            std::unordered_map<std::string, Ref<Model>> models;
            std::unordered_map<std::string, Ref<Shader>> shaders;
            std::unordered_map<std::string, Ref<Texture2D>> texture2Ds;
            std::unordered_map<std::string, Ref<Texture3D>> texture3Ds;
            std::unordered_map<std::string, Ref<VertexArray>> vertexArrays;
        };
    public:
        Context(Window* window) : m_window(window) { }
        virtual ~Context() { }
        virtual void SwapBuffers() = 0;

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual inline void setUserPointer(void* userPointer) { m_userPointerPair.second = userPointer; }
        virtual inline void* getUserPointer() { return m_userPointerPair.second; }
        virtual inline Window* getWindow() { return m_window; }

        // 获取原生窗口
        virtual void* getNativeWindow() = 0;

        virtual void enableVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;

        virtual KeyAction getKeyState(Key key) = 0;
        virtual MouseButtonAction getMouseButtonState(MouseButton button) = 0;
        virtual glm::vec2 getMousePosition() = 0;

        /**
         * @brief 创建渲染上下文
         * @tparam API 渲染上下文API
         * @tparam Args 对应的构造函数参数
         * @param args 构造函数参数
         * @return Ref<Content> 构造出来的渲染上下文
         */
        template <API api, class... Args>
        inline static Ref<Context> create(Args&&... args);

        /**
         * @brief 创建一个T类型的GPU资源
         * @tparam T GPU资源类型
         * @tparam Args 创建这个GPU资源所需的参数类型
         * @param name 这个GPU资源的名字，请牢记，这个名字将用于后续的获取
         * @param args 创建这个GPU资源所需的参数
         * @return T& 创建出来的GPU资源的引用
         * @warning - 创建GPU资源的时候请确保此上下文已经被绑定，否则将有可能出现 Segmentation Fault
         * @warning - 相同类型的GPU资源的名字不能重复，但是不同类型的资源可以重复
         * @warning - 请不要使用delete强行删除GPU资源，否则未来使用这个GPU资源的时候会出现 Segmentation Fault
         * @note - 最好将名字长度限制在15个字符以内以确保触发小字符串优化（不同编译器的触发阈值不一样），以加快查找速度
         * @note - 不同类型的GPU资源有不同的创建参数，请参考对应的GPU资源类构造函数
         * @throws std::logic_error 已经有另一个相同类型的名字为name的资源
         */
        template <class T, class... Args>
        inline T& create(std::string name, Args&&... args);

        /**
         * @brief 获取一个已经创建出来的T类型的GPU资源
         * @tparam T GPU资源类型
         * @param name GPU资源的名字
         * @return T& 找到的GPU资源的引用
         * @warning - 请在查找对应的GPU资源之前，先调用create函数创建这个GPU资源，否则将抛出异常
         * @warning - 请不要使用delete强行删除GPU资源，否则未来使用这个GPU资源的时候会出现 Segmentation Fault
         * @throws std::out_of_range 找不到名字为name的GPU资源，可能你没创建过这个GPU资源，或者名字写错了，或者传了错误的模板参数
         */
        template <class T>
        inline T& get(const std::string& name);

        Callback callback;

    protected:
        virtual VertexBuffer& createVertexBuffer(
            const std::string& name,
            float* vertices,
            uint32_t size,
            VertexBufferLayout layout,
            BufferUsage usage) = 0;
        
        virtual IndexBuffer& createIndexBuffer(
            const std::string& name,
            uint32_t* indices,
            uint32_t size,
            BufferUsage usage) = 0;
        
        virtual Mesh& createMesh(
            const std::string& name,
            VertexArray& vertexArray,
            const Material& material) = 0;

        virtual Model& createModel(
            const std::string& name,
            const std::string& path) = 0;

        virtual Shader& createShader(
            const std::string& name,
            const std::string& vertPath,
            const std::string& fragPath,
            const std::string& geomPath) = 0;

        virtual Shader& createShader(
            const std::string& name,
            const std::string& vertPath,
            const std::string& fragPath) = 0;

        virtual Texture2D& createTexture2D(
            const std::string& name,
            const std::string& path,
            TextureType type = TextureType::Diffuse) = 0;

        virtual Texture3D& createTexture3D(
            const std::string& name,
            const std::string& path,
            TextureType type = TextureType::Diffuse) = 0;

        virtual VertexArray& createVertexArray(const std::string& name) = 0;

        Library library;

        /**
         * @brief 用户指针，第一个位置存储上下文，第二个位置存储用户自定义数据
         */
        using UserPointerPair = std::pair<Context*, void*>;
        UserPointerPair m_userPointerPair;
        bool m_isVSync = true;
        Window* m_window;
    };
    using ContextLock = BindLock<Context>;

    /**
     * @brief OpenGL渲染上下文
     */
    class OpenGLContext : public Context {
    public:
        OpenGLContext(Window* window, const std::string& windowName, int width,
            int height, GLFWmonitor* monitor = nullptr,
            GLFWwindow* share = nullptr);
        virtual ~OpenGLContext();
        virtual void SwapBuffers() override;

        virtual void* getNativeWindow() override { return m_nativeWindow; }

        virtual void enableVSync(bool enabled) override;
        virtual bool isVSync() const override;

        virtual void bind() override;
        virtual void unbind() override;

        virtual KeyAction getKeyState(Key key) override;
        virtual MouseButtonAction getMouseButtonState(MouseButton button) override;
        virtual glm::vec2 getMousePosition() override;
    private:
        inline virtual VertexBuffer& createVertexBuffer(
            const std::string& name,
            float* vertices,
            uint32_t size,
            VertexBufferLayout layout,
            BufferUsage usage
        ) override {
            if (library.vertexBuffers.find(name) == library.vertexBuffers.end()) {
                library.vertexBuffers.emplace(name, new OpenGLVertexBuffer(vertices, size, layout, usage));
                return *library.vertexBuffers.at(name);
            }
            throw std::logic_error("VertexBuffer already exists");
        }

        inline virtual IndexBuffer& createIndexBuffer(
            const std::string& name,
            uint32_t* indices,
            uint32_t size,
            BufferUsage usage
        ) override {
            if (library.indexBuffers.find(name) == library.indexBuffers.end()) {
                library.indexBuffers.emplace(name, new OpenGLIndexBuffer(indices, size, usage));
                return *library.indexBuffers.at(name);
            }
            throw std::logic_error("IndexBuffer already exists");
        }

        inline virtual Mesh& createMesh(
            const std::string& name,
            VertexArray& vertexArray,
            const Material& material
        ) override {
            if (library.meshes.find(name) == library.meshes.end()) {
                library.meshes.emplace(name, new Mesh(vertexArray, material));
                return *library.meshes.at(name);
            }
            throw std::logic_error("Mesh already exists");
        }

        inline virtual Model& createModel(
            const std::string& name,
            const std::string& path
        ) override {
            if (library.models.find(name) == library.models.end()) {
                library.models.emplace(name, new Model(*this, name, path));
                return *library.models.at(name);
            }
            throw std::logic_error("Mesh already exists");
        }

        inline virtual Shader& createShader(
            const std::string& name,
            const std::string& vertPath,
            const std::string& fragPath,
            const std::string& geomPath = ""
        ) override {
            if (library.shaders.find(name) == library.shaders.end()) {
                library.shaders.emplace(name, new OpenGLShader(vertPath, fragPath, geomPath));
                return *library.shaders.at(name);
            }
            throw std::logic_error("Shader already exists");
        }

        inline virtual Shader& createShader(
            const std::string& name,
            const std::string& vertPath,
            const std::string& fragPath
        ) override {
            if (library.shaders.find(name) == library.shaders.end()) {
                library.shaders.emplace(name, new OpenGLShader(vertPath, fragPath));
                return *library.shaders.at(name);
            }
            throw std::logic_error("Shader already exists");
        }

        inline virtual Texture2D& createTexture2D(
            const std::string& name,
            const std::string& path,
            TextureType type = TextureType::Diffuse
        ) override {
            if (library.texture2Ds.find(name) == library.texture2Ds.end()) {
                library.texture2Ds.emplace(name, new OpenGLTexture2D(path, type));
                return *library.texture2Ds.at(name);
            }
            throw std::logic_error("Texture already exists");
        }

        inline virtual Texture3D& createTexture3D(
            const std::string& name,
            const std::string& path,
            TextureType type = TextureType::Diffuse
        ) override {
            if (library.texture3Ds.find(name) == library.texture3Ds.end()) {
                library.texture3Ds.emplace(name, new OpenGLTexture3D(path, type));
                return *library.texture3Ds.at(name);
            }
            throw std::logic_error("Texture already exists");
        }

        inline virtual VertexArray& createVertexArray(
            const std::string& name
        ) override {
            if (library.vertexArrays.find(name) == library.vertexArrays.end()) {
                library.vertexArrays.emplace(name, new OpenGLVertexArray());
                return *library.vertexArrays.at(name);
            }
            throw std::logic_error("VertexArray already exists");
        }

        void GLFWInit();
        void GLADInit();
        void RegisterCallbacks();

        GLFWwindow* m_nativeWindow;

        static std::once_flag s_GLADInitialized;
        static std::once_flag s_GLFWInitialized;
    };

    /**
     * @brief 创建一个上下文
     * @tparam api 上下文API类型
     * @tparam Args 创建这个上下文所需的参数类型
     * @param args 创建这个上下文所需的参数
     * @return Ref<Context> 创建出来的上下文的智能指针
     */
    template <API api, class... Args>
    Ref<Context> Context::create(Args&&... args) {
        if constexpr (api == API::OpenGL)
            return Ref<Context>(new OpenGLContext(std::forward<Args>(args)...));
        else
            static_assert(false, "Unknown API type");
    }

    /**
     * @brief 创建一个T类型的GPU资源
     * @tparam T GPU资源类型
     * @tparam Args 创建这个GPU资源所需的参数类型
     * @param name 这个GPU资源的名字，请牢记，这个名字将用于后续的获取
     * @param args 创建这个GPU资源所需的参数
     * @return T& 创建出来的GPU资源的引用
     * @warning - 创建GPU资源的时候请确保此上下文已经被绑定，否则将有可能出现 Segmentation Fault
     * @warning - 相同类型的GPU资源的名字不能重复，但是不同类型的资源可以重复
     * @warning - 请不要使用delete强行删除GPU资源，否则未来使用这个GPU资源的时候会出现 Segmentation Fault
     * @note - 最好将名字长度限制在15个字符以内以确保触发小字符串优化（不同编译器的触发阈值不一样），以加快查找速度
     * @note - 不同类型的GPU资源有不同的创建参数，请参考对应的GPU资源类构造函数
     * @throws std::logic_error 已经有另一个相同类型的名字为name的资源
     */
    template <class T, class... Args>
    T& Context::create(std::string name, Args&&... args) {
        if constexpr (std::is_same_v<T, Context>)
            static_assert(false,
                "Cannot get a Context by this method. Use Context::create<API>(...) instead");
        else if constexpr (std::is_same_v<T, VertexBuffer>)     return createVertexBuffer(name, std::forward<Args>(args)...);
        else if constexpr (std::is_same_v<T, IndexBuffer>)      return createIndexBuffer(name, std::forward<Args>(args)...);
        else if constexpr (std::is_same_v<T, Mesh>)             return createMesh(name, std::forward<Args>(args)...);
        else if constexpr (std::is_same_v<T, Model>)            return createModel(name, std::forward<Args>(args)...);
        else if constexpr (std::is_same_v<T, Shader>)           return createShader(name, std::forward<Args>(args)...);
        else if constexpr (std::is_same_v<T, Texture2D>)        return createTexture2D(name, std::forward<Args>(args)...);
        else if constexpr (std::is_same_v<T, Texture3D>)        return createTexture3D(name, std::forward<Args>(args)...);
        else if constexpr (std::is_same_v<T, VertexArray>)      return createVertexArray(name, std::forward<Args>(args)...);
        else static_assert(false, "Unknown resource type");
    }

    /**
     * @brief 获取一个已经创建出来的T类型的GPU资源
     * @tparam T GPU资源类型
     * @param name GPU资源的名字
     * @return T& 找到的GPU资源的引用
     * @warning - 请在查找对应的GPU资源之前，先调用create函数创建这个GPU资源，否则将抛出异常
     * @warning - 请不要使用delete强行删除GPU资源，否则未来使用这个GPU资源的时候会出现 Segmentation Fault
     * @throws std::out_of_range 找不到名字为name的GPU资源，可能你没创建过这个GPU资源，或者名字写错了，或者传了错误的模板参数
     */
    template <class T>
    T& Context::get(const std::string& name) {
        if constexpr (std::is_same_v<T, Context>)
            static_assert(false,
                "Cannot get a Context by this method. Use Context::create<API>(...) instead");
        else if constexpr (std::is_same_v<T, VertexBuffer>)     return *library.vertexBuffers.at(name);
        else if constexpr (std::is_same_v<T, IndexBuffer>)      return *library.indexBuffers.at(name);
        else if constexpr (std::is_same_v<T, Mesh>)             return *library.meshes.at(name);
        else if constexpr (std::is_same_v<T, Shader>)           return *library.shaders.at(name);
        else if constexpr (std::is_same_v<T, Texture2D>)        return *library.texture2Ds.at(name);
        else if constexpr (std::is_same_v<T, Texture3D>)        return *library.texture3Ds.at(name);
        else if constexpr (std::is_same_v<T, VertexArray>)      return *library.vertexArrays.at(name);
        else static_assert(false, "Unknown type");
    }

}