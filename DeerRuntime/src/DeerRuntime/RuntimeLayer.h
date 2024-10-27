#pragma once
#include "Deer/Core/Application.h"
#include "Deer/Core/Layer.h"

namespace Deer {
	class RuntimeLayer : public Layer {
		void onAttach() override;

		void onRender(Timestep delta) override;
		void onUpdate(Timestep delta) override;
	};
}