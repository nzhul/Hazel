#pragma once

#include "Core.h"
#include "Window.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

#include <Hazel/Window.h>

#include "Hazel/ImGui/ImGuiLayer.h"

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"

namespace Hazel
{
    class HAZEL_API Application
    {
    public:
        Application();
        virtual ~Application() = default;

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline static Application& Get() { return *_Instance; }
        inline Window& GetWindow() { return *_Window; }
    private:
        bool OnWindowClosed(WindowCloseEvent& e);

        std::unique_ptr<Window> _Window;
        ImGuiLayer* _ImGuiLayer;
        bool _Running = true;
        LayerStack _LayerStack;

        unsigned int _VertexArray;
        std::unique_ptr<Shader> _Shader;
        std::unique_ptr<VertexBuffer> _VertexBuffer;
        std::unique_ptr<IndexBuffer> _IndexBuffer;

    private:
        static Application* _Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}