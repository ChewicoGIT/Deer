#include "EnviromentTreePannel.h"
#include "imgui.h"
#include "Deer/Scene/Components.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Core/Log.h"

#include <string.h>

#include "Plattform/OpenGL/imgui_impl_opengl3.h"

namespace Deer {
	EnviromentTreePannel::EnviromentTreePannel(const Ref<Environment>& enviroment, const std::string& name, Entity* activeEntity)
		: m_enviroment(enviroment), m_treeName(name), m_activeEntity(activeEntity) { }

	void EnviromentTreePannel::onImGui() {
		ImGui::ShowDemoWindow();
		ImGui::Begin(m_treeName.c_str());
		updateReciveDragPayload(m_enviroment->getRoot());

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		for (auto& entityID : m_enviroment->getRoot().getChildren()) {
			Entity childEntity = m_enviroment->tryGetEntity((uid)entityID);
			updateEntity(childEntity);
		}
		ImGui::PopStyleVar();

		updateContextMenu();

		ImGui::Spacing();
		ImVec2 spaceSize(ImGui::GetWindowContentRegionWidth(), 40);
		ImGui::InvisibleButton("DragDropSpace", spaceSize);
		updateReciveDragPayload(m_enviroment->getRoot());

		ImGui::End();

	}

	void EnviromentTreePannel::setActiveEntity(Entity& entity) {
		if (m_activeEntity != nullptr)
			*m_activeEntity = entity;
	}

	bool EnviromentTreePannel::isActiveEntity(Entity& entity) {
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
			if (entity == *m_activeEntity)
				flags |= ImGuiTreeNodeFlags_Selected;

			ImGui::TreeNodeEx(entityID, flags, name.c_str());
			if (!updateDragPayload(entity, tag.tag))
				updateReciveDragPayload(entity);

			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				*m_activeEntity = entity;

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
				m_contextMenuEntity = entity;
				ImGui::OpenPopup("Entity Context Menu");
			}

			return;
		}

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
		if (entity == *m_activeEntity)
			flags |= ImGuiTreeNodeFlags_Selected;

		// for the moment i prefer to default open all
		flags |= ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx(entityID, flags, name.c_str())) {
			if (!entity.isRoot())
				updateDragPayload(entity, tag.tag);
			updateReciveDragPayload(entity);

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
				m_contextMenuEntity = entity;
				ImGui::OpenPopup("Entity Context Menu");
			}

			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				*m_activeEntity = entity;

			for (auto& entityID : entity.getChildren()) {
				updateReciveDragPayload(entity);

				Entity childEntity = m_enviroment->tryGetEntity((uid)entityID);
				updateEntity(childEntity);
			}

			updateContextMenu();
			ImGui::TreePop();
		}
		else {
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				*m_activeEntity = entity;

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
				m_contextMenuEntity = entity;
				ImGui::OpenPopup("Entity Context Menu");
			}

			if (!entity.isRoot())
				updateDragPayload(entity, tag.tag);
			updateReciveDragPayload(entity);
		}

	}


	bool EnviromentTreePannel::updateDragPayload(Entity& entity, const std::string& name) {
		if (!ImGui::BeginDragDropSource())
			return false;

		ImGui::SetDragDropPayload("_ENTITY", &entity, sizeof(entity));
		ImGui::Text(name.c_str());
		ImGui::EndDragDropSource();
		return true;
	}

	void EnviromentTreePannel::updateReciveDragPayload(Entity& entity) {
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_ENTITY")) {
				Entity* receivedData = (Entity*)payload->Data;

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
				Entity entity = m_enviroment->createEntity("new entity");
				entity.setParent(m_contextMenuEntity);
				ImGui::CloseCurrentPopup();
			}
			if (!m_contextMenuEntity.isRoot() && ImGui::MenuItem("Delete")) {
				m_contextMenuEntity.destroy();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Rename")) {
				callRename = true;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Duplicate")) {
				m_contextMenuEntity.duplicate();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (callRename)
			ImGui::OpenPopup("Rename Entity Menu");

		if (ImGui::BeginPopup("Rename Entity Menu")) {

			std::string& name = m_contextMenuEntity.getComponent<TagComponent>().tag;
			char nameBuffer[256];
			strcpy_s(nameBuffer, 256, name.c_str());
			
			ImGui::Text("Rename");
			if (ImGui::InputText("##", nameBuffer, 256)) {
				m_contextMenuEntity.getComponent<TagComponent>().tag = nameBuffer;
			}

			ImGui::EndPopup();
		}
	}
}
