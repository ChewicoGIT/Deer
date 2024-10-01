#include "GamePannel.h"
#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Scene/Entity.h"
#include "imgui.h"

namespace Deer {
    GamePannel::GamePannel(Ref<Scene> scene) 
        : m_scene(scene){
        FrameBufferSpecification fbSpecs = FrameBufferSpecification(100, 100, { TextureBufferType::RGBA8 }, 1, false);
        m_frameBuffer = FrameBuffer::create(fbSpecs);
    }

    void GamePannel::onImGui() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Game Window");
        ImGui::PopStyleVar();

        Ref<Environment> environment = m_scene->getMainEnviroment();
        uid cameraUID = environment->tryGetMainCamera();

        if (cameraUID == 0) {
            ImGui::TextColored(ImVec4(.3f, .3f, .8f, 1.0f), "There is no camera");
            ImGui::End();
            return;
        }

        Entity& cameraEntity = environment->getEntity(cameraUID);
        CameraComponent& cameraComponent = cameraEntity.getComponent<CameraComponent>();

        ImVec2 contentRegionMin = ImGui::GetWindowContentRegionMin();
        ImVec2 pos = ImGui::GetWindowPos();
        pos.y += contentRegionMin.y;

        ImVec2 windowSize = ImGui::GetContentRegionAvail();

        if (m_lastWindowSize != *(glm::vec2*)&windowSize) {
            m_lastWindowSize = { windowSize.x, windowSize.y };
            m_frameBuffer->resize(windowSize.x, windowSize.y);

            cameraComponent.aspect = windowSize.x / windowSize.y;
        }

        m_frameBuffer->bind();
        m_frameBuffer->clear();
        unsigned char clearColor[4]{ 0, 0, 0, 255 };
        m_frameBuffer->clearBuffer(0, &clearColor);

        m_scene->render();
        m_frameBuffer->unbind();
        ImGui::Image((void*)m_frameBuffer->getTextureBufferID(0), windowSize);

        ImGui::End();
        /*
        ImVec2 contentRegionMin = ImGui::GetWindowContentRegionMin();
        ImVec2 pos = ImGui::GetWindowPos();
        pos.y += contentRegionMin.y;

        ImVec2 windowSize = ImGui::GetContentRegionAvail();

        if (m_lastWindowSize != *(glm::vec2*)&windowSize) {
            m_lastWindowSize = { windowSize.x, windowSize.y };
            m_frameBuffer->resize(windowSize.x, windowSize.y);

            //m_virtualCamera.camera.aspect = (float)windowSize.x / (float)windowSize.y;
        }

        m_frameBuffer->bind();
        m_frameBuffer->clear();
        int clearData = -1;
        m_frameBuffer->clearBuffer(1, &clearData);
        unsigned char clearColor[4]{ 0, 0, 0, 255 };
        m_frameBuffer->clearBuffer(0, &clearColor);

        //m_environment->render(m_virtualCamera);

        ImGui::Image((void*)m_frameBuffer->getTextureBufferID(0), windowSize);
        bool isUsingDrawGizmo = drawGizmos(pos.x, pos.y, windowSize.x, windowSize.y);

        if (!isUsingDrawGizmo && m_handleClick) {
            int relativeX, relativeY;

            ImVec2 mPos = ImGui::GetMousePos();
            relativeX = mPos.x - pos.x;
            relativeY = mPos.y - pos.y;

            if (relativeX >= 0 && relativeX < windowSize.x &&
                relativeY >= 0 && relativeY < windowSize.y) {

                int id = m_frameBuffer->getTextureBufferPixel(1, relativeX, relativeY);

                if (!(Input::isKeyPressed(DEER_KEY_LEFT_CONTROL) || Input::isKeyPressed(DEER_KEY_LEFT_ALT)))
                    m_activeEntity->clear();

                if (id >= 0) {
                    Entity& selectedEntity = m_environment->tryGetEntity((uid)id);
                    m_activeEntity->addEntity(selectedEntity);
                }
                
            }*/
    }
}