#pragma once

#include "Hazel/Renderer/Texture.h"

#include <glad/glad.h>


namespace Hazel
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(uint32_t width, uint32_t height);
        OpenGLTexture2D(const std::string& path);
        virtual ~OpenGLTexture2D();

        virtual uint32_t GetWidth() const override { return _Width; };
        virtual uint32_t GetHeight() const override { return _Height; };

        virtual void SetData(void* data, uint32_t size) override;

        virtual void Bind(uint32_t slot = 0) const override;

    private:
        std::string _Path;
        uint32_t _Width, _Height;
        uint32_t _RendererId;
        GLenum _InternalFormat, _DataFormat;
    };
}