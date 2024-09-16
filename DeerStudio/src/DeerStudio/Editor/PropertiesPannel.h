#pragma once
#include "DeerStudio/Editor/EditorPannel.h"
#include "DeerStudio/Editor/ActiveEntity.h"

#include "Deer/Core/Core.h"

#include "Deer/Scene/Entity.h"

namespace Deer {
	class PropertiesPannel : public EditorPannel {
	public:
		PropertiesPannel(Ref<ActiveEntity>& activeEntity) : m_activeEntity(activeEntity) { }
		void onImGui() override;

	private:
		void addComponentContext();
		Ref<ActiveEntity> m_activeEntity;

	private:
		template<typename T>
		bool collapsingComponentHeader(const std::string& componentName, bool canDelete = true);

		template<typename T>
		void addComponentButton(const std::string& componentName);

		void drawMagicSlider(const std::string& text, float* value);
		void drawMagicSlider3f(const std::string& text, float* value, float defaultValue = 0);

	};

}