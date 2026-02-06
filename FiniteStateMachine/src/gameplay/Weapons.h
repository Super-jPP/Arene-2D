#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "../math/Vec2.h"

enum class WeaponType {
    PISTOL,
    SHOTGUN,
    MAGIC_WAND
};

struct Projectile {
    Vec2 position;
    Vec2 velocity;
    float size;
    bool active = true;
};

class Weapons {
public:

    // Change les stats du système
    void setWeaponType(WeaponType type);

    // Crée un ou plusieurs projectiles selon l'arme
    void fire(const Vec2& startPos, const Vec2& targetDir);

    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    std::vector<Projectile> m_projectiles;
    WeaponType m_currentType;

    // Statistiques gérées par Weapons
    float m_projSpeed;
    float m_projSize;
    int   m_projCount;
    float m_fireRate;
    float m_timer;
};