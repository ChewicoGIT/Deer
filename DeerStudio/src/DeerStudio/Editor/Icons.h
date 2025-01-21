#pragma once
#include "Deer/Core/Core.h"

#define ICON_MIN_SIZE 60
#define ICON_MAX_SIZE 90

namespace Deer {
	class Texture2D;

	extern Ref<Texture2D> scene_icon;
	extern Ref<Texture2D> folder_icon;
	
	void setupIcons();
}