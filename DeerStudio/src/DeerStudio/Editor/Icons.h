#pragma once
#include "Deer/Core/Memory.h"

#define ICON_MIN_SIZE 90
#define ICON_MAX_SIZE 160

namespace Deer {
	class Texture2D;

	extern Ref<Texture2D> scene_icon;
	extern Ref<Texture2D> folder_icon;
	extern Ref<Texture2D> add_icon;
	extern Ref<Texture2D> substract_icon;
	extern Ref<Texture2D> fill_icon;
	extern Ref<Texture2D> fill_empty_icon;
	
	void setupIcons();
}