#pragma once

#include "Hazel/Renderer/Buffer.h"

namespace Hazel
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual const BufferLayout& GetLayout() const override
        {
            return _Layout;
        }

        virtual void SetLayout(const BufferLayout& layout) override
        {
            _Layout = layout;
        };

    private:
        uint32_t _RendererId;
        BufferLayout _Layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const;
        virtual void UnBind() const;

        virtual uint32_t GetCount() const { return _Count; };
    private:
        uint32_t _RendererId;
        uint32_t _Count;
    };
}