#include "hzpch.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>

namespace Hazel
{
    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
        : _Width(width), _Height(height)
    {
        HZ_PROFILE_FUNCTION();

        _InternalFormat = GL_RGBA8;
        _DataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &_RendererId);
        glTextureStorage2D(_RendererId, 1, _InternalFormat, _Width, _Height);

        glTextureParameteri(_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        : _Path(path)
    {
        HZ_PROFILE_FUNCTION();

        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = nullptr;
        {
            HZ_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string& path) stbi_load");
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }
        HZ_CORE_ASSERT(data, "Failed to load image!");
        _Width = width;
        _Height = height;

        GLenum internalFormat = 0, dataFormat = 0;

        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if(channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        _InternalFormat = internalFormat;
        _DataFormat = dataFormat;

        HZ_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!")

        glCreateTextures(GL_TEXTURE_2D, 1, &_RendererId);
        glTextureStorage2D(_RendererId, 1, internalFormat, _Width, _Height);

        glTextureParameteri(_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(_RendererId, 0, 0, 0, _Width, _Height, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        HZ_PROFILE_FUNCTION();

        glDeleteTextures(1, &_RendererId);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size)
    {
        HZ_PROFILE_FUNCTION();

        uint32_t bitesPerPixel = _DataFormat == GL_RGBA ? 4 : 3;
        HZ_CORE_ASSERT(size == _Width * _Height * bitesPerPixel, "Data must be entire texture!")
        glTextureSubImage2D(_RendererId, 0, 0 ,0, _Width, _Height, _DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        HZ_PROFILE_FUNCTION();

        glBindTextureUnit(slot, _RendererId);
    }
}