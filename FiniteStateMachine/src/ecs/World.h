#pragma once
#include <vector>
#include <memory>
#include "gameplay/Obstacles.h"
#include "gameplay/Enemy.h"

class World {
public:
    void update(float dt, const Vec2& playerPos);
    void draw(sf::RenderWindow& window);
    void spawnWolf(Vec2 pos); 

private:
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<Obstacle> m_obstacles;
};