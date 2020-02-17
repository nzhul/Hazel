//#include <Hazel.h>
//#include <Hazel/Core/EntryPoint.h>
//
//#include <imgui/imgui.h>
//
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "Sandbox2D.h"
//
//class ExampleLayer : public Hazel::Layer
//{
//public:
//    ExampleLayer()
//        : Layer("Example"), _CameraController(1280.0f / 720.0f, true)
//    {
//        _VertexArray = Hazel::VertexArray::Create();
//
//        float vertices[3 * 7] = {
//            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
//             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
//             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
//        };
//
//        Hazel::Ref<Hazel::VertexBuffer> vertexBuffer = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));
//        Hazel::BufferLayout layout = {
//            { Hazel::ShaderDataType::Float3, "a_Position" },
//            { Hazel::ShaderDataType::Float4, "a_Color" }
//        };
//        vertexBuffer->SetLayout(layout);
//        _VertexArray->AddVertexBuffer(vertexBuffer);
//
//        uint32_t indices[3] = { 0, 1, 2 };
//        Hazel::Ref<Hazel::IndexBuffer> indexBuffer = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
//        _VertexArray->SetIndexBuffer(indexBuffer);
//
//        _SquareVA = Hazel::VertexArray::Create();
//
//        float squareVertices[5 * 4] = {
//            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
//             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
//            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
//        };
//
//        Hazel::Ref<Hazel::VertexBuffer> squareVB = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
//        squareVB->SetLayout({
//           { Hazel::ShaderDataType::Float3, "a_Position" },
//           { Hazel::ShaderDataType::Float2, "a_TexCoord" }
//            });
//        _SquareVA->AddVertexBuffer(squareVB);
//
//        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
//        Hazel::Ref<Hazel::IndexBuffer> squareIB = Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
//        _SquareVA->SetIndexBuffer(squareIB);
//
//        std::string vertexSrc = R"(
//            #version 330 core
//
//            layout(location = 0) in vec3 a_Position;
//            layout(location = 1) in vec4 a_Color;
//
//            uniform mat4 u_ViewProjection;
//            uniform mat4 u_Transform;
//
//            out vec3 v_Position;
//            out vec4 v_Color;
//
//            void main()
//            {
//                v_Position = a_Position;
//                v_Color = a_Color;
//                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
//            }
//        )";
//
//        std::string fragmentSrc = R"(
//            #version 330 core
//
//            layout(location = 0) out vec4 color;
//
//            in vec3 v_Position;
//            in vec4 v_Color;
//
//            void main()
//            {
//                color = vec4(v_Position * 0.5 + 0.5, 1.0);
//                color = v_Color;
//            }
//        )";
//
//        _Shader = Hazel::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);
//
//        /// SQUARE SHADER
//
//        std::string flagColorShadervertexSrc = R"(
//            #version 330 core
//
//            layout(location = 0) in vec3 a_Position;
//
//            uniform mat4 u_ViewProjection;
//            uniform mat4 u_Transform;
//
//            out vec3 v_Position;
//
//            void main()
//            {
//                v_Position = a_Position;
//                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
//            }
//        )";
//
//        std::string flatColorShaderfragmentSrc = R"(
//            #version 330 core
//
//            layout(location = 0) out vec4 color;
//
//            in vec3 v_Position;
//
//            uniform vec3 u_Color;
//
//            void main()
//            {
//                color = vec4(u_Color, 1.0);
//            }
//        )";
//
//        _FlatColorShader = Hazel::Shader::Create("FlatColor", flagColorShadervertexSrc, flatColorShaderfragmentSrc);
//
//        auto textureShader = _ShaderLibrary.Load("assets/shaders/Texture.glsl");
//
//        _Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
//        _ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");
//
//        textureShader->Bind();
//        textureShader->SetInt("u_Texture", 0);
//    }
//
//    void OnAttach() override 
//    {
//    }
//
//    void OnDetach() override
//    {
//
//    }
//
//    void OnUpdate(Hazel::Timestep deltaTime) override
//    {
//        //if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
//        //{
//        //    HZ_TRACE("Tab key is pressed!");
//        //}
//
//        // Update
//        _CameraController.OnUpdate(deltaTime);
//
//        // Render
//        Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
//        Hazel::RenderCommand::Clear();
//
//        Hazel::Renderer::BeginScene(_CameraController.GetCamera());
//
//        static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
//
//        _FlatColorShader->Bind();
//        _FlatColorShader->SetFloat3("u_Color", _SquareColor);
//
//        for(int y = 0; y < 20; y++)
//        {
//            for(int x = 0; x < 20; x++)
//            {
//                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
//                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
//                Hazel::Renderer::Submit(_FlatColorShader, _SquareVA, transform);
//            }
//        }
//
//        auto textureShader = _ShaderLibrary.Get("Texture");
//
//        _Texture->Bind();
//        Hazel::Renderer::Submit(textureShader, _SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
//
//        _ChernoLogoTexture->Bind();
//        Hazel::Renderer::Submit(textureShader, _SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
//
//        // Triangle
//        //Hazel::Renderer::Submit(_Shader, _VertexArray);
//
//        Hazel::Renderer::EndScene();
//    }
//
//    virtual void OnImGuiRender() override
//    {
//        ImGui::Begin("Settings");
//        ImGui::ColorEdit3("Square Color", glm::value_ptr(_SquareColor));
//        ImGui::End();
//    }
//
//    void OnEvent(Hazel::Event& e) override
//    {
//        _CameraController.OnEvent(e);
//
//        //// HZ_TRACE("{0}", event);
//
//        //if (event.GetEventType() == Hazel::EventType::KeyPressed)
//        //{
//        //    Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
//        //    HZ_TRACE("{0}", (char)e.GetKeyCode());
//        //}
//
//        //Hazel::EventDispatcher dispatcher(event);
//        //dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnkeyPressedEvent));
//    }
//
//    //bool OnkeyPressedEvent(Hazel::KeyPressedEvent& event)
//    //{
//    //    if (event.GetKeyCode() == HZ_KEY_LEFT)
//    //        _CameraPosition.x -= _CameraSpeed;
//
//    //    if (event.GetKeyCode() == HZ_KEY_RIGHT)
//    //        _CameraPosition.x += _CameraSpeed;
//
//    //    if (event.GetKeyCode() == HZ_KEY_DOWN)
//    //        _CameraPosition.y -= _CameraSpeed;
//
//    //    if (event.GetKeyCode() == HZ_KEY_UP)
//    //        _CameraPosition.y += _CameraSpeed;
//
//    //    return false;
//    //}
//
//private:
//    Hazel::ShaderLibrary _ShaderLibrary;
//    Hazel::Ref<Hazel::Shader> _Shader;
//    Hazel::Ref<Hazel::VertexArray> _VertexArray;
//
//    Hazel::Ref<Hazel::Shader> _FlatColorShader;
//    Hazel::Ref<Hazel::VertexArray> _SquareVA;
//
//    Hazel::Ref<Hazel::Texture2D> _Texture, _ChernoLogoTexture;
//
//    Hazel::OrthographicCameraController _CameraController;
//    glm::vec3 _SquareColor = { 0.2f, 0.3f, 0.8f };
//};
//
//class Sandbox : public Hazel::Application {
//public:
//    Sandbox() 
//    {
//        // PushLayer(new ExampleLayer());
//        PushLayer(new Sandbox2D());
//    }
//
//    ~Sandbox() {
//
//    }
//};
//
//Hazel::Application* Hazel::CreateApplication() {
//    return new Sandbox();
//}