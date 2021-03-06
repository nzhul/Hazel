#include "hzpch.h"
#include "Hazel/Renderer/OrthographicCameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : _AspectRatio(aspectRatio), 
        _Camera(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel), 
        _Rotation(rotation)
    {
    }

    void OrthographicCameraController::OnUpdate(Timestep deltaTime)
    {
        HZ_PROFILE_FUNCTION();

        if (Input::IsKeyPressed(HZ_KEY_A))
        {
            _CameraPosition.x -= cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * deltaTime;
            _CameraPosition.y -= sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * deltaTime;
        }
        else if (Input::IsKeyPressed(HZ_KEY_D))
        {
            _CameraPosition.x += cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * deltaTime;
            _CameraPosition.y += sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * deltaTime;
        }

        if (Input::IsKeyPressed(HZ_KEY_W))
        {
            _CameraPosition.x += -sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * deltaTime;
            _CameraPosition.y += cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * deltaTime;
        }
        else if (Input::IsKeyPressed(HZ_KEY_S))
        {
            _CameraPosition.x -= -sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * deltaTime;
            _CameraPosition.y -= cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * deltaTime;
        }

        if (_Rotation)
        {
            if (Input::IsKeyPressed(HZ_KEY_Q))
                _CameraRotation += _CameraRotationSpeed * deltaTime;

            if (Input::IsKeyPressed(HZ_KEY_E))
                _CameraRotation -= _CameraRotationSpeed * deltaTime;

            if (_CameraRotation > 180.0f)
                _CameraRotation -= 360.0f;
            else if (_CameraRotation <= -180.0f)
                _CameraRotation += 360.0f;

            _Camera.SetRotation(_CameraRotation);
        }

        _Camera.SetPosition(_CameraPosition);

        _CameraTranslationSpeed = _ZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        HZ_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        HZ_PROFILE_FUNCTION();

        _ZoomLevel -= e.GetYOffset() * 0.25f;
        _ZoomLevel = std::max(_ZoomLevel, 0.25f);
        _Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        HZ_PROFILE_FUNCTION();

        _AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        _Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);
        return false;
    }
}