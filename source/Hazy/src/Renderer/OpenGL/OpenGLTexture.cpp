#include <hazy_pch.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD // 禁用 SIMD 优化，因为某些机器上会报错
#include "Hazy/Util/Log.h"
#include <stb_image.h>
#include "Hazy/Renderer/Texture.h"

#include "assert.h"
#define CALL(x) x; assert(glGetError() == GL_NO_ERROR)

namespace Hazy {

    template<>
    OpenGLTexture<2>::OpenGLTexture(const std::string& path, TextureType type)
        : m_lastSlot(0), m_type(type) {
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &m_scale.x, &m_scale.y, &m_scale.z, 0);
        if (data == nullptr) {
            Logger::LogError("Failed to load texture: {0}", path);
            return;
        }

        int openglFormat = 0, pictureFormat = 0;
        if (m_scale.z == 4) {
            openglFormat = GL_RGBA8;
            pictureFormat = GL_RGBA;
        } else if (m_scale.z == 3) {
            openglFormat = GL_RGB8;
            pictureFormat = GL_RGB;
        }

        CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID));
        CALL(glTextureStorage2D(m_textureID, 1, openglFormat, m_scale.x, m_scale.y));

        CALL(glTextureSubImage2D(m_textureID, 0, 0, 0, m_scale.x, m_scale.y, pictureFormat, GL_UNSIGNED_BYTE, data));
        
        stbi_image_free(data);
    }

    template<>
    OpenGLTexture<2>::~OpenGLTexture() {
        CALL(glDeleteTextures(1, &m_textureID));
    }

    template<>
    void OpenGLTexture<2>::bind(uint8_t slot) {
        CALL(glBindTextureUnit(slot, m_textureID));
        m_lastSlot = slot;
    }

    template<>
    void OpenGLTexture<2>::unbind() {
        CALL(glBindTextureUnit(m_lastSlot, 0));
    }

    template<>
    Texture<2>& OpenGLTexture<2>::setFilter(TextureFilter mag, TextureFilter min) {
        CALL(glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, FilterToOpenGL(min)));
        CALL(glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, FilterToOpenGL(mag)));
        return *this;
    }

    template<>
    Texture<2>& OpenGLTexture<2>::setWrap(TextureWrapVec<2> wrap) {
        CALL(glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, WrapToOpenGL(wrap.x)));
        CALL(glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, WrapToOpenGL(wrap.y)));
        return *this;
    }

    template<>
    Texture<2>& OpenGLTexture<2>::generateMipMap() {
        CALL(glGenerateTextureMipmap(m_textureID));
        return *this;
    }

    //////////////////////////////////////////////////////////
    
    template<>
    OpenGLTexture<3>::OpenGLTexture(const std::string& , TextureType ) {
    }

    template<>
    OpenGLTexture<3>::~OpenGLTexture() {
    }

    template<>
    void OpenGLTexture<3>::bind(uint8_t ) {
    }

    template<>
    void OpenGLTexture<3>::unbind() {
    }

    template<>
    Texture<3>& OpenGLTexture<3>::setFilter(TextureFilter, TextureFilter) {
        return *this;
    }

    template<>
    Texture<3>& OpenGLTexture<3>::setWrap(TextureWrapVec<3>) {
        return *this;
    }

    template<>
    Texture<3>& OpenGLTexture<3>::generateMipMap() {
        return *this;
    }

    template class OpenGLTexture<2>;
    template class OpenGLTexture<3>;
    
}

#undef STB_IMAGE_IMPLEMENTATION
#undef CALL