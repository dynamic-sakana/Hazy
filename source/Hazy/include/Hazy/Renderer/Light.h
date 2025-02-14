#pragma once
#include <hazy_pch.h>

namespace Hazy {

    struct PointLight {
    public:

        PointLight(
            const glm::vec3& position,
            const glm::vec3& ambient = { 1.0f, 1.0f, 1.0f },
            const glm::vec3& diffuse = { 1.0f, 1.0f, 1.0f },
            const glm::vec3& specular = { 1.0f, 1.0f, 1.0f },
            float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f)
            : position(position), ambient(ambient), diffuse(diffuse), specular(specular), 
            constant(constant), linear(linear), quadratic(quadratic){ }
        
        ~PointLight() = default;
        
        glm::vec3 position;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
    };

    struct DirectionalLight {
    public:
        DirectionalLight(
            const glm::vec3& direction,
            const glm::vec3& ambient = { 1.0f, 1.0f, 1.0f },
            const glm::vec3& diffuse = { 1.0f, 1.0f, 1.0f },
            const glm::vec3& specular = { 1.0f, 1.0f, 1.0f }
        ) : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular) { }

        ~DirectionalLight() = default;

        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct SpotLight {
    public:
        SpotLight(
            const glm::vec3& position,
            const glm::vec3& direction,
            const glm::vec3& ambient = { 1.0f, 1.0f, 1.0f },
            const glm::vec3& diffuse = { 1.0f, 1.0f, 1.0f },
            const glm::vec3& specular = { 1.0f, 1.0f, 1.0f },
            float cutOff = glm::cos(glm::radians(12.5f)), float outerCutOff = glm::cos(glm::radians(17.5f))
        ) : position(position), direction(direction), ambient(ambient), diffuse(diffuse), specular(specular),
            cutOff(cutOff), outerCutOff(outerCutOff) { }

        ~SpotLight() = default;

        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        inline float getCutOff() const { return cutOff; }
        inline float getOuterCutOff() const { return outerCutOff; }

        inline void setCutOff(float cutOff) { this->cutOff = glm::cos(glm::radians(cutOff)); }
        inline void setOuterCutOff(float outerCutOff) { this->outerCutOff = glm::cos(glm::radians(outerCutOff)); }

    private:

        float cutOff = glm::cos(glm::radians(12.5f));
        float outerCutOff = glm::cos(glm::radians(17.5f));
    };

}