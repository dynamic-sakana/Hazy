#include "Hazy/Renderer/Renderer.h"

namespace Hazy {
    std::string Renderer::s_viewProjectMatrixName  = "u_viewProj";
    std::string Renderer::s_modelMatrixName        = "u_model";
    std::string Renderer::s_texture2DSamplerPrefix = "u_texture2D_";
    std::string Renderer::s_texture3DSamplerPrefix = "u_texture3D_";
    std::string Renderer::s_lightColorName         = "u_lightColor";
    std::string Renderer::s_lightPositionName      = "u_lightPos";
}