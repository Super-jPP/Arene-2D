#include "gameplay/Weapons.h"
#include <cmath>

Weapons::Weapons()
    : m_range(60.f)
    , m_damage(10.f)
    , m_knockback(50.f)
    , m_debugTimer(0.f)
{
    // Setup du cercle de debug (optionnel)
    m_debugHitzone.setRadius(m_range);
    m_debugHitzone.setOrigin({ m_range, m_range });
    m_debugHitzone.setFillColor(sf::Color(255, 0, 0, 50));
}

void Weapons::swingSword(const Vec2& playerPos, bool facingLeft, std::vector<Enemy>& enemies) {
    m_debugTimer = 0.1f; // Affiche la zone rouge brièvement
    m_debugHitzone.setPosition({ playerPos.x, playerPos.y });

    for (auto& enemy : enemies) {
        Vec2 toEnemy = enemy.getPos() - playerPos;
        float dist = std::sqrt(toEnemy.x * toEnemy.x + toEnemy.y * toEnemy.y);

        if (dist <= m_range) {
            // Vérification de la direction (le joueur regarde-t-il l'ennemi ?)
            bool enemyIsLeft = (toEnemy.x < 0);

            if (facingLeft == enemyIsLeft) {
                // L'ennemi est touché !
                // enemy.takeDamage(m_damage); 
                // enemy.applyKnockback(toEnemy.normalized() * m_knockback);
            }
        }
    }
}

void Weapons::update(float dt) {
    if (m_debugTimer > 0) m_debugTimer -= dt;
}

void Weapons::draw(sf::RenderWindow& window) {
    if (m_debugTimer > 0) {
        window.draw(m_debugHitzone);
    }
}