#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"
#include <glad/glad.h>

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::_Instance = nullptr;

    Application::Application()
    {
        HZ_CORE_ASSERT(!_Instance, "Application already exists!");
        _Instance = this;

        _Window = std::unique_ptr<Window>(Window::Create());
        _Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }

    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        _LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        _LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e) 
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));

        for (auto it = _LayerStack.end(); it != _LayerStack.begin(); )
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while (_Running)
        {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer : _LayerStack)
                layer->OnUpdate();

            _Window->OnUpdate();
        }
    }

    bool Application::OnWindowClosed(WindowCloseEvent& e)
    {
        _Running = false;
        return true;
    }
}