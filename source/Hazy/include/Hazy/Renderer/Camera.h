#pragma once
#include <hazy_pch.h>

namespace Hazy {

    class Camera {
    public:
        Camera(float fov, float aspectRatio, float zNear, float zFar);
        ~Camera() = default;

        inline const glm::mat4& getViewMatrix() {
            updateMatrices();
            return m_viewMatrix;
        }

        inline const glm::mat4& getProjectionMatrix() {
            updateMatrices();
            return m_projectionMatrix;
        }

        inline const glm::mat4& getViewProjectionMatrix() {
            updateMatrices();
            return m_viewProjectionMatrix;
        }

        inline Camera& setFOV(float fov) {
            if (fov != m_fov) {
                m_fov = fov;
                m_projectionDirty = true;
                m_matricesDirty = true;
            } return *this;
        }

        inline Camera& setAspectRatio(float aspectRatio) {
            if (aspectRatio != m_aspectRatio) {
                m_aspectRatio = aspectRatio;
                m_projectionDirty = true;
                m_matricesDirty = true;
            } return *this;
        }

        inline Camera& setNear(float zNear) {
            if (zNear != m_near) {
                m_near = zNear;
                m_projectionDirty = true;
                m_matricesDirty = true;
            } return *this;
        }

        inline Camera& setFar(float zFar) {
            if (zFar != m_far) {
                m_far = zFar;
                m_projectionDirty = true;
                m_matricesDirty = true;
            } return *this;
        }

        /**
         * @brief 旋转相机，旋转轴为相机坐标系的 x 正方向（俯仰）
         * @param angle 旋转的角度
         * @return Camera& 自身的引用
         */
        inline Camera& pitch(float angle) {
            rotate(angle, glm::vec3(1, 0, 0));
            return *this;
        }

        /**
         * @brief 旋转相机，旋转轴为相机坐标系的 y 正方向（左右）
         * @param angle 旋转的角度
         * @return Camera& 自身的引用
         */
        inline Camera& yaw(float angle) {
            rotate(angle, glm::vec3(0, 1, 0));
            return *this;
        }

        /**
         * @brief 旋转相机，旋转轴为相机坐标系的 z 正方向（侧身）
         * @param angle 旋转的角度
         * @return Camera& 自身的引用
         */
        inline Camera& roll(float angle) {
            rotate(angle, glm::vec3(0, 0, 1));
            return *this;
        }

        inline glm::vec3 getPosition() const { return m_position; }
        inline glm::vec3 getRight() const { return m_cachedRight; }
        inline glm::vec3 getUp() const { return m_cachedUp; }
        inline glm::vec3 getForward() const { return m_cachedForward; }
        inline glm::quat getOrientation() const { return m_orientation; }

        void setPosition(const glm::vec3& position);
        void setOrientation(const glm::quat& orientation);
        void setTransform(const glm::vec3& position, const glm::quat& orientation);

        /**
         * @brief 移动相机，相对于相机坐标系
         * @param localOffset 相对于相机坐标系的偏移
         * @return 自身的引用，便于链式调用
         */
        Camera& translate(const glm::vec3& localOffset);

        /**
         * @brief 旋转相机，相对于相机坐标系
         * @param angle 旋转角度
         * @param localAxis 相对于相机本身的旋转轴
         * @return 自身的引用，便于链式调用
         */
        Camera& rotate(float angle, const glm::vec3& localAxis);

        /**
         * @brief 绕世界空间中的一个点和轴确定的直线进行旋转
         * @param worldPoint 世界空间中的点
         * @param angle      旋转角度
         * @param worldAxis  世界空间中的旋转轴
         * @return 自身的引用，便于链式调用
         */
        Camera& rotateAround(const glm::vec3& worldPoint, float angle, const glm::vec3& worldAxis);

    private:
        void updateProjectionMatrix();
        void updateMatrices();
        void updateVectors();

        float m_fov, m_aspectRatio, m_near, m_far;

        glm::mat4 m_viewMatrix           { 1.0f };
        glm::mat4 m_projectionMatrix     { 1.0f };
        glm::mat4 m_viewProjectionMatrix { 1.0f };
        
        glm::vec3 m_position { 0.0f };
        glm::quat m_orientation { 1.0f, 0.0f, 0.0f, 0.0f };

        glm::vec3 m_cachedRight   { 1.0f, 0.0f,  0.0f };
        glm::vec3 m_cachedUp      { 0.0f, 1.0f,  0.0f };
        glm::vec3 m_cachedForward { 0.0f, 0.0f, -1.0f };

        bool m_projectionDirty = true;
        bool m_matricesDirty   = true;
        bool m_vectorsDirty    = true;
    };


}