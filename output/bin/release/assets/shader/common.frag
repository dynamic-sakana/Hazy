#version 460 core
layout (location = 0) in vec2 textCoord;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 fragPos;

uniform vec3 u_cameraPos; // camera position
uniform sampler2D u_texture2D_0;

struct Material {
    vec3 ambient;       // 环境光各通道对应的反射系数
    vec3 diffuse;       // 漫反射各通道对应的反射系数
    vec3 specular;      // 镜面反射各通道对应的反射系数
    float shininess;    // 高光系数
};
uniform Material u_material;

struct Light {
    vec3 position;      // 光源位置
    vec3 ambient;       // 环境光各通道对应的强度
    vec3 diffuse;       // 漫反射各通道对应的强度
    vec3 specular;      // 镜面反射各通道对应的强度

    float c, l, q;      // 衰减系数
};
uniform Light u_light;

layout (location = 0) out vec4 fragColor;

void main()
{
    float deltaX = distance(fragPos, u_light.position);                                         // 计算光源到片段的距离
    float attenuation = 1.0 / (u_light.c + u_light.l * deltaX + u_light.q * deltaX * deltaX);   // 计算衰减系数

    vec4 rawColor = texture2D(u_texture2D_0, textCoord);                                        // 获取纹理颜色
    vec3 lightDir = normalize(fragPos - u_light.position);                                      // 计算光线方向

    vec3 ambientLight = u_light.ambient * u_material.ambient * attenuation;                     // 环境光光强

    float diffuse = max(dot(normal, -lightDir), 0.0);                                           // 计算漫反射强度
    vec3 diffuseLight = u_light.diffuse * diffuse * u_material.diffuse * attenuation;           // 漫反射光强

    vec3 viewDir = normalize(u_cameraPos - fragPos);                                            // 获取到视角方向
    vec3 reflectDir = reflect(lightDir, normal);                                                // 获取到反射方向
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);                 // 计算镜面反射强度
    vec3 specularLight = u_light.specular * spec * u_material.specular * attenuation;           // 镜面反射光强

    fragColor = vec4((rawColor.rgb * (ambientLight + diffuseLight + specularLight)), rawColor.a);
}