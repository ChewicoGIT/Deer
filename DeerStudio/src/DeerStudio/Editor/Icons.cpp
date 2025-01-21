#include "Icons.h"
#include "DeerRender/Render/Texture.h"

namespace Deer {
	Ref<Texture2D> scene_icon;
	Ref<Texture2D> folder_icon;

	void setupIcons() {
		scene_icon = Texture2D::create("editor/icons/scene.png");
		folder_icon = Texture2D::create("editor/icons/folder.png");
	}
}
