#pragma once
#include "DeerStudio/Editor/EditorPannel.h"

#include "Deer/Core/Core.h"
#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Entity.h"

namespace Deer {
	class EnviromentTreePannel : public EditorPannel {
	public:
		EnviromentTreePannel(const Ref<Environment>& enviroment, const std::string& name, Entity* activeEntity = nullptr);

		void onImGui() override;
		Entity* getActiveEntity() { return m_activeEntity; }
		void setActiveEntity(Entity* entity) { m_activeEntity = entity; }
	private:
		void setActiveEntity(Entity& entity);
		bool isActiveEntity(Entity& entity);
		void updateEntity(Entity& entity);
		bool updateDragPayload(Entity& entity, const std::string& name);
		void updateReciveDragPayload(Entity& entity);
		void updateContextMenu();

	private:
		Ref<Environment> m_enviroment;
		std::string m_treeName;
		Entity* m_activeEntity;
		Entity m_contextMenuEntity;
	};
}

