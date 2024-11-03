#include "EnviromentTreePannel.h"
#include "imgui.h"
#include "Deer/Scene/Components.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Core/Log.h"
#include "Deer/Core/Input.h"
#include "Deer/Core/KeyCodes.h"

#include <string.h>
#include <functional>

#include "Plattform/OpenGL/imgui_impl_opengl3.h"

namespace Deer {
	EnviromentTreePannel::EnviromentTreePannel(const Ref<Environment>& enviroment, const std::string& name, Ref<ActiveEntity>& activeEntity)
		: m_enviroment(enviroment), m_treeName(name), m_activeEntity(activeEntity) { }

	void EnviromentTreePannel::onImGui() {
		ImGui::ShowDemoWindow();
		ImGui::Begin(m_treeName.c_str());

		if (m_openRenamePopUp) {
			m_openRenamePopUp = false;

			ImGui::OpenPopup("Rename Entity Menu");
		}

		m_isRightClickHandled = false;
		Entity& root = m_enviroment->getRoot();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		for (auto& entityID : m_enviroment->getRoot().getChildren()) {
			Entity& childEntity = m_enviroment->getEntity(entityID);
			updateEntity(childEntity);
		}
		ImGui::PopStyleVar();

		updateContextMenu();

		ImGui::Spacing();
		ImVec2 spaceSize(ImGui::GetWindowContentRegionWidth(), 80);

		ImGui::InvisibleButton("DragDropSpace", spaceSize);
		updateReciveDragPayload(root);

		if (!m_isRightClickHandled && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) &&
			ImGui::IsMouseClicked(ImGuiMouseButton_Right) &&
			!ImGui::IsAnyItemHovered()) {

			m_contextMenuEntity = &root;
			ImGui::OpenPopup("Entity Context Menu");
		}

		updateReciveDragPayload(root);
		ImGui::End();
	}

	void EnviromentTreePannel::onEventCallback(Event& e) {
		EventDispatcher dispatcher(e);

		dispatcher.dispatch<KeyPressedEvent>(std::bind(&EnviromentTreePannel::onKeyPressed, this, std::placeholders::_1));
	}

	void EnviromentTreePannel::clickEntity(Entity& entity) {

		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
			if (!(Input::isKeyPressed(DEER_KEY_LEFT_CONTROL) || Input::isKeyPressed(DEER_KEY_LEFT_ALT)))
				m_activeEntity->clear();

			if (Input::isKeyPressed(DEER_KEY_LEFT_ALT))
				m_activeEntity->removeEntity(entity);
			else
				m_activeEntity->addEntity(entity);
		}
	}

	bool EnviromentTreePannel::onKeyPressed(KeyPressedEvent& keyPressed) {

		if (keyPressed.getKeyCode() == DEER_KEY_F2) {
			if (m_activeEntity->count() > 0) {
				m_contextMenuEntity = &m_activeEntity->getEntity(0);
				m_openRenamePopUp = true;
			}
		}

		return false;
	}

	void EnviromentTreePannel::updateEntity(Entity& entity) {
		auto& tag = entity.getComponent<TagComponent>();
		auto& relationship = entity.getComponent<RelationshipComponent>();

		std::string name = (tag.tag == "") ? "-" : tag.tag;

		const void* entityID = reinterpret_cast<const void*>(static_cast<uintptr_t>(entity.getUID()));

		ImGui::Spacing();

		//End of the tree
		if (relationship.children.size() == 0) {
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth;
			if (m_activeEntity->contains(entity))
				flags |= ImGuiTreeNodeFlags_Selected;

			ImGui::TreeNodeEx(entityID, flags, name.c_str());
			if (!updateDragPayload(&entity, tag.tag))
				updateReciveDragPayload(entity);

			clickEntity(entity);

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
				m_contextMenuEntity = &entity;
				m_isRightClickHandled = true;
				ImGui::OpenPopup("Entity Context Menu");
			}
			 
			return;
		}

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
		if (m_activeEntity->contains(entity))
			flags |= ImGuiTreeNodeFlags_Selected;

		// for the moment i prefer to default open all
		flags |= ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx(entityID, flags, name.c_str())) {
			if (!entity.isRoot())
				updateDragPayload(&entity, tag.tag);
			updateReciveDragPayload(entity);

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
				m_contextMenuEntity = &entity;
				m_isRightClickHandled = true;
				ImGui::OpenPopup("Entity Context Menu");
			}	

			clickEntity(entity);

			for (auto& entityID : entity.getChildren()) {
				updateReciveDragPayload(entity);

				Entity& childEntity = m_enviroment->getEntity(entityID);
				updateEntity(childEntity);
			}

			updateContextMenu();
			ImGui::TreePop();
		}
		else {
			clickEntity(entity);

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
				m_contextMenuEntity = &entity;
				m_isRightClickHandled = true;
				ImGui::OpenPopup("Entity Context Menu");
			}

			if (!entity.isRoot())
				updateDragPayload(&entity, tag.tag);
			updateReciveDragPayload(entity);
		}

	}


	bool EnviromentTreePannel::updateDragPayload(Entity* entity, const std::string& name) {
		if (!ImGui::BeginDragDropSource())
			return false;

		ImGui::SetDragDropPayload("_ENTITY", &entity, sizeof(Entity*));

		ImGui::Text(name.c_str());
		ImGui::EndDragDropSource();
		return true;
	}

	void EnviromentTreePannel::updateReciveDragPayload(Entity& entity) {
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_ENTITY")) {
				Entity* receivedData = *(Entity**)payload->Data;

				if (!entity.isDescendant(*receivedData))
					receivedData->setParent(entity);
			}

			ImGui::EndDragDropTarget();
		}
	}

	void EnviromentTreePannel::updateContextMenu() {

		bool callRename = false;

		if (ImGui::BeginPopup("Entity Context Menu")) {
			if (ImGui::MenuItem("New Entity")) {
				Entity& entity = m_enviroment->createEntity("new entity");
				entity.setParent(*m_contextMenuEntity);

				ImGui::CloseCurrentPopup();
			}
			if (!m_contextMenuEntity->isRoot() && ImGui::MenuItem("Delete")) {
				m_contextMenuEntity->destroy();
				m_activeEntity->clear();
				ImGui::CloseCurrentPopup();
			}
			if (!m_contextMenuEntity->isRoot() && ImGui::MenuItem("Rename")) {
				callRename = true;
				ImGui::CloseCurrentPopup();
			}
			if (!m_contextMenuEntity->isRoot() && ImGui::MenuItem("Duplicate")) {
				m_contextMenuEntity->duplicate();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (callRename)
			ImGui::OpenPopup("Rename Entity Menu");

		if (ImGui::BeginPopup("Rename Entity Menu")) {

			std::string& name = m_contextMenuEntity->getComponent<TagComponent>().tag;
			char nameBuffer[256];
			strcpy_s(nameBuffer, 256, name.c_str());
			
			ImGui::Text("Rename");
			if (ImGui::InputText("##", nameBuffer, 256, ImGuiInputTextFlags_EnterReturnsTrue)) {
				m_contextMenuEntity->getComponent<TagComponent>().tag = nameBuffer;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}
