#include <glad/glad.h>
#include <hazy_pch.h>
#include "Hazy/Renderer/Buffer.h"
#include "assert.h"

#define CALL(x) x; assert(glGetError() == GL_NO_ERROR)

namespace Hazy {
    
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, VertexBufferLayout layout, BufferUsage)
        : VertexBuffer(layout) {
        glCreateBuffers(1, &m_bufferID);
        glNamedBufferStorage(m_bufferID, size, vertices, GL_DYNAMIC_STORAGE_BIT);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        CALL(glDeleteBuffers(1, &m_bufferID));
    }

    void OpenGLVertexBuffer::bind() {
        CALL(glBindBuffer(GL_ARRAY_BUFFER, m_bufferID));
    }
    
    void OpenGLVertexBuffer::unbind() {
        CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t size, BufferUsage )
        : m_count(size / sizeof(uint32_t)) {
        CALL(glCreateBuffers(1, &m_bufferID));
        CALL(glNamedBufferStorage(m_bufferID, size, indices, GL_DYNAMIC_STORAGE_BIT));
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        CALL(glDeleteBuffers(1, &m_bufferID));
    }

    void OpenGLIndexBuffer::bind() {
        CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID));
    }

    void OpenGLIndexBuffer::unbind() {
        CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

}

#undef CALL