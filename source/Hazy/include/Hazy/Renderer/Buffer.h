#pragma once
#include <hazy_pch.h>
#include "Hazy/Renderer/Interface.h"
#include "Hazy/Enumerates.h"
namespace Hazy {

    struct HAZY_API VertexElement {
        DataType type;      // 数据类型
        uint32_t count;     // 向量的维度个数
        std::string name;   // 元素名字
        bool normalized;    // 是否归一化
        uint32_t offset;    // 偏移量（自动计算，不需要设置，单位为字节）

        VertexElement(DataType type, uint32_t count, std::string&& name, bool normalized = false)
            : type(type), count(count), name(std::move(name)), normalized(normalized), offset(0) { }
    };

    class HAZY_API VertexBufferLayout {
    public:
        VertexBufferLayout() { }
        VertexBufferLayout(const std::initializer_list<VertexElement>& elements)
            : m_elements(elements), m_stride(0) {
            for (auto& element : m_elements) {
                element.offset = m_stride;
                m_stride += element.count * DataTypeSize(element.type);
            }
        }

        inline std::vector<VertexElement>::iterator begin() { return m_elements.begin(); }
        inline std::vector<VertexElement>::iterator end() { return m_elements.end(); }

        inline std::vector<VertexElement>::const_iterator begin() const { return m_elements.begin(); }
        inline std::vector<VertexElement>::const_iterator end() const { return m_elements.end(); }

        inline VertexBufferLayout& addElement(const VertexElement& element) {
            m_elements.push_back(element);
            m_elements.back().offset = m_stride;
            m_stride += element.count * DataTypeSize(element.type);
            return *this;
        }

        inline uint32_t getOffsetOf(const std::string& name) {
            for (auto& element : m_elements) {
                if (element.name == name) {
                    return element.offset;
                }
            }
            throw std::logic_error("Vertex element not found! Check your spelling and capitalization.");
        }

        inline uint32_t getStride() const { return m_stride; }
        inline const std::vector<VertexElement>& getElements() const { return m_elements; }
        inline int getElementCount() const { return (int)m_elements.size(); }

    private:
        std::vector<VertexElement> m_elements; // 元素
        uint32_t m_stride = 0;                 // 步长
    };

    /**
     * @brief vertex buffer基类，顶点缓冲
     * @note 构造函数参数：float* vertices, uint32_t size, VertexBufferLayout layout, BufferUsage usage
     * @warning 请勿直接构造一个顶点缓冲对象，请使用Context的create模板函数来创建
     */
    class HAZY_API VertexBuffer {
    public:
        VertexBuffer(VertexBufferLayout layout) : m_layout(layout) { }
        virtual ~VertexBuffer() = default;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual uint32_t getID() const = 0;
        inline const VertexBufferLayout& getLayout() const { return m_layout; }
    protected:
        VertexBufferLayout m_layout;
    };
    using VertexBufferLock = BindLock<VertexBuffer>;

    /** 
     * @brief index buffer基类，索引缓冲
     * @note 构造函数参数：uint32_t* indices, uint32_t size, BufferUsage usage
     * @warning 请勿直接构造一个索引缓冲对象，请使用Context的create模板函数来创建
     */
    class HAZY_API IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual uint32_t getID() const = 0;
        /**
         * @brief 获取索引个数
         * @return uint32_t 索引个数
         */
        virtual uint32_t getCount() const = 0;
    };
    using IndexBufferLock = BindLock<IndexBuffer>;


    /**
     * @brief Vertex buffer的OpenGL实现
     */
    class HAZY_API OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size, VertexBufferLayout layout, BufferUsage usage = BufferUsage::StaticDraw);
        ~OpenGLVertexBuffer();

        virtual void bind() override;
        virtual void unbind() override;
        inline virtual uint32_t getID() const override { return m_bufferID; }
    private:
        uint32_t m_bufferID;
    };

    /**
     * @brief Index buffer 的OpenGL实现
     */
    class HAZY_API OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t size, BufferUsage usage = BufferUsage::StaticDraw);
        ~OpenGLIndexBuffer();

        virtual void bind() override;
        virtual void unbind() override;
        inline virtual uint32_t getID() const override { return m_bufferID; }
        virtual inline uint32_t getCount() const override { return m_count; }
    private:
        uint32_t m_bufferID;
        uint32_t m_count;
    };

}