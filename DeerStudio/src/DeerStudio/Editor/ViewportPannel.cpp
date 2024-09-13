#include "ViewportPannel.h"
#include "Deer/Scene/Components.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Core/Input.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include <string>

namespace Deer {
	ViewportPannel::ViewportPannel(Ref<Environment>& environment, const std::string& windowName)
		: m_environment(environment), m_windowName(windowName) {

        m_frameBuffer = FrameBuffer::create(FrameBufferSpecification(100, 100, 1, false));
        m_virtualCamera.transform.position = glm::vec3(0, 0, -3);
	}

	void ViewportPannel::onImGui() {

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin(m_windowName.c_str());
        ImVec2 contentRegionMin = ImGui::GetWindowContentRegionMin();
        ImVec2 pos = ImGui::GetWindowPos();
        pos.y += contentRegionMin.y;

        m_isActive = ImGui::IsWindowFocused();
        ImVec2 windowSize = ImGui::GetContentRegionAvail();

        if (m_lastWindowSize != *(glm::vec2*)&windowSize) {
            m_lastWindowSize = { windowSize.x, windowSize.y };
            m_frameBuffer->resize(windowSize.x, windowSize.y);
            m_virtualCamera.camera.aspect = (float)windowSize.x / (float)windowSize.y;
        }

        m_frameBuffer->bind();
        m_frameBuffer->clear();
        m_environment->render(m_virtualCamera);
        m_frameBuffer->unbind();

        ImGui::Image((void*)m_frameBuffer->getColorBufferID(), windowSize);
        drawGizmos(pos.x, pos.y, windowSize.x, windowSize.y);

        ImGui::End();
        ImGui::PopStyleVar();
	}

    void ViewportPannel::onUpdate(Timestep timestep) {
        if (!m_isActive)
            return;

        float vel = 1;
        if (Input::isKeyPressed(DEER_KEY_LEFT_SHIFT))
            vel = 4;
        if (Input::isKeyPressed(DEER_KEY_LEFT_ALT))
            vel = .25f;

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

        if (Input::isKeyPressed(DEER_KEY_LEFT_CONTROL))
            m_virtualCamera.transform.position.y += -timestep.getSeconds();

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
        }
        else
            m_lastMousePressedButton1 = false;

        if (Input::isKeyPressed(DEER_KEY_T))
            m_transformMode = TransformMode::Translate;
        if (Input::isKeyPressed(DEER_KEY_R))
            m_transformMode = TransformMode::Rotate;
        if (Input::isKeyPressed(DEER_KEY_Y))
            m_transformMode = TransformMode::Scale;

    }

    void ViewportPannel::drawGizmos(int wPosX, int wPosY, int wSizeX, int wSizeY) {

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        glm::mat4 camMatrix = glm::inverse(m_virtualCamera.transform.getMatrix());//
        glm::mat4 projectionMatrix = m_virtualCamera.camera.getMatrix() * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));

        ImGuizmo::SetRect(wPosX, wPosY, wSizeX, wSizeY);
        
        ImGuizmo::DrawGrid(glm::value_ptr(camMatrix), glm::value_ptr(projectionMatrix), glm::value_ptr(glm::mat4(1.0f)), 4.f);

        if (m_entity.isValid()) {
            glm::mat4 entity_matrix = m_entity.getWorldMatrix();

            ImGuizmo::OPERATION operations;

            ImGuizmo::Manipulate(glm::value_ptr(camMatrix), glm::value_ptr(projectionMatrix), (ImGuizmo::OPERATION)m_transformMode,
                ImGuizmo::MODE::LOCAL, glm::value_ptr(entity_matrix));

            if (ImGuizmo::IsUsing()) {
                glm::mat4 parentMatrix = m_entity.getParent().getWorldMatrix();
                glm::mat4 relativeMatrix = glm::inverse(parentMatrix) * entity_matrix;

                glm::vec3 skew;
                glm::vec4 perspective;

                TransformComponent& t = m_entity.getComponent<TransformComponent>();
                glm::decompose(relativeMatrix, t.scale, t.rotation, t.position, skew, perspective);

            }
        }
    }
}
