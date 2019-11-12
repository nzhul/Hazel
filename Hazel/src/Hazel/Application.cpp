#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include <GLFW/glfw3.h>

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application()
    {
        _Window = std::unique_ptr<Window>(Window::Create());
        _Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& e) 
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
        HZ_CORE_TRACE("{0}", e);
    }

    void Application::Run()
    {
        while (_Running)
        {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            _Window->OnUpdate();
        }
    }

    bool Application::OnWindowClosed(WindowCloseEvent& e)
    {
        _Running = false;
        return true;
    }
}