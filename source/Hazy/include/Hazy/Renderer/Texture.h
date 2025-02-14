#pragma once
#include <hazy_pch.h>
#include "Hazy/Enumerates.h"
#include "Hazy/Renderer/Interface.h"

namespace Hazy {
    template <int dimension>
    using TextureWrapVec = glm::vec<dimension, TextureWrap, glm::qualifier::defaultp>;

    /**
     * @brief 纹理类，用于存储纹理数据
     * @tparam dimension 维度，2为2D纹理，3为3D纹理
     * @warning 请勿直接构造一个纹理对象，请使用Context的create模板函数来创建
     */
    template <int dimension = 2>
    class Texture {
    public:
        ~Texture() = default;

        /**
         * @brief 绑定纹理到指定纹理单元，绑定后请一定记得解绑
         * @param slot 纹理单元，默认为0
         * @warning 绑定后请一定记得解绑，注意上下文
         */
        virtual void bind(uint8_t slot = 0) = 0;

        /**
         * @brief 解绑纹理
         * @warning 注意上下文
         */
        virtual void unbind() = 0;

        /**
         * @brief 设置纹理过滤方式
         * @param mag 设置纹理放大过滤方式，默认为线性过滤
         * @param min 设置纹理缩小过滤方式，默认为线性过滤
         * @return 本身的指针
         */
        virtual Texture<dimension>& setFilter(TextureFilter mag, TextureFilter min) = 0;

        /**
         * @brief 设置纹理过滤方式，使用相同过滤方式
         * @param filter 纹理过滤方式
         * @return 本身的指针
         */
        inline Texture<dimension>& setFilter(TextureFilter filter) {
            return setFilter(filter, filter);
        }

        /**
         * @brief 设置纹理环绕方式
         * @param wrap 纹理环绕方式
         * @return 本身的指针
         */
        virtual Texture<dimension>& setWrap(TextureWrapVec<dimension> wrap) = 0;

        /**
         * @brief 设置纹理环绕方式，使用相同环绕方式
         * @param wrap 环绕方式
         * @return 本身的指针
         */
        inline Texture<dimension>& setWrap(TextureWrap wrap) {
            return setWrap(TextureWrapVec<dimension>(wrap));
        }

        /**
         * @brief 生成mipmap（多级渐远纹理）
         * @return 本身的指针
         */
        virtual Texture<dimension>& generateMipMap() = 0;

        /**
         * @brief 获取此纹理的纹理类型
         * @return TextureType 
         */
        virtual TextureType getType() const = 0;
    };
    using Texture2D = Texture<2>;
    using Texture3D = Texture<3>;
    using Texture2DLock = BindLock<Texture2D>;
    using Texture3DLock = BindLock<Texture3D>;

    ////////////////////////////////////////////////////////////////////////////////////////////

    template <int dimension = 2>
    class OpenGLTexture : public Texture<dimension> {
    public:
        
        OpenGLTexture(const std::string& path, TextureType type);
        ~OpenGLTexture();

        virtual void bind(uint8_t slot) override;
        virtual void unbind() override;
        virtual Texture<dimension>& setFilter(TextureFilter mag, TextureFilter min) override;
        virtual Texture<dimension>& setWrap(TextureWrapVec<dimension> wrap) override;
        virtual Texture<dimension>& generateMipMap() override;

        virtual TextureType getType() const override {
            return m_type;
        }

    private:
        uint32_t m_textureID;
        uint8_t m_lastSlot;
        TextureType m_type;
        glm::vec<dimension + 1, int, glm::qualifier::defaultp> m_scale;
    };

    using OpenGLTexture2D = OpenGLTexture<2>;
    using OpenGLTexture3D = OpenGLTexture<3>;
}