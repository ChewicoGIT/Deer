#include "PropertiesPannel.h"
#include "imgui.h"

namespace Deer {

	bool* getIsEditingState(ImGuiID id)
	{
		ImGuiStorage* storage = ImGui::GetStateStorage();
		void* ptr = storage->GetVoidPtr(id);
		if (ptr)
			return (bool*)ptr;

		// If state doesn't exist, initialize it
		bool* state = new bool(false);
		storage->SetVoidPtr(id, state);
		return state;
	}

	void PropertiesPannel::onImGui() {

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
		ImGui::Begin("Properties");

		if (!m_entity.isValid()) {
			ImGui::End();
			ImGui::PopStyleVar();
			return;
		}

		auto& tag = m_entity.getComponent<TagComponent>();
		if (tag.tag == "")
			ImGui::Text("-");
		else
			ImGui::Text(tag.tag.c_str());

		ImGui::SameLine();
		addComponentContext();
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		if (collapsingComponentHeader<TransformComponent>("Transform Component", false))
		{

			auto& transform = m_entity.getComponent<TransformComponent>();
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::Indent();

			drawMagicSlider3f("Position", &transform.position.x, 0);
			glm::vec3 rotation = transform.getEulerAngles();
			glm::vec3 lastRotation = rotation;
			drawMagicSlider3f("Rotation", &rotation.x, 0);
			if (rotation != lastRotation)
				transform.setEulerAngles(rotation);
			drawMagicSlider3f("Scale", &transform.scale.x, 1);

			if (rotation != lastRotation)
				transform.setEulerAngles(rotation);

			ImGui::Unindent();
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
		}

		if (collapsingComponentHeader<MeshRenderComponent>("MeshRender Component"))
		{
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::Indent();

			auto& mesh = m_entity.getComponent<MeshRenderComponent>();

			if (mesh.mesh == nullptr)
				ImGui::Text("Current mesh: null");
			else
				ImGui::Text("Current mesh: null");

			ImGui::Unindent();
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
		}

		if (collapsingComponentHeader<CameraComponent>("Camera Component"))
		{
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::Indent();

			auto& camera = m_entity.getComponent<CameraComponent>();

			float fov = glm::degrees(camera.fov);
			ImGui::SliderFloat("Fov", &fov, 1, 180);
			camera.fov = glm::radians(fov);
			ImGui::InputFloat("Z Near", &camera.nearZ, 0, 1000);
			ImGui::InputFloat("Z Far", &camera.farZ, 1, 10000);

			ImGui::Unindent();
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void PropertiesPannel::addComponentContext() {

		float buttonWidth = ImGui::CalcTextSize(" + add Component ").x; // Example button width
		float windowWidth = ImGui::GetWindowSize().x;
		float availableWidth = windowWidth - ImGui::GetCursorPosX();

		// Place button at the right, with some padding (e.g., 10px)
		ImGui::SetCursorPosX(windowWidth - buttonWidth - 20);

		if (ImGui::Button(" + add Component ")) {//, ImVec2(ImGui::GetWindowContentRegionWidth(), 40)
			// Opens a popup window when the button is clicked
			ImGui::OpenPopup("Add Component Popup");
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
		ImGui::SetNextWindowSize(ImVec2(240, 200));
		// Create the popup window
		if (ImGui::BeginPopup("Add Component Popup"))
		{
			
			addComponentButton<MeshRenderComponent>("Mesh Render Component");
			addComponentButton<CameraComponent>("Camera Component");

			ImGui::EndPopup();  // End the popup
		}
		ImGui::PopStyleVar();
	}

	void PropertiesPannel::drawMagicSlider(const std::string& text, float* value) {
		ImGuiID id = ImGui::GetID(text.c_str());

		bool* isEditing = getIsEditingState(id);
		if (*isEditing)
		{
			if (ImGui::InputFloat(text.c_str(), value, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				*isEditing = false;

			if (!ImGui::IsItemActive() && ImGui::IsMouseClicked(0))
				*isEditing = false;

			return;
		}

		ImGui::DragFloat(text.c_str(), value, 0.05f, 0.0f, 0.0f, "%.3f",
			ImGuiSliderFlags_NoRoundToFormat);

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			*isEditing = true;
	}

	void PropertiesPannel::drawMagicSlider3f(const std::string& text, float* value, float defaultValue) {

		ImGui::Columns(4, 0, false);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2());
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f); // Set FrameRounding to 0 for hard edges

		ImGui::Text(text.c_str());

		ImGui::NextColumn();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f)); // Red background
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.2f, 0.2f, 1.0f)); // Darker red when hovered
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.2f, 0.2f, 1.0f)); // Even darker red when active

		if (ImGui::Button((" X ##" + text).c_str()))
			*value = defaultValue;

		ImGui::SameLine();

		ImGui::PopStyleColor(3); // Restore original style

		drawMagicSlider("##" + text + "_x", value);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f)); // Green background
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.6f, 0.2f, 1.0f)); // Darker green when hovered
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.4f, 0.2f, 1.0f)); // Even darker green when active

		ImGui::NextColumn();
		if (ImGui::Button((" Y ##" + text).c_str()))
			value[1] = defaultValue;

		ImGui::SameLine();
		ImGui::PopStyleColor(3); // Restore original style


		drawMagicSlider("##" + text + "_y", &value[1]);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.7f, 1.0f)); // Blue background
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.6f, 1.0f)); // Darker blue when hovered
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.4f, 1.0f)); // Even darker blue when active

		ImGui::NextColumn();
		if (ImGui::Button((" Z ##" + text).c_str()))
			value[2] = defaultValue;

		ImGui::SameLine();
		ImGui::PopStyleColor(3); // Restore original style

		drawMagicSlider("##" + text + "_z", &value[2]);
		ImGui::Columns();
		ImGui::PopStyleVar(2);

	}

	template<typename T>
	inline bool PropertiesPannel::collapsingComponentHeader(const std::string& componentName, bool canDelete)
	{
		if (!m_entity.hasComponent<T>())
			return false;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
		bool collapsingHeader = m_entity.hasComponent<T>() && ImGui::TreeNodeEx(componentName.c_str(), flags);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup(componentName.c_str());
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
		if (ImGui::BeginPopup(componentName.c_str())) {

			if (ImGui::Selectable("reset")) {
				m_entity.getComponent<T>() = T();
				ImGui::CloseCurrentPopup();
			}

			if (canDelete && ImGui::Selectable("delete")) {
				m_entity.removeComponent<T>();
				ImGui::CloseCurrentPopup();
				// Force false to avoid not found errors
				ImGui::EndPopup();
				ImGui::PopStyleVar();
				return false;
			}

			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();

		return collapsingHeader;
	}

	template<typename T>
	void PropertiesPannel::addComponentButton(const std::string& componentName)
	{
		ImGuiSelectableFlags selectableFlag = (m_entity.hasComponent<T>()) ? ImGuiSelectableFlags_Disabled : ImGuiSelectableFlags_None;
		if (ImGui::Selectable(componentName.c_str(), false, selectableFlag)) {
			m_entity.addComponent<T>();
			ImGui::CloseCurrentPopup();
		}
	}
}

