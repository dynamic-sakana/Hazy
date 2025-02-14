#include "Hazy.h"
#include <hazy_pch.h>

using namespace Hazy;
class DemoWindow : public Window {
public:
    DemoWindow() : Window(WindowProps { "Demo Window", 2560, 1440 }, API::OpenGL) {
        {
            ContextLock lock(*m_context);
            VertexBufferLayout layout = {
                { DataType::Float, 3, "a_Position" },
                { DataType::Float, 2, "a_TexCoord" },
                { DataType::Float, 3, "a_Normal" }
            };
            float vertices[] = {
                -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
                 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
                 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
                 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
                -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
                -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
            
                -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f,
                 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f,
                -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f,
            
                -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
            
                 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,
            
                -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,   1.0f, 1.0f,   0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f,
            
                -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f,  -1.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  -1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,   0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f,  -1.0f,  0.0f
            };
            VertexBuffer& vertexBuffer = m_context->create<VertexBuffer>("cube", vertices, sizeof(vertices), layout,  BufferUsage::StaticDraw);

            uint32_t indices[] = {
                 0,  1,  2,  3,  4,  5,
                 6,  7,  8,  9, 10, 11,
                12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23,
                24, 25, 26, 27, 28, 29,
                30, 31, 32, 33, 34, 35
            };
            IndexBuffer& indexBuffer = m_context->create<IndexBuffer>("cube", indices, sizeof(indices), BufferUsage::StaticDraw);

            m_context->create<VertexArray>("cube").addVertexBuffer(vertexBuffer).setIndexBuffer(indexBuffer);

            m_context->create<Shader>("cube", "assets/shader/common.vert", "assets/shader/common.frag");
            m_context->create<Texture2D>("cube", "assets/texture/box.png", TextureType::Diffuse).setFilter(TextureFilter::Linear).setWrap(TextureWrap::Repeat).generateMipMap();
            m_context->create<Texture2D>("cube_spe", "assets/texture/box_specular.png", TextureType::Specular).setFilter(TextureFilter::Linear).setWrap(TextureWrap::Repeat).generateMipMap();
        }

        m_camera.translate({ 0.0f, 0.0f, -3.0f });

        ImGuiLayer* imguiLayer = new ImGuiLayer(this,
            [this] {
                {
                    glm::vec3 cameraPos = m_camera.getPosition();
                    glm::vec3 lightPos = m_light.position;
                    ImGui::Begin("Info");
                    ImGui::SetWindowFontScale(2.0f);
                    auto io = ImGui::GetIO();
                    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                    ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y, cameraPos.z);
                    ImGui::Text("PointLight  Position: (%.2f, %.2f, %.2f)", lightPos.x, lightPos.y, lightPos.z);
                    ImGui::End();
                }
                {
                    ImGui::Begin("Color Picker");
                    ImGui::SetWindowFontScale(2.0f);
                    ImGui::ColorEdit4("background color", glm::value_ptr(m_backgroundColor));
                    ImGui::End();
                }
                {
                    ImGui::Begin("Camera");
                    ImGui::SetWindowFontScale(2.0f);
                    ImGui::SliderFloat("Move   Speed", &m_moveSpeed, 0.0f, 10.0f);
                    ImGui::SliderFloat("rotate Speed", &m_rotateSpeed, 30.0f, 180.0f);
                    ImGui::End();
                }
                {
                    ImGui::Begin("PointLight");
                    ImGui::SetWindowFontScale(2.0f);
                    ImGui::ColorEdit3("ambient  color", glm::value_ptr(m_light.ambient));
                    ImGui::ColorEdit3("diffuse  color", glm::value_ptr(m_light.diffuse));
                    ImGui::ColorEdit3("specular color", glm::value_ptr(m_light.specular));
                    ImGui::SliderFloat("y", &m_light.position.y, -10.0f, 10.0f);
                    ImGui::SliderFloat("radius", &m_lightRadius, 0.0f, 10.0f);
                    ImGui::SliderFloat("rotation speed", &m_lightRotationSpeed, 0.0f, 360.0f);
                    ImGui::End();
                }
            });
        m_layerStack.pushOverlay(imguiLayer);

        m_updateFunc =
            [this]() {
                if (Input::isKeyPressed(Key::W)) m_camera.translate({  0.0f, 0.0f,  m_moveSpeed * m_deltaTime });
                if (Input::isKeyPressed(Key::S)) m_camera.translate({  0.0f, 0.0f, -m_moveSpeed * m_deltaTime });
                if (Input::isKeyPressed(Key::A)) m_camera.translate({ -m_moveSpeed * m_deltaTime, 0.0f, 0.0f });
                if (Input::isKeyPressed(Key::D)) m_camera.translate({  m_moveSpeed * m_deltaTime, 0.0f, 0.0f });
                if (Input::isKeyPressed(Key::LeftControl)) m_camera.translate({ 0.0f, -m_moveSpeed * m_deltaTime, 0.0f });
                if (Input::isKeyPressed(Key::Space)) m_camera.translate({ 0.0f, m_moveSpeed * m_deltaTime, 0.0f });
                if (Input::isKeyPressed(Key::Up))    m_camera.pitch( m_rotateSpeed * m_deltaTime);
                if (Input::isKeyPressed(Key::Down))  m_camera.pitch(-m_rotateSpeed * m_deltaTime);
                if (Input::isKeyPressed(Key::Left))  m_camera.yaw( m_rotateSpeed * m_deltaTime);
                if (Input::isKeyPressed(Key::Right)) m_camera.yaw(-m_rotateSpeed * m_deltaTime);
                float angle = glm::radians(NormalizeAngle(TimePoint::Now() * m_lightRotationSpeed, 360.0f));
                m_light.position.x = glm::cos(angle) * m_lightRadius;
                m_light.position.z = glm::sin(angle) * m_lightRadius;
            };

        m_renderFunc =
            [this]() {
                VertexArrayLock vertexArrayLock(m_context->get<VertexArray>("cube"));
                ShaderLock shaderLock(m_context->get<Shader>("cube"));
                Texture2DLock texture2DLock(m_context->get<Texture2D>("cube"), 0);
                Texture2DLock texture2DLock2(m_context->get<Texture2D>("cube_spe"), 1);
                m_context->get<Shader>("cube")
                    .setMat4("u_model", m_model)
                    .setMat4("u_viewProj", m_camera.getViewProjectionMatrix())
                    .setVec3("u_camPos", m_camera.getPosition());

                m_context->get<Shader>("cube")
                    .setInt("u_mat.dif", 0)
                    .setInt("u_mat.spe", 1)
                    .setFloat("u_mat.shi", 32.0f);

                m_context->get<Shader>("cube")
                    .setVec3("u_p_lit.pos", m_light.position)
                    .setVec3("u_p_lit.amb", m_light.ambient)
                    .setVec3("u_p_lit.dif", m_light.diffuse)
                    .setVec3("u_p_lit.spe", m_light.specular)
                    .setFloat("u_p_lit.c", m_light.constant)
                    .setFloat("u_p_lit.l", m_light.linear)
                    .setFloat("u_p_lit.q", m_light.quadratic);

                m_context->get<Shader>("cube").setInt("u_tex_0", 0);

                m_renderer->clearColor(m_backgroundColor);
                m_renderer->clear();
                m_renderer->drawCall(m_context->get<VertexArray>("cube"));
                m_context->get<Shader>("cube")
                    .setMat4("u_model", glm::scale(glm::translate(glm::mat4(1.0f), m_light.position), glm::vec3(0.125f)));
                m_renderer->drawCall(m_context->get<VertexArray>("cube"));
            };
    }

    ~DemoWindow() {
        ContextLock lock(m_context);
    }

    void onEvent(Event& e) override {
        Window::onEvent(e);
        switch(e.getType()) {
        case EventType::WindowResize:
            m_camera.setAspectRatio(static_cast<WindowResizeEvent&>(e).getAspectRatio());
            break;
        default: break;
        }
    }

private:
    Camera m_camera = Camera({ 45.0f, 16.0f / 9.0f, 0.1f, 100.0f });
    PointLight m_light = PointLight({ 1.0f, 0.75f, 2.0f });
    glm::mat4 m_model = glm::mat4(1.0f);
    glm::mat4 m_lightModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.125f));
    glm::vec4 m_backgroundColor = { 0.2f, 0.3f, 0.3f, 1.0f };
    float m_moveSpeed = 3.0f;
    float m_rotateSpeed = 60.0f;
    float m_lightRadius = 3.0f;
    float m_lightRotationSpeed = 60.0f;
};

int main(int, char* []) {
    DemoWindow* window = new DemoWindow();
    Application::addWindow(window);
    Application::Run();
    return 0;
}