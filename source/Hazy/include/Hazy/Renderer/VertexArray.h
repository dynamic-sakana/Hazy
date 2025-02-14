#pragma once
#include <hazy_pch.h>
#include "Hazy/Renderer/Interface.h"
#include "Hazy/Enumerates.h"
#include "Hazy/Renderer/Buffer.h"
namespace Hazy {

    /**
     * @brief 顶点数组
     * @warning 请勿直接构造一个顶点数组对象，请使用Context的create模板函数来创建
     * @see VertexBuffer
     * @see IndexBuffer
     */
    class HAZY_API VertexArray {
    public:
        virtual ~VertexArray() = default;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual VertexArray& addVertexBuffer(VertexBuffer& vertexBuffer) = 0;
        virtual VertexArray& setIndexBuffer(IndexBuffer& indexBuffer) = 0;
        virtual IndexBuffer& getIndexBuffer() const = 0;
    };
    using VertexArrayLock = BindLock<VertexArray>;

    class HAZY_API OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();
        void bind() override;
        void unbind() override;
        VertexArray& addVertexBuffer(VertexBuffer& vertexBuffer) override;
        VertexArray& setIndexBuffer(IndexBuffer& indexBuffer) override;
        inline IndexBuffer& getIndexBuffer() const override { return *m_indexBuffer; }
    private:
        std::vector<VertexBuffer*> m_vertexBuffers;
        IndexBuffer* m_indexBuffer;
        uint32_t m_arrayID;
    };
    
}