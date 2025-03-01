#include "Viewport.h"
#include "DeerStudio/Editor/ActiveEntity.h"
#include "DeerRender/Render/FrameBuffer.h"
#include "DeerRender/Scene/GizmoRenderer.h"

#include "Deer/Core/Core.h"
#include "DeerRender/Core/Input/Input.h"
#include "Deer/Core/Project.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Entity.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/glm.hpp"
#include <string>

namespace Deer {
    Ref<FrameBuffer> m_frameBuffer;

    glm::vec2 m_lastWindowSize;
    glm::vec2 m_lastMousePos;

    float viewport_relativeXMouse, viewport_relativeYMouse;
    bool viewport_isActive;
    SceneCamera viewport_sceneCamera(TransformComponent(glm::vec3(16, 16, -16)), CameraComponent());

    bool m_lastMousePressedButton1;
    bool m_handleClick;
    bool viewport_hasClicked;

    float m_sensitivity = 0.005f;

    enum class TransformMode {
        Translate = 0,
        Rotate = 1,
        Scale = 2
    };

    TransformMode m_transformMode = TransformMode::Translate;

    void processMovment();
    bool viewport_onClickEvent(MouseButtonPressedEvent mouseEvent);
    bool viewport_onKeyEvent(KeyPressedEvent keyEvent);
    bool drawGizmos(int wPosX, int wPosY, int wSizeX, int wSizeY);

    void viewport_onImGui() {
        viewport_hasClicked = ImGui::GetMouseClickedCount(0) == 1;

        processMovment();

        if (!m_frameBuffer) {
            m_frameBuffer = FrameBuffer::create(FrameBufferSpecification(100, 100, { TextureBufferType::RGBA8, TextureBufferType::RED_INTEGER }, 4, false));
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");

        ImVec2 contentRegionMin = ImGui::GetWindowContentRegionMin();
        ImVec2 wPos = ImGui::GetWindowPos();
        wPos.y += contentRegionMin.y;

        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)
            && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            ImGui::SetWindowFocus();
        }

        viewport_isActive = ImGui::IsWindowFocused();
        ImVec2 windowSize = ImGui::GetContentRegionAvail();

        if (m_lastWindowSize != *(glm::vec2*)&windowSize) {
            m_lastWindowSize = { windowSize.x, windowSize.y };
            m_frameBuffer->resize(windowSize.x, windowSize.y);
            viewport_sceneCamera.camera.aspect = (float)windowSize.x / (float)windowSize.y;
        }

        m_frameBuffer->bind();
        m_frameBuffer->clear();
        int clearData = -1;
        m_frameBuffer->clearBuffer(1, &clearData);
        unsigned char clearColor[4]{ 15, 10, 10, 255 };
        m_frameBuffer->clearBuffer(0, &clearColor);

        ImVec2 mPos = ImGui::GetMousePos();

        viewport_relativeXMouse = (mPos.x - wPos.x) / windowSize.x;
        viewport_relativeYMouse = 1 - (mPos.y - wPos.y) / windowSize.y;

        Project::m_scene.render(viewport_sceneCamera);

        ImGui::Image((void*)m_frameBuffer->getTextureBufferID(0), windowSize, ImVec2(0, 1), ImVec2(1, 0));
        bool isUsingDrawGizmo = drawGizmos(wPos.x, wPos.y, windowSize.x, windowSize.y);

        if (!isUsingDrawGizmo && m_handleClick) {
            int relativeX, relativeY;

            ImVec2 mPos = ImGui::GetMousePos();
            relativeX = mPos.x - wPos.x;
            relativeY = windowSize.y - (mPos.y - wPos.y);

            if (relativeX >= 0 && relativeX < windowSize.x &&
                relativeY >= 0 && relativeY < windowSize.y) {

                int id = m_frameBuffer->getTextureBufferPixel(1, relativeX, relativeY);

                if (!(Input::isKeyPressed(DEER_KEY_LEFT_CONTROL) || Input::isKeyPressed(DEER_KEY_LEFT_ALT)))
                    ActiveEntity::clear();

                if (id >= 0) {
                    Entity& selectedEntity = Project::m_scene.getMainEnviroment()->getEntity((uid)id);
                    ActiveEntity::addEntity(selectedEntity);
                }
            }
        }

        m_handleClick = false;

        m_frameBuffer->unbind();
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void processMovment() {
        if (!viewport_isActive)
            return;

        if (!Input::isKeyPressed(DEER_KEY_LEFT_CONTROL)) {

            float vel = 6;
            if (Input::isKeyPressed(DEER_KEY_LEFT_SHIFT))
                vel = 14;
            if (Input::isKeyPressed(DEER_KEY_LEFT_ALT))
                vel = 1.0f;

            if (Input::isKeyPressed(DEER_KEY_W))
                viewport_sceneCamera.transform.position += viewport_sceneCamera.transform.rotation * glm::vec3(0, 0, 1) * 0.02f * vel;

            if (Input::isKeyPressed(DEER_KEY_S))
                viewport_sceneCamera.transform.position += viewport_sceneCamera.transform.rotation * glm::vec3(0, 0, -1) * 0.02f * vel;

            if (Input::isKeyPressed(DEER_KEY_D))
                viewport_sceneCamera.transform.position += viewport_sceneCamera.transform.rotation * glm::vec3(1, 0, 0) * 0.02f * vel;

            if (Input::isKeyPressed(DEER_KEY_A))
                viewport_sceneCamera.transform.position += viewport_sceneCamera.transform.rotation * glm::vec3(-1, 0, 0) * 0.02f * vel;

            if (Input::isKeyPressed(DEER_KEY_SPACE))
                viewport_sceneCamera.transform.position.y += 0.02f;

        }

        if (Input::isMouseButtonPressed(DEER_MOUSE_BUTTON_2)) {
            float posX, posY;
            Input::getMousePos(posX, posY);
            glm::vec2 newMousePos = glm::vec2(posX, posY);

            if (m_lastMousePressedButton1) {
                glm::vec2 mouseDiff = newMousePos - m_lastMousePos;

                glm::quat pitchQuat = glm::angleAxis(mouseDiff.y * m_sensitivity, glm::vec3(1, 0, 0));
                glm::quat yawQuat = glm::angleAxis(mouseDiff.x * m_sensitivity, glm::vec3(0, 1, 0));

                viewport_sceneCamera.transform.rotation = yawQuat * viewport_sceneCamera.transform.rotation * pitchQuat;
            }

            m_lastMousePos = newMousePos;
            m_lastMousePressedButton1 = true;
        }
        else
            m_lastMousePressedButton1 = false;
    }

    bool drawGizmos(int wPosX, int wPosY, int wSizeX, int wSizeY) {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        glm::mat4 camMatrix = glm::inverse(viewport_sceneCamera.transform.getMatrix());
        glm::mat4 projectionMatrix = viewport_sceneCamera.camera.getMatrix() * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));

        ImGuizmo::SetRect(wPosX, wPosY, wSizeX, wSizeY);
        if (ActiveEntity::count() != 0) {

            Entity& currentEntity = ActiveEntity::getEntity(0);

            glm::mat4 entity_matrix = currentEntity.getWorldMatrix();

            ImGuizmo::OPERATION operations;

            ImGuizmo::Manipulate(glm::value_ptr(camMatrix), glm::value_ptr(projectionMatrix), (ImGuizmo::OPERATION)m_transformMode,
                ImGuizmo::MODE::LOCAL, glm::value_ptr(entity_matrix));

            if (ImGuizmo::IsUsing()) {
                glm::mat4 parentMatrix = currentEntity.getParent().getWorldMatrix();
                glm::mat4 relativeMatrix = glm::inverse(parentMatrix) * entity_matrix;

                glm::vec3 skew;
                glm::vec4 perspective;

                TransformComponent& t = currentEntity.getComponent<TransformComponent>();
                glm::decompose(relativeMatrix, t.scale, t.rotation, t.position, skew, perspective);

                return true;
            }
        }

        return false;
    }

    void viewport_onEvent(Event& e) {
        EventDispatcher dispatcher(e);

        dispatcher.dispatch<MouseButtonPressedEvent>(Deer::viewport_onClickEvent);
        dispatcher.dispatch<KeyPressedEvent>(Deer::viewport_onKeyEvent);
    }
    bool viewport_onClickEvent(MouseButtonPressedEvent mouseEvent) {
        if (!viewport_isActive)
            return false;

        if (mouseEvent.getMouseButton() == DEER_MOUSE_BUTTON_1)
            m_handleClick = true;

        return false;
    }
    bool viewport_onKeyEvent(KeyPressedEvent keyEvent) {
        if (!viewport_isActive)
            return false;

        if (Input::isKeyPressed(DEER_KEY_T))
            m_transformMode = TransformMode::Translate;
        if (Input::isKeyPressed(DEER_KEY_R))
            m_transformMode = TransformMode::Rotate;
        if (Input::isKeyPressed(DEER_KEY_G))
            m_transformMode = TransformMode::Scale;

        if (Input::isKeyPressed(DEER_KEY_D) && Input::isKeyPressed(DEER_KEY_LEFT_CONTROL)) {
            if (ActiveEntity::count() > 0)
                ActiveEntity::getEntity(0).duplicate();
        }

        return false;
    }
}