#pragma once
#include "Deer/Memory.h"

#define ICON_MIN_SIZE 90
#define ICON_MAX_SIZE 160

#define ICON_BTN_MIN_SIZE 32

namespace Deer {
	class Texture2D;

	namespace Icons {
		extern Ref<Texture2D> scene_icon;
		extern Ref<Texture2D> folder_icon;
		extern Ref<Texture2D> add_icon;
		extern Ref<Texture2D> substract_icon;
		extern Ref<Texture2D> fill_icon;
		extern Ref<Texture2D> fill_empty_icon;
		extern Ref<Texture2D> box_select;
		extern Ref<Texture2D> face_voxel_selection;
		extern Ref<Texture2D> internal_voxel_selection;
		
		void setupIcons();
	}
}