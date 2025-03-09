

class BallScript : ComponentScript {

    float height = 2;
    float ballHeight = 0.25f;

    float checkDistance = 8.8f;
    float checkRad = 0.1f;
    
    float downHeight = .25;
    float upHeight = 9.75;

    Entity player;
    Entity enemy;

    Vec3 direction;

    void start() {
        player = getEntity(6);
        enemy = getEntity(5);

        direction = Vec3(-1,1,0);
        direction.normalize();
    }

    void update() {

        Vec3 currentPos = entity.getPosition();
        if (currentPos.y <= downHeight){
            direction.y = abs(direction.y);
        }
        if (currentPos.y >= upHeight){
            direction.y = -abs(direction.y);
        }

        if (direction.x < 0 && currentPos.x >= -checkDistance - checkRad / 2.0f && currentPos.x <= -checkDistance + checkRad / 2.0f) {
            if (hasCollision(player)) {
                recalculateDirection(player);
            }
        }
        
        if (direction.x > 0 && currentPos.x >= checkDistance - checkRad / 2.0f && currentPos.x <= checkDistance + checkRad / 2.0f) {
            if (hasCollision(enemy)) {
                recalculateDirection(enemy);
            }
        }
        
        Vec3 nextPos = currentPos + direction * 0.14f;
        entity.setPosition(nextPos);

        if (abs(nextPos.x) > 10)
            entity.setPosition(Vec3(0, 5, 0));
    }

    void recalculateDirection(Entity col) {
        Vec3 ballEntDir = entity.getPosition() - col.getPosition();
        direction = ballEntDir.normalize();

            if (direction.x > 0)
                direction.x = 1;
            else
                direction.x = -1;

    }

    bool hasCollision(Entity colEntity) {
        Vec3 position = entity.getPosition();
        float maxBallPos = position.y + ballHeight / 2.0f;
        float minBallPos = position.y - ballHeight / 2.0f;

        Vec3 entPosition = colEntity.getPosition();
        float entMaxPos = entPosition.y + height / 2.0f;
        float entMinPos = entPosition.y - height / 2.0f;

        return (maxBallPos <= entMaxPos && maxBallPos >= entMinPos);// || 
            //(minBallPos <= entMaxPos && minBallPos >= entMinPos);
    }
    
}