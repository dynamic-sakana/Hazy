#include <hazy_pch.h>
extern "C" {
    #include <glad/glad.h>
    #include <GLFW/glfw3.h>
}
#include "Hazy/Util/Log.h"
#include "Hazy/EventSystem.h"
#include "Hazy/Window.h"
#include "Hazy/Application.h"
#include "Hazy/Renderer/Context.h"
#include "Hazy/Renderer/Shader.h"
#include "assert.h"
#define CALL(x) x; assert(glGetError() == GL_NO_ERROR)

void OpenGLInfoCallback(
    GLenum         source,
    GLenum         type,
    GLuint         id,
    GLenum         severity,
    GLsizei     /* length */,
    const GLchar* message,
    const void* /* userParam */
);

namespace Hazy {

    std::once_flag OpenGLContext::s_GLADInitialized;
    std::once_flag OpenGLContext::s_GLFWInitialized;

    OpenGLContext::OpenGLContext(Window* window, const std::string& windowName, int width, int height, GLFWmonitor* monitor, GLFWwindow* share) : Context(window) {
        m_userPointerPair = std::make_pair(this, nullptr);
        GLFWInit();

        m_nativeWindow = glfwCreateWindow(width, height, windowName.c_str(), monitor, share);
        if (m_nativeWindow == nullptr) {
            Logger::LogCritical("Failed to create GLFW window, error code: {}", glfwGetError(nullptr));
            std::exit(EXIT_FAILURE);
        }

        glfwSetWindowUserPointer(m_nativeWindow, &m_userPointerPair);

        {
            ContextLock lock(*this);
            GLADInit();
        }

        RegisterCallbacks();
    }

    OpenGLContext::~OpenGLContext() {
        GLFWwindow* currentContext = glfwGetCurrentContext();
        {
            glfwMakeContextCurrent(m_nativeWindow);
            library.vertexBuffers.clear();
            library.indexBuffers.clear();
            library.meshes.clear();
            library.models.clear();
            library.shaders.clear();
            library.texture2Ds.clear();
            library.texture3Ds.clear();
            library.vertexArrays.clear();
        }
        glfwMakeContextCurrent(currentContext);
        glfwDestroyWindow(m_nativeWindow);
    }

    void OpenGLContext::SwapBuffers() {
        glfwSwapBuffers(m_nativeWindow);
        glfwPollEvents();
    }

    void OpenGLContext::enableVSync(bool enabled) {
        ContextLock lock(*this);
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        m_isVSync = enabled;
    }

    bool OpenGLContext::isVSync() const {
        return m_isVSync;
    }

    void OpenGLContext::bind() {
        glfwMakeContextCurrent(m_nativeWindow);
    }

    void OpenGLContext::unbind() {
        glfwMakeContextCurrent(nullptr);
    }

    KeyAction OpenGLContext::getKeyState(Key key) {
        auto state = glfwGetKey(m_nativeWindow, KeyToGLFW(key));
        switch (state)
        {
        case GLFW_PRESS:
            return KeyAction::Press;
        case GLFW_REPEAT:
            return KeyAction::Repeat;
        case GLFW_RELEASE:
            return KeyAction::Release;
        default:
            return KeyAction::Release;
        }
    }

    MouseButtonAction OpenGLContext::getMouseButtonState(MouseButton button) {
        auto state = glfwGetMouseButton(m_nativeWindow, MouseButtonToGLFW(button));
        switch (state) {
        case GLFW_PRESS:
            return MouseButtonAction::Press;
        case GLFW_RELEASE:
            return MouseButtonAction::Release;
        default:
            return MouseButtonAction::Release;
        }
    }

    glm::vec2 OpenGLContext::getMousePosition() {
        double x, y;
        glfwGetCursorPos(m_nativeWindow, &x, &y);
        x = (x > 0.0f) ? x : 0.0f;
        x = (x < m_window->getWidth()) ? x : m_window->getWidth();
        y = (y > 0.0f) ? y : 0.0f;
        y = (y < m_window->getHeight()) ? y : m_window->getHeight();
        return { static_cast<float>(x), static_cast<float>(y) };
    }

    void OpenGLContext::GLFWInit() {
        std::call_once(s_GLFWInitialized,
            []() {
                if (glfwInit() == GLFW_TRUE) {
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
                    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

                    Logger::LogInfo("GLFW initialized");
                    glfwSetErrorCallback(
                        [](int error, const char* description) {
                            Logger::LogError("GLFW error: code: {0} description: {1}", error, description);
                        });
                }
                else {
                    Logger::LogCritical("Failed to initialize GLFW, error code: {}", glfwGetError(nullptr));
                    std::exit(EXIT_FAILURE);
                }
                Application::addShutDownHook(
                    [] {
                        glfwTerminate();
                        Logger::LogInfo("GLFW terminated");
                    });
            });
    }

    void OpenGLContext::GLADInit() {
        std::call_once(s_GLADInitialized,
            []() {
                if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                    Logger::LogWarn("API::OpenGL initialized, following information is detected:");
                    Logger::LogWarn("|>  renderer   {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
                    Logger::LogWarn("|>  version    {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
                    Logger::LogWarn("|>  vendor     {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
                    glDebugMessageCallback(&::OpenGLInfoCallback, nullptr);
                    CALL(glEnable(GL_DEBUG_OUTPUT));
                }
                else {
                    Logger::LogCritical("Failed to initialize API::OpenGL");
                    std::exit(EXIT_FAILURE);
                }
            });
    }

    void OpenGLContext::RegisterCallbacks() {
        glfwSetWindowCloseCallback(m_nativeWindow,
            [](GLFWwindow* window) {
                Context* thisContext = static_cast<UserPointerPair*>(glfwGetWindowUserPointer(window))->first;
                thisContext->callback.whenWindowClosed(thisContext->getWindow());
            });

        glfwSetWindowSizeCallback(m_nativeWindow,
            [](GLFWwindow* window, int width, int height) {
                Context* thisContext = static_cast<UserPointerPair*>(glfwGetWindowUserPointer(window))->first;
                thisContext->callback.whenWindowResized(thisContext->getWindow(), width, height);
            });

        glfwSetWindowPosCallback(m_nativeWindow,
            [](GLFWwindow* window, int x, int y) {
                Context* thisContext = static_cast<UserPointerPair*>(glfwGetWindowUserPointer(window))->first;
                thisContext->callback.whenWindowMoved(thisContext->getWindow(), x, y);
            });

        glfwSetWindowFocusCallback(m_nativeWindow,
            [](GLFWwindow* window, int focused) {
                Context* thisContext = static_cast<UserPointerPair*>(glfwGetWindowUserPointer(window))->first;
                thisContext->callback.whenWindowFocusChanged(thisContext->getWindow(), focused == GLFW_TRUE);
            });

        glfwSetKeyCallback(m_nativeWindow,
            [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                Context* thisContext = static_cast<UserPointerPair*>(glfwGetWindowUserPointer(window))->first;
                thisContext->callback.whenKeyTriggered(thisContext->getWindow(), GLFWToKey(key), scancode, GLFWToKeyAction(action), GLFWToModifierKey(mods));
            });

        glfwSetCursorPosCallback(m_nativeWindow,
            [](GLFWwindow* window, double x, double y) {
                Context* thisContext = static_cast<UserPointerPair*>(glfwGetWindowUserPointer(window))->first;
                thisContext->callback.whenMouseMoved(thisContext->getWindow(), x, y);
            });

        glfwSetMouseButtonCallback(m_nativeWindow,
            [](GLFWwindow* window, int button, int action, int mods) {
                Context* thisContext = static_cast<UserPointerPair*>(glfwGetWindowUserPointer(window))->first;
                thisContext->callback.whenMouseClicked(thisContext->getWindow(), GLFWToMouseButton(button), GLFWToMouseButtonAction(action), GLFWToModifierKey(mods));
            });

        glfwSetScrollCallback(m_nativeWindow,
            [](GLFWwindow* window, double x, double y) {
                Context* thisContext = static_cast<UserPointerPair*>(glfwGetWindowUserPointer(window))->first;
                thisContext->callback.whenMouseScrolled(thisContext->getWindow(), x, y);
            });
    }

}

void OpenGLInfoCallback(
    GLenum         source,
    GLenum         type,
    GLuint         id,
    GLenum         severity,
    GLsizei     /* length */,
    const GLchar*  message,
    const void* /* userParam */
) {
    std::string sourceStr;
    std::string typeStr;
    std::string idStr = std::to_string(id);

    switch (source) {
    case GL_DEBUG_SOURCE_API:               sourceStr = "OpenGL API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     sourceStr = "Window system"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:   sourceStr = "Shader compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:       sourceStr = "Third party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:       sourceStr = "Application"; break;
    case GL_DEBUG_SOURCE_OTHER:             sourceStr = "other"; break;
    default:                                sourceStr = "unknown"; break;
    }
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:               typeStr = "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated behavior"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeStr = "Undefined behavior"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         typeStr = "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         typeStr = "Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              typeStr = "Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          typeStr = "Push group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           typeStr = "Pop group"; break;
    case GL_DEBUG_TYPE_OTHER:               typeStr = "Other"; break;
    default:                                typeStr = "unknown"; break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        Hazy::Logger::LogCritical("[{0}] {1}({2}): {3}", sourceStr, typeStr, idStr, message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        Hazy::Logger::LogError   ("[{0}] {1}({2}): {3}", sourceStr, typeStr, idStr, message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        Hazy::Logger::LogWarn    ("[{0}] {1}({2}): {3}", sourceStr, typeStr, idStr, message);
        break;
    }
}

#undef CALL