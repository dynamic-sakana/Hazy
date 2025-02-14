#include "Hazy/Renderer/Renderer.h"
#include <glad/glad.h>
#include "Hazy/Renderer/Model.h"
#include "Hazy/Renderer/VertexArray.h"
#include "Hazy/Renderer/Shader.h"
#include "Hazy/Renderer/Camera.h"
#include "Hazy/Renderer/Texture.h"
#include "Hazy/Renderer/Light.h"

#include "Hazy/Util/Log.h"

#include "assert.h"
#define CALL(x) x; assert(glGetError() == GL_NO_ERROR)

namespace Hazy {

    OpenGLRenderer::OpenGLRenderer() {
        CALL(glEnable(GL_BLEND));
        CALL(glEnable(GL_DEPTH_TEST));
        CALL(glEnable(GL_CULL_FACE));
        CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }
    
    void OpenGLRenderer::clearColor(glm::vec4 color) {
        if (color != m_clearColor) {
            m_colorDirty = true;
            m_clearColor = color;
        }
    }

    void OpenGLRenderer::clear() {
        if (m_colorDirty) {
            CALL(glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
            m_colorDirty = false;
        }
        CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void OpenGLRenderer::resize(uint32_t width, uint32_t height) {
        assert(width > 0 && height > 0);
        CALL(glViewport(0, 0, width, height));
    }

    Renderer& OpenGLRenderer::beginScene(Camera& camera, PointLight& light) {
        m_camera = &camera;
        m_light = &light;
        return *this;
    }

    void OpenGLRenderer::endScene() {
        
    }

    Renderer& OpenGLRenderer::submit(const Model& model) {
        for (const auto& mesh : model.getMeshes()) {
            BindLock<VertexArray> vaLock(mesh.vertexArray);
            drawCall(*mesh.vertexArray);
        }
        return *this;
    }

    void OpenGLRenderer::drawCall(VertexArray& vertexArray) {
        CALL(glDrawElements(GL_TRIANGLES, vertexArray.getIndexBuffer().getCount(), GL_UNSIGNED_INT, nullptr));
    }

}

#undef CALL