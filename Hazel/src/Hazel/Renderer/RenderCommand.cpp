#include "hzpch.h"
#include "Hazel/Renderer/RenderCommand.h"

namespace Hazel
{
    Scope<RendererAPI> RenderCommand::_RendererAPI = RendererAPI::Create();
}