#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include "math/Vec2.h"
#include "Enemy.h"
#include "Weapons.h"
#include "Player.h"

// Spawns enemies like Vampire Survivors/HoloCure:
// - continuous spawn with a timer
// - spawns outside the camera view
// - enemies will chase the player (detect radius can be huge)
class Spawner
{
public:
    void init(const Vec2& worldSize);

    void update(float dt, const Vec2& playerPos, const sf::View& cameraView);
    void draw(sf::RenderTarget& rt);

    void setSpawnRate(float perSecond) { m_spawnRate = perSecond; }
    void setMaxEnemies(std::size_t n)  { m_maxEnemies = n; }
    void setSpawnMargin(float px)      { m_spawnMargin = px; }
    std::vector<Enemy>& getEnemies() { return m_enemies; }

private:
    Vec2 randomOutsideView(const sf::View& view, const Vec2& playerPos);
    float randf(float a, float b);

    Vec2 m_worldSize{0.f, 0.f};

    std::vector<Enemy> m_enemies;

    float m_spawnRate   = 3.0f;   // enemies / sec
    float m_spawnAcc    = 0.0f;
    std::size_t m_maxEnemies = 250;
    float m_spawnMargin = 140.f;

    std::mt19937 m_rng{ std::random_device{}() };
};
