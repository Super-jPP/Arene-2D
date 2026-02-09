#include "gameplay/Enemy.h"

class Bones : public Enemy {
public:
    Bones(Vec2 pos) : Enemy(pos, 60.f, sf::Color::Green) {}

    void update(float dt, const Vec2& playerPos) override {
        Vec2 dir = (playerPos - m_position).normalized();
        m_position += dir * m_speed * dt;
        m_shape.setPosition({ m_position.x, m_position.y });
    }
};