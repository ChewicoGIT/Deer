

class BallScript : ComponentScript {

    Vec3 direction;

    void start() {
        direction = Vec3(1,0,0);
        direction.normalize();
    }

    void update() {
        Vec3 currentPos = entity.getPosition();
        Vec3 nextPos = currentPos + direction * 0.02;
        entity.setPosition(nextPos);

    }

}