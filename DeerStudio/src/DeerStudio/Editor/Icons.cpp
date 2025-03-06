#include "Icons.h"
#include "DeerRender/Render/Texture.h"
#include "Deer/DataStore/DataStore.h"

namespace Deer {
	Ref<Texture2D> scene_icon;
	Ref<Texture2D> folder_icon;
	Ref<Texture2D> add_icon;
	Ref<Texture2D> substract_icon;
	Ref<Texture2D> fill_icon;
	Ref<Texture2D> fill_empty_icon;

	void setupIcons() {
		scene_icon = Texture2D::create(DataStore::rootPath / "editor/icons/scene.png");
		folder_icon = Texture2D::create(DataStore::rootPath / "editor/icons/folder.png");
		add_icon = Texture2D::create(DataStore::rootPath / "editor/icons/add.png");
		substract_icon = Texture2D::create(DataStore::rootPath / "editor/icons/substract.png");
		fill_icon = Texture2D::create(DataStore::rootPath / "editor/icons/fill.png");
		fill_empty_icon = Texture2D::create(DataStore::rootPath / "editor/icons/fill_empty.png");
	}
}
