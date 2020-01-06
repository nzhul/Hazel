#include <Hazel.h>
#include <Hazel\Input.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), _Camera(-1.6f, 1.6f, -0.9f, 0.9f),
        _CameraPosition(0.0f), _SquarePosition(0.0f)
    {
        _VertexArray.reset(Hazel::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
        Hazel::BufferLayout layout = {
            { Hazel::ShaderDataType::Float3, "a_Position" },
            { Hazel::ShaderDataType::Float4, "a_Color" }
        };
        vertexBuffer->SetLayout(layout);
        _VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = { 0, 1, 2 };
        std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
        indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        _VertexArray->SetIndexBuffer(indexBuffer);

        _SquareVA.reset(Hazel::VertexArray::Create());

        float squareVertices[3 * 4] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
        };

        std::shared_ptr<Hazel::VertexBuffer> squareVB;
        squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
           { Hazel::ShaderDataType::Float3, "a_Position" }
            });
        _SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<Hazel::IndexBuffer> squareIB;
        squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        _SquareVA->SetIndexBuffer(squareIB);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

        _Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));


        /// SQUARE SHADER

        std::string blueShadervertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;

            void main()
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string blueShaderfragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        _BlueShader.reset(new Hazel::Shader(blueShadervertexSrc, blueShaderfragmentSrc));
    }

    void OnAttach() override 
    {
    }

    void OnDetach() override
    {

    }

    void OnUpdate(Hazel::Timestep deltaTime) override
    {
        //if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
        //{
        //    HZ_TRACE("Tab key is pressed!");
        //}

        if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
            _CameraPosition.x -= _CameraMoveSpeed * deltaTime;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
            _CameraPosition.x += _CameraMoveSpeed * deltaTime;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
            _CameraPosition.y += _CameraMoveSpeed * deltaTime;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
            _CameraPosition.y -= _CameraMoveSpeed * deltaTime;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
            _CameraRotation += _CameraRotationSpeed * deltaTime;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
            _CameraRotation -= _CameraRotationSpeed * deltaTime;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_J))
            _SquarePosition.x -= _SquareMoveSpeed * deltaTime;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_L))
            _SquarePosition.x += _SquareMoveSpeed * deltaTime;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_I))
            _SquarePosition.y += _SquareMoveSpeed * deltaTime;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
            _SquarePosition.y -= _SquareMoveSpeed * deltaTime;

        Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Hazel::RenderCommand::Clear();

        _Camera.SetPosition(_CameraPosition);
        _Camera.SetRotation(_CameraRotation);

        Hazel::Renderer::BeginScene(_Camera);

        static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        for(int y = 0; y < 20; y++)
        {
            for(int x = 0; x < 20; x++)
            {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                Hazel::Renderer::Submit(_BlueShader, _SquareVA, transform);
            }
        }

        Hazel::Renderer::Submit(_Shader, _VertexArray);

        Hazel::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
    {
        //ImGui::Begin("Test");
        //ImGui::Text("Hello World");
        //ImGui::End();
    }

    void OnEvent(Hazel::Event& event) override
    {
        //// HZ_TRACE("{0}", event);

        //if (event.GetEventType() == Hazel::EventType::KeyPressed)
        //{
        //    Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
        //    HZ_TRACE("{0}", (char)e.GetKeyCode());
        //}

        //Hazel::EventDispatcher dispatcher(event);
        //dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnkeyPressedEvent));
    }

    //bool OnkeyPressedEvent(Hazel::KeyPressedEvent& event)
    //{
    //    if (event.GetKeyCode() == HZ_KEY_LEFT)
    //        _CameraPosition.x -= _CameraSpeed;

    //    if (event.GetKeyCode() == HZ_KEY_RIGHT)
    //        _CameraPosition.x += _CameraSpeed;

    //    if (event.GetKeyCode() == HZ_KEY_DOWN)
    //        _CameraPosition.y -= _CameraSpeed;

    //    if (event.GetKeyCode() == HZ_KEY_UP)
    //        _CameraPosition.y += _CameraSpeed;

    //    return false;
    //}

private:
    std::shared_ptr<Hazel::Shader> _Shader;
    std::shared_ptr<Hazel::VertexArray> _VertexArray;

    std::shared_ptr<Hazel::Shader> _BlueShader;
    std::shared_ptr<Hazel::VertexArray> _SquareVA;

    Hazel::OrthographicCamera _Camera;
    glm::vec3 _CameraPosition;
    float _CameraMoveSpeed = 5.0f;
    float _CameraRotation = 0.0f;
    float _CameraRotationSpeed = 180.0f;

    glm::vec3 _SquarePosition;
    float _SquareMoveSpeed = 1.0f;
};

class Sandbox : public Hazel::Application {
public:
    Sandbox() 
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox() {

    }
};

Hazel::Application* Hazel::CreateApplication() {
    return new Sandbox();
}