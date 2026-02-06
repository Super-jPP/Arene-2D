#include "World.h"

void World::update(float dt, const Vec2& playerPos) {
    for (auto& enemy : m_enemies) {
        enemy->update(dt, playerPos); // Appelle la version Zombie ou Ghost automatiquement
    }
}

void World::draw(sf::RenderWindow& window) {
    for (auto& enemy : m_enemies) enemy->draw(window);
}

void World::spawnWolf(Vec2 pos)
{
        m_enemies.push_back(std::make_unique<Wolf>(pos));
}
