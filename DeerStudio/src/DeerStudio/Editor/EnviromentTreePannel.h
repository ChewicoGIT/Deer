#pragma once
#include "DeerStudio/Editor/EditorPannel.h"
#include "DeerStudio/Editor/ActiveEntity.h"

#include "Deer/Core/Core.h"
#include "Deer/Core/Events/KeyEvent.h"
#include "Deer/Core/Events/Event.h"
#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Entity.h"

namespace Deer {
	class EnviromentTreePannel : public EditorPannel {
	public:
		EnviromentTreePannel(const Ref<Environment>& enviroment, const std::string& name, Ref<ActiveEntity>& activeEntity);

		void onImGui() override;
		void onEvent(Event& e) override;
	private:

		void clickEntity(Entity& entity);
		bool onKeyPressed(KeyPressedEvent& keyPressed);
		void updateEntity(Entity& entity);
		bool updateDragPayload(Entity* entity, const std::string& name);
		void updateReciveDragPayload(Entity& entity);
		void updateContextMenu();

	private:
		Ref<Environment> m_enviroment;
		Ref<ActiveEntity> m_activeEntity;
		bool m_isRightClickHandled = false;
		bool m_openRenamePopUp = false;

		std::string m_treeName;
		Entity* m_contextMenuEntity;
	};
}

