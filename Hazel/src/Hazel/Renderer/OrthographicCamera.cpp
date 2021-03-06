#include "hzpch.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : _ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), _ViewMatrix(1.0f)
    {
        HZ_PROFILE_FUNCTION();

        _ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        HZ_PROFILE_FUNCTION();

        _ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        _ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix() 
    {
        HZ_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _Position) * 
            glm::rotate(glm::mat4(1.0f), glm::radians(_Rotation), glm::vec3(0,0,1));

        _ViewMatrix = glm::inverse(transform);
        _ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
    }
}