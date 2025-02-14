#include "Hazy/Renderer/Camera.h"

namespace Hazy {

    Camera::Camera(float fov, float aspectRatio, float zNear, float zFar)
        : m_fov(fov), m_aspectRatio(aspectRatio), m_near(zNear), m_far(zFar) {
        updateProjectionMatrix();
        updateVectors();
        updateMatrices();
    }

    void Camera::setPosition(const glm::vec3& position) {
        m_position = position;
        m_matricesDirty = true;
    }

    void Camera::setOrientation(const glm::quat& orientation) {
        m_orientation = glm::normalize(orientation);
        m_vectorsDirty = true;
        m_matricesDirty = true;
    }

    void Camera::setTransform(const glm::vec3& position, const glm::quat& orientation) {
        m_position = position;
        m_orientation = glm::normalize(orientation);
        m_vectorsDirty = true;
        m_matricesDirty = true;
    }

    Camera& Camera::translate(const glm::vec3& localOffset) {
        if (m_vectorsDirty) {
            updateVectors();
        }

        // 在局部坐标系中进行平移
        m_position += m_cachedRight * localOffset.x +
            m_cachedUp * localOffset.y +
            m_cachedForward * localOffset.z;

        m_matricesDirty = true;
        return *this;
    }

    Camera& Camera::rotate(float angle, const glm::vec3& localAxis) {
        if (m_vectorsDirty) {
            updateVectors();
        }

        // 将局部轴转换为世界轴
        glm::vec3 worldAxis = m_orientation * localAxis;

        // 创建旋转四元数并应用
        glm::quat rotation = glm::angleAxis(glm::radians(angle), glm::normalize(worldAxis));
        m_orientation = glm::normalize(rotation * m_orientation);

        m_vectorsDirty = true;
        m_matricesDirty = true;
        return *this;
    }

    Camera& Camera::rotateAround(const glm::vec3& worldPoint, float angle, const glm::vec3& worldAxis) {
        // 创建世界空间中的旋转
        glm::quat rotation = glm::angleAxis(glm::radians(angle), glm::normalize(worldAxis));

        // 旋转位置
        glm::vec3 toPoint = m_position - worldPoint;
        toPoint = rotation * toPoint;
        m_position = worldPoint + toPoint;

        // 旋转方向
        m_orientation = glm::normalize(rotation * m_orientation);

        m_vectorsDirty = true;
        m_matricesDirty = true;
        return *this;
    }

    void Camera::updateVectors() {
        if (!m_vectorsDirty) return;

        m_cachedForward = glm::normalize(m_orientation * glm::vec3(0.0f, 0.0f, -1.0f));
        m_cachedRight   = glm::normalize(m_orientation * glm::vec3(1.0f, 0.0f,  0.0f));
        m_cachedUp      = glm::normalize(m_orientation * glm::vec3(0.0f, 1.0f,  0.0f));

        m_vectorsDirty = false;
    }

    void Camera::updateMatrices() {
        // 矩阵都没变化，向量肯定也没变化
        if (!m_matricesDirty && !m_projectionDirty) return;

        if (m_vectorsDirty) {
            updateVectors();
        }

        if (m_projectionDirty) {
            updateProjectionMatrix();
        }

        m_viewMatrix = glm::lookAt(m_position, m_position + m_cachedForward, m_cachedUp);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

        m_matricesDirty = false;
    }

    void Camera::updateProjectionMatrix() {
        if (!m_projectionDirty) return;

        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_near, m_far);
        m_projectionDirty = false;
    }

}