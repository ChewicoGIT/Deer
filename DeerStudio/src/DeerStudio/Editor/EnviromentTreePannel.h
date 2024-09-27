#pragma once
#include "DeerStudio/Editor/EditorPannel.h"
#include "DeerStudio/Editor/ActiveEntity.h"

#include "Deer/Core/Core.h"
#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Entity.h"

namespace Deer {
	class EnviromentTreePannel : public EditorPannel {
	public:
		EnviromentTreePannel(const Ref<Environment>& enviroment, const std::string& name, Ref<ActiveEntity>& activeEntity);

		void onImGui() override;
	private:

		void clickEntity(Entity& entity);
		void updateEntity(Entity& entity);
		bool updateDragPayload(Entity* entity, const std::string& name);
		void updateReciveDragPayload(Entity& entity);
		void updateContextMenu();

	private:
		Ref<Environment> m_enviroment;
		Ref<ActiveEntity> m_activeEntity;

		std::string m_treeName;
		Entity* m_contextMenuEntity;
	};
}

