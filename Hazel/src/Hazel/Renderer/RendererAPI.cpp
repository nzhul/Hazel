#include "hzpch.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel
{
    RendererAPI::API RendererAPI::_API = RendererAPI::API::OpenGL;

    Scope<RendererAPI> RendererAPI::Create()
    {
        switch (_API)
        {
            case RendererAPI::API::None:    HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
        }

        HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}