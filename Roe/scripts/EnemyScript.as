
class EnemyScript : ComponentScript {
	float height = 0;

	float minHeight = 1;
	float maxHeight = 9;

	void update() {
		if (isKeyPressed(DeerKey::KEY_O)) {
			height += 0.02 * 7;
		}

		if (isKeyPressed(DeerKey::KEY_L)) {
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