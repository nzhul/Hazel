#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel
{
    RendererAPI* RenderCommand::_RendererAPI = new OpenGLRendererAPI;
}