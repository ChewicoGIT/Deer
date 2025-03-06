#include "GamePannel.h"
#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Core/Project.h"
#include "Deer/Scripting/ScriptEngine.h"
#include "Deer/Scene/SceneDataStore.h"

#include "imgui.h"
#include <filesystem>

namespace Deer {
    GamePannel::GamePannel() {
        FrameBufferSpecification fbSpecs = FrameBufferSpecification(100, 100, { TextureBufferType::RGBA8 }, 1, false);
        m_frameBuffer = FrameBuffer::create(fbSpecs);
    }

    void GamePannel::onImGui() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Game Window");
        ImGui::PopStyleVar();

        Ref<Environment> environment = Project::m_scene.getMainEnviroment();
        uid cameraUID = environment->tryGetMainCamera();

        if (cameraUID == 0) {
            ImGui::TextColored(ImVec4(.3f, .3f, .8f, 1.0f), "There is no camera");

            if (!Project::m_scene.getExecutingState()) {
                if (ScriptEngine::isCompilationValid() && ImGui::Button("Execute")) {
                    SceneDataStore::exportRuntimeScene(Project::m_scene);
                    Project::m_scene.beginExecution();
                }
            }
            else {
                if (ImGui::Button("Stop")) {
                    Project::m_scene.endExecution();
                    Project::m_scene = SceneDataStore::importRuntimeScene();
                }
            }

            ImGui::End();
            return;
        }

        Entity& cameraEntity = environment->getEntity(cameraUID);
        CameraComponent& cameraComponent = cameraEntity.getComponent<CameraComponent>();

        ImVec2 contentRegionMin = ImGui::GetWindowContentRegionMin();
        ImVec2 pos = ImGui::GetWindowPos();
        pos.y += contentRegionMin.y;

        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        ImVec2 cursorPos = ImGui::GetCursorPos();

        if (m_lastWindowSize != *(glm::vec2*)&windowSize) {
            m_lastWindowSize = { windowSize.x, windowSize.y };
            m_frameBuffer->resize(windowSize.x, windowSize.y);

            cameraComponent.aspect = windowSize.x / windowSize.y;
        }

        m_frameBuffer->bind();
        m_frameBuffer->clear();
        unsigned char clearColor[4]{ 0, 0, 0, 255 };
        m_frameBuffer->clearBuffer(0, &clearColor);

        Project::m_scene.render();
        m_frameBuffer->unbind();

        ImGui::Image((void*)(uint64_t)m_frameBuffer->getTextureBufferID(0), windowSize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::SetCursorPos(cursorPos);

        if (!Project::m_scene.getExecutingState()) {
            if (ScriptEngine::isCompilationValid() && ImGui::Button("Execute")) {
                SceneDataStore::exportRuntimeScene(Project::m_scene);
                Project::m_scene.beginExecution();
            }
        }
        else {
            if (ImGui::Button("Stop")) {
                Project::m_scene.endExecution();
                Project::m_scene = SceneDataStore::importRuntimeScene();
            }
        }

        ImGui::End();
    }
}