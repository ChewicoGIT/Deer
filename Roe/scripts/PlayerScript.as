
class PlayerScript : ComponentScript {
	float height = 0;
	float minHeight = 1;
	float maxHeight = 9;

	void update() {
		if (isKeyPressed(DeerKey::KEY_W)) {
			height += 0.02 * 7
		}

		if (isKeyPressed(DeerKey::KEY_A) && isValid){
			Vec3 position = parent.getPosition();
			position = position + (Vec3(0, 1, 0) * 1 / 60);
			parent.setPosition(position);
		}
	}
}