
class PlayerScript : ComponentScript {
	Entity parent;

	void update() {

		parent = entity.getParent();
		print("" + entity.uid);
		
		if (isKeyPressed(DeerKey::KEY_A)){
			Vec3 position = parent.getPosition();
			position = position + (Vec3(0, 1, 0) * 1 / 60);
			parent.setPosition(position);
		}
	}
}