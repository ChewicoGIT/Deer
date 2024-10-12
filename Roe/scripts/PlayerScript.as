
class PlayerScript : ComponentScript {
	void update() {
		print("" + entity.uid);
		Vec3 position = entity.getPosition();
		position = position + (Vec3(0, 1, 0) * 1 / 60);
		entity.setPosition(position);

		entity.setScale(entity.getScale() + Vec3(0.01, 0, 0));
	}
}