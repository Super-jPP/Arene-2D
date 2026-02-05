#include "gameplay/Weapons.h"

void Weapons::setWeaponType(WeaponType type) {
    m_currentType = type;
    switch (type) {
    case WeaponType::PISTOL:
        m_projSpeed = 500.f; m_projSize = 5.f; m_projCount = 1; break;
    case WeaponType::SHOTGUN:
        m_projSpeed = 400.f; m_projSize = 4.f; m_projCount = 3; break;
    case WeaponType::MAGIC_WAND:
        m_projSpeed = 300.f; m_projSize = 10.f; m_projCount = 1; break;
    }
}

void Weapons::fire(const Vec2& startPos, const Vec2& targetDir) {
    for (int i = 0; i < m_projCount; ++i) {
        Projectile p;
        p.position = startPos;
        p.size = m_projSize;

        // Logique Dev B : on utilise tes Vec2 pour la vitesse
        p.velocity = targetDir * m_projSpeed;

        // Optionnel : ajouter une petite dispersion pour le Shotgun
        m_projectiles.push_back(p);
    }
}

void Weapons::update(float dt) {
    for (auto& p : m_projectiles) {
        p.position += p.velocity * dt; // Mouvement linéaire simple

        // TODO: Désactiver si hors écran pour les perfs
    }
    // Nettoyage des projectiles inactifs
}

void Weapons::draw(sf::RenderWindow& window) {
    sf::CircleShape shape;
    for (const auto& p : m_projectiles) {
        shape.setRadius(p.size);
        shape.setPosition({ p.position.x, p.position.y });
        shape.setFillColor(sf::Color::Yellow);
        window.draw(shape);
    }
}