#pragma once

#include "Core.h"
#include "Window.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

#include "Hazel/Core/Timestep.h"

#include <Hazel/Window.h>

#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel
{
    class Application
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

    private:
        std::unique_ptr<Window> _Window;
        ImGuiLayer* _ImGuiLayer;
        bool _Running = true;
        LayerStack _LayerStack;
        float _LastFrameTime = 0.0f;
    private:
        static Application* _Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}