
class PlayerScript : ComponentScript {
	float height = 0;
	float minHeight = 1;
	float maxHeight = 9;

	void update() {
		if (isKeyPressed(DeerKey::KEY_W)) {
			height += 0.02 * 7;
		}

		if (isKeyPressed(DeerKey::KEY_S)) {
			height -= 0.02 * 7;
		}

		if (height > maxHeight)
			height = maxHeight;

		if (height < minHeight)
			height = minHeight;


		Vec3 currentPos = entity.getPosition();
		entity.setPosition(Vec3(currentPos.x, height, currentPos.z));
		
	}
}