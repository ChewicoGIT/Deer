#include "Icons.h"
#include "DeerRender/Render/Texture.h"
#include "Deer/DataStore/DataStore.h"

namespace Deer {
	Ref<Texture2D> Icons::scene_icon;
	Ref<Texture2D> Icons::folder_icon;
	Ref<Texture2D> Icons::add_icon;
	Ref<Texture2D> Icons::substract_icon;
	Ref<Texture2D> Icons::fill_icon;
	Ref<Texture2D> Icons::fill_empty_icon;
	Ref<Texture2D> Icons::box_select;

	void Icons::setupIcons() {
		Icons::scene_icon = Texture2D::create(DataStore::rootPath / "editor/icons/scene.png");
		Icons::folder_icon = Texture2D::create(DataStore::rootPath / "editor/icons/folder.png");
		Icons::add_icon = Texture2D::create(DataStore::rootPath / "editor/icons/add.png");
		Icons::substract_icon = Texture2D::create(DataStore::rootPath / "editor/icons/substract.png");
		Icons::fill_icon = Texture2D::create(DataStore::rootPath / "editor/icons/fill.png");
		Icons::fill_empty_icon = Texture2D::create(DataStore::rootPath / "editor/icons/fill_empty.png");
		Icons::box_select = Texture2D::create(DataStore::rootPath / "editor/icons/box_select.png");
	}
}
