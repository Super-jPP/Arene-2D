#include "gameplay/Enemy.h"

class Wolf : public Enemy {
public:
    Wolf(Vec2 pos) : Enemy(pos, 150.f, sf::Color(255, 255, 255, 150)) {}

    void update(float dt, const Vec2& playerPos) override {
        // Logique de mouvement différente possible ici
        Vec2 dir = (playerPos - m_position).normalized();
        m_position += dir * m_speed * dt;
        m_shape.setPosition({ m_position.x, m_position.y });
    }
};