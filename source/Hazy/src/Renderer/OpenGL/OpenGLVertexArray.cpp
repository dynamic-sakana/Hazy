#include <hazy_pch.h>
#include <glad/glad.h>
#include "Hazy/Renderer/VertexArray.h"
#include "assert.h"
#define CALL(x) x; assert(glGetError() == GL_NO_ERROR)
namespace Hazy {

    OpenGLVertexArray::OpenGLVertexArray() {
        CALL(glCreateVertexArrays(1, &m_arrayID));
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        CALL(glDeleteVertexArrays(1, &m_arrayID));
    }

    void OpenGLVertexArray::bind() {
        CALL(glBindVertexArray(m_arrayID));
    }

    void OpenGLVertexArray::unbind() {
        CALL(glBindVertexArray(0));
    }

    VertexArray& OpenGLVertexArray::addVertexBuffer(VertexBuffer& vertexBuffer) {
        glBindVertexArray(m_arrayID);
        VertexBufferLock lock(vertexBuffer);
        uint32_t index = 0;
        const auto& layout = vertexBuffer.getLayout();
        for (const auto& element : layout) {
            CALL(glVertexArrayVertexBuffer(
                m_arrayID,                                     // 本顶点数组对象的 ID
                index,                                         // 第几个属性
                vertexBuffer.getID(),                         // VBO 的 ID
                element.offset,                                // 每一个属性的偏移量
                layout.getStride()                             // 每个顶点的间隔（步长）
            ));
            CALL(glEnableVertexAttribArray(index));             // 启用第几个属性
            index++;
        }
        m_vertexBuffers.push_back(&vertexBuffer);
        return *this;
    }

    VertexArray& OpenGLVertexArray::setIndexBuffer(IndexBuffer& indexBuffer) {
        CALL(glVertexArrayElementBuffer(m_arrayID, indexBuffer.getID()));
        m_indexBuffer = &indexBuffer;
        return *this;
    }

}

#undef CALL