#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"
#include <glad/glad.h>

#include "Input.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::_Instance = nullptr;

    Application::Application()
    {
        HZ_CORE_ASSERT(!_Instance, "Application already exists!");
        _Instance = this;

        _Window = std::unique_ptr<Window>(Window::Create());
        _Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        _ImGuiLayer = new ImGuiLayer();
        PushOverlay(_ImGuiLayer);

        // Vertex Array
        // Vertex Buffer
        // Index Buffer

        glGenVertexArrays(1, &_VertexArray);
        glBindVertexArray(_VertexArray);

        glGenBuffers(1, &_VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _VertexBuffer);

        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f,
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), nullptr);

        glGenBuffers(1, &_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IndexBuffer);

        unsigned int indices[3] = { 0, 1, 2 };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            out vec3 v_Position;

            void main()
            {
                v_Position = a_Position;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
            }
        )";

        _Shader.reset(new Shader(vertexSrc, fragmentSrc));
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
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            _Shader->Bind();
            glBindVertexArray(_VertexArray);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

            for (Layer* layer : _LayerStack)
                layer->OnUpdate();

            _ImGuiLayer->Begin();
            for (Layer* layer : _LayerStack)
                layer->OnImGuiRender();
            _ImGuiLayer->End();

            _Window->OnUpdate();
        }
    }

    bool Application::OnWindowClosed(WindowCloseEvent& e)
    {
        _Running = false;
        return true;
    }
}