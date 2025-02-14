#version 460 core
layout (location = 0) in vec2 textCoord;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 fragPos;

uniform vec3 u_camPos; // camera pos
uniform sampler2D u_tex_0;

struct Material {
    sampler2D dif;  // 漫反射各通道对应的反射系数
    sampler2D spe;  // 镜面反射各通道对应的反射系数
    float shi;      // 高光系数
};
uniform Material u_mat;

struct PointLight {
    vec3 pos;
    vec3 amb;
    vec3 dif;
    vec3 spe;
    float c, l, q;   
};
uniform PointLight u_p_lit;

struct DirLight {
    vec3 dir;
    vec3 amb;
    vec3 dif;
    vec3 spe;
};  
uniform DirLight u_d_lit;

struct SpotLight {
    vec3 pos;
    vec3 dir;
    vec3 amb;
    vec3 dif;
    vec3 spe;
    float c, l, q, i_cut, o_cut;
};  
uniform SpotLight u_s_lit;

layout (location = 0) out vec4 fragColor;

vec3 CalcRateOfDirLight(vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseMap, vec3 specularMap) {
    vec3 lightDir = normalize(u_d_lit.dir);
    float angle = max(dot(normal, -lightDir), 0.0);
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(-dot(viewDir, reflectDir), 0.0), u_mat.shi);

    vec3 ambientRate = u_d_lit.amb * diffuseMap;
    vec3 diffuseRate = u_d_lit.dif * angle * diffuseMap;
    vec3 specularRate = u_d_lit.spe * specularMap * spec;

    return ambientRate + diffuseRate + specularRate;
}

vec3 CalcRateOfPointLight(vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseMap, vec3 specularMap) {
    float x = distance(fragPos, u_p_lit.pos);
    float attenuation = 1.0 / (u_p_lit.c + u_p_lit.l * x + u_p_lit.q * x * x);

    vec3 lightDir = normalize(fragPos - u_p_lit.pos);
    float angle = max(dot(normal, -lightDir), 0.0);
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(-dot(viewDir, reflectDir), 0.0), u_mat.shi);

    vec3 ambientRate = u_p_lit.amb * diffuseMap * attenuation;
    vec3 diffuseRate = u_p_lit.dif * angle * diffuseMap * attenuation;
    vec3 specularRate = u_p_lit.spe * specularMap * spec * attenuation;

    return ambientRate + diffuseRate + specularRate;
}

vec3 CalcRateOfSpotLight(vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseMap, vec3 specularMap) {
    float x = distance(fragPos, u_s_lit.pos);
    float attenuation = 1.0 / (u_s_lit.c + u_s_lit.l * x + u_s_lit.q * x * x);

    vec3 lightDir = normalize(fragPos - u_s_lit.pos);
    float angle = max(dot(normal, -lightDir), 0.0);
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(-dot(viewDir, reflectDir), 0.0), u_mat.shi);

    float theta = dot(lightDir, normalize(u_s_lit.dir));
    float epsilon = u_s_lit.i_cut - u_s_lit.o_cut;
    float intensity = clamp((theta - u_s_lit.o_cut) / epsilon, 0.0, 1.0);

    vec3 ambientRate = u_s_lit.amb * diffuseMap * attenuation * intensity;
    vec3 diffuseRate = u_s_lit.dif * angle * diffuseMap * attenuation * intensity;
    vec3 specularRate = u_s_lit.spe * specularMap * spec * attenuation * intensity;

    return ambientRate + diffuseRate + specularRate;
}

void main()
{
    vec4 rawColor = texture2D(u_tex_0, textCoord);                                      // 获取纹理颜色
    vec3 diffuseMap = texture2D(u_mat.dif, textCoord).rgb;                              // 获取漫反射贴图
    vec3 specularMap = texture2D(u_mat.spe, textCoord).rgb;                             // 获取镜面反射贴图
    vec3 viewDir = normalize(fragPos - u_camPos);                                       // 计算视线方向
    vec3 dirLightRate = CalcRateOfDirLight(normal, fragPos, viewDir, diffuseMap, specularMap);
    vec3 pointLightRate = CalcRateOfPointLight(normal, fragPos, viewDir, diffuseMap, specularMap);
    vec3 spotLightRate = CalcRateOfSpotLight(normal, fragPos, viewDir, diffuseMap, specularMap);
    dirLightRate = clamp(dirLightRate, 0.0, 1.0);
    pointLightRate = clamp(pointLightRate, 0.0, 1.0);
    spotLightRate = clamp(spotLightRate, 0.0, 1.0);
    fragColor = vec4((rawColor.rgb * (dirLightRate + pointLightRate + spotLightRate)), rawColor.a);
}