
class PlayerScript : ComponentScript {
	Entity parent;

	void start() {
		parent = entity;
		print("" + parent.uid);
		
	}

	void update() {
		if (isKeyPressed(DeerKey::KEY_A)){
			Vec3 position = parent.getPosition();
			position = position + (Vec3(0, 1, 0) * 1 / 60);
			parent.setPosition(position);
		}
	}
}