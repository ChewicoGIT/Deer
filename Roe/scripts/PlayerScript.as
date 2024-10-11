
class PlayerScript : ComponentScript {
	void update() {
		Vec3 position = getEntityPosition(UID);
		position = position + (Vec3(0, 1, 0) * 1 / 60);
		setEntityPosition(position, UID);
	}
}