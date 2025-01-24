#include "TreePannel.h"
#include "Deer/Core/Project.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Entity.h"

#include "imgui.h"

namespace Deer {
	void updateEntity(Entity& entity);
	void updateReciveDragPayload(Entity& entity);
	bool updateDragPayload(Entity* entity, const std::string& name);
	void updateContextMenu();

	bool m_isRightClickHandled;
	Entity* m_contextMenuEntity = nullptr;

	void treePannel_onImGui() {
		ImGui::Begin("Tree Pannel", (bool*)0, ImGuiWindowFlags_MenuBar);

		m_isRightClickHandled = false;
		Entity& root = Project::m_scene.getMainEnviroment()->getRoot();

		if (!m_contextMenuEntity)
			m_contextMenuEntity = &root;

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("New")) {
				if (ImGui::MenuItem("Entity")) {
					Entity& entity = Project::m_scene.getMainEnviroment()->createEntity("new entity");
					entity.setParent(root);
				}
				if (ImGui::MenuItem("Camera")) {
					Entity& entity = Project::m_scene.getMainEnviroment()->createEntity("new camera");
					entity.addComponent<CameraComponent>();
					entity.setParent(root);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		for (auto& entityID : Project::m_scene.getMainEnviroment()->getRoot().getChildren()) {
			Entity& childEntity = Project::m_scene.getMainEnviroment()->getEntity(entityID);
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


	void updateEntity(Entity& entity) {
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

				Entity& childEntity = Project::m_scene.getMainEnviroment()->getEntity(entityID);
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

	void updateReciveDragPayload(Entity& entity) {
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_ENTITY")) {
				Entity* receivedData = *(Entity**)payload->Data;

				if (!entity.isDescendant(*receivedData))
					receivedData->setParent(entity);
			}

			ImGui::EndDragDropTarget();
		}
	}

	bool updateDragPayload(Entity* entity, const std::string& name) {
		if (!ImGui::BeginDragDropSource())
			return false;

		ImGui::SetDragDropPayload("_ENTITY", &entity, sizeof(Entity*));

		ImGui::Text(name.c_str());
		ImGui::EndDragDropSource();
		return true;
	}

	void updateContextMenu() {

		bool callRename = false;

		if (ImGui::BeginPopup("Entity Context Menu")) {
			if (ImGui::MenuItem("New Entity")) {
				Entity& entity = Project::m_scene.getMainEnviroment()->createEntity("new entity");
				entity.setParent(*m_contextMenuEntity);

				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("New Camera")) {
				Entity& entity = Project::m_scene.getMainEnviroment()->createEntity("new camera");
				entity.addComponent<CameraComponent>();
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