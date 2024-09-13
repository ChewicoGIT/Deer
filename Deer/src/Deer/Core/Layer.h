#pragma once
#include "Deer/Core/Events/Event.h"
#include "Deer/Core/Timestep.h"
#include <string>

namespace Deer {
	class Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Timestep delta) {}
		virtual void onImGUI() {}
		virtual void onEvent(Event& event) {}

		inline const std::string& getName() const { return m_name; }
	protected:
		std::string m_name;
	};
}