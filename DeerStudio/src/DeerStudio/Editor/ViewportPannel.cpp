#include "ViewportPannel.h"
#include "Deer/Scene/Components.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Core/Input.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "Deer/Core/Project.h"
#include "Deer/Scene/Scene.h"

#include <string>

namespace Deer {
	ViewportPannel::ViewportPannel(const std::string& windowName, Ref<ActiveEntity>& activeEntity)
		: m_windowName(windowName), m_activeEntity(activeEntity) {

        m_frameBuffer = FrameBuffer::create(FrameBufferSpecification(100, 100, { TextureBufferType::RGBA8, TextureBufferType::RED_INTEGER}, 1, false));
        m_virtualCamera.transform.position = glm::vec3(0, 10, -20);
	}

	void ViewportPannel::onImGui() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin(m_windowName.c_str());
        ImVec2 contentRegionMin = ImGui::GetWindowContentRegionMin();
        ImVec2 pos = ImGui::GetWindowPos();
        pos.y += contentRegionMin.y;

        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)
            && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            ImGui::SetWindowFocus();
        }

        m_isActive = ImGui::IsWindowFocused();
        ImVec2 windowSize = ImGui::GetContentRegionAvail();

        if (m_lastWindowSize != *(glm::vec2*)&windowSize) {
            m_lastWindowSize = { windowSize.x, windowSize.y };
            m_frameBuffer->resize(windowSize.x, windowSize.y);
            m_virtualCamera.camera.aspect = (float)windowSize.x / (float)windowSize.y;
        }

        m_frameBuffer->bind();
        m_frameBuffer->clear();
        int clearData = -1;
        m_frameBuffer->clearBuffer(1, &clearData);
        unsigned char clearColor[4]{ 0, 0, 0, 255 };
        m_frameBuffer->clearBuffer(0, &clearColor);

        Project::m_scene->render(m_virtualCamera);
        
        m_frameBuffer->unbind();

        ImGui::Image((void*)m_frameBuffer->getTextureBufferID(0), windowSize, ImVec2(0, 1), ImVec2(1, 0));
        bool isUsingDrawGizmo = drawGizmos(pos.x, pos.y, windowSize.x, windowSize.y);

        if (!isUsingDrawGizmo && m_handleClick) {
            int relativeX, relativeY;

            ImVec2 mPos = ImGui::GetMousePos();
            relativeX = mPos.x - pos.x;
            relativeY = windowSize.y - (mPos.y - pos.y);

            if (relativeX >= 0 && relativeX < windowSize.x &&
                relativeY >= 0 && relativeY < windowSize.y) {

                int id = m_frameBuffer->getTextureBufferPixel(1, relativeX, relativeY);

                if (!(Input::isKeyPressed(DEER_KEY_LEFT_CONTROL) || Input::isKeyPressed(DEER_KEY_LEFT_ALT)))
                    m_activeEntity->clear();

                if (id >= 0) {
                    Entity& selectedEntity = Project::m_scene->getMainEnviroment()->getEntity((uid)id);
                    m_activeEntity->addEntity(selectedEntity);
                }
            }
        }

        m_handleClick = false;

        m_frameBuffer->unbind();
        ImGui::End();
        ImGui::PopStyleVar();
	}

    void ViewportPannel::onRender(Timestep timestep) {
        if (!m_isActive)
            return;

        if (!Input::isKeyPressed(DEER_KEY_LEFT_CONTROL)) {

            float vel = 6;
            if (Input::isKeyPressed(DEER_KEY_LEFT_SHIFT))
                vel = 14;
            if (Input::isKeyPressed(DEER_KEY_LEFT_ALT))
                vel = 1.0f;

            if (Input::isKeyPressed(DEER_KEY_W))
                m_virtualCamera.transform.position += m_virtualCamera.transform.rotation * glm::vec3(0, 0, 1) * timestep.getSeconds() * vel;

            if (Input::isKeyPressed(DEER_KEY_S))
                m_virtualCamera.transform.position += m_virtualCamera.transform.rotation * glm::vec3(0, 0, -1) * timestep.getSeconds() * vel;

            if (Input::isKeyPressed(DEER_KEY_D))
                m_virtualCamera.transform.position += m_virtualCamera.transform.rotation * glm::vec3(1, 0, 0) * timestep.getSeconds() * vel;

            if (Input::isKeyPressed(DEER_KEY_A))
                m_virtualCamera.transform.position += m_virtualCamera.transform.rotation * glm::vec3(-1, 0, 0) * timestep.getSeconds() * vel;

            if (Input::isKeyPressed(DEER_KEY_SPACE))
                m_virtualCamera.transform.position.y += timestep.getSeconds();

        }

        if (Input::isMouseButtonPressed(DEER_MOUSE_BUTTON_2)) {
            float posX, posY;
            Input::getMousePos(posX, posY);
            glm::vec2 newMousePos = glm::vec2(posX, posY);
            
            if (m_lastMousePressedButton1) {
                glm::vec2 mouseDiff = newMousePos - m_lastMousePos;

                glm::quat pitchQuat = glm::angleAxis(mouseDiff.y * m_sensitivity, glm::vec3(1, 0, 0));
                glm::quat yawQuat = glm::angleAxis(mouseDiff.x * m_sensitivity, glm::vec3(0, 1, 0));

                m_virtualCamera.transform.rotation = yawQuat  * m_virtualCamera.transform.rotation * pitchQuat;
            }

            m_lastMousePos = newMousePos;
            m_lastMousePressedButton1 = true;
        } else
            m_lastMousePressedButton1 = false;

    }

    void ViewportPannel::onEventCallback(Event& e) {
        EventDispatcher dispatcher(e);

        dispatcher.dispatch<MouseButtonPressedEvent>(std::bind(&ViewportPannel::onClickEvent, this, std::placeholders::_1));
        dispatcher.dispatch<KeyPressedEvent>(std::bind(&ViewportPannel::onKeyEvent, this, std::placeholders::_1));
    }

    bool ViewportPannel::onClickEvent(MouseButtonPressedEvent mouseEvent) {
        if (!m_isActive)
            return false;

        if (mouseEvent.getMouseButton() == DEER_MOUSE_BUTTON_1)
            m_handleClick = true;

        return false;
    }

    bool ViewportPannel::onKeyEvent(KeyPressedEvent keyEvent)
    {
        if (!m_isActive)
            return false;

        if (Input::isKeyPressed(DEER_KEY_T))
            m_transformMode = TransformMode::Translate;
        if (Input::isKeyPressed(DEER_KEY_R))
            m_transformMode = TransformMode::Rotate;
        if (Input::isKeyPressed(DEER_KEY_G))
            m_transformMode = TransformMode::Scale;

        if (Input::isKeyPressed(DEER_KEY_D) && Input::isKeyPressed(DEER_KEY_LEFT_CONTROL)) {
            if (m_activeEntity->count() > 0)
                m_activeEntity->getEntity(0).duplicate();
        }

        return false;
    }

    bool ViewportPannel::drawGizmos(int wPosX, int wPosY, int wSizeX, int wSizeY) {

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        glm::mat4 camMatrix = glm::inverse(m_virtualCamera.transform.getMatrix());//
        glm::mat4 projectionMatrix = m_virtualCamera.camera.getMatrix() * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));

        ImGuizmo::SetRect(wPosX, wPosY, wSizeX, wSizeY);
        if (m_activeEntity->count() != 0) {

            Entity& currentEntity = m_activeEntity->getEntity(0);

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
}
