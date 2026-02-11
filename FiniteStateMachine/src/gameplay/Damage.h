#pragma once
#include <vector>
#include <cmath>
#include "Enemy.h"
#include "Player.h"

class Damage {
public:
    // 1. Le Joueur tape les Ennemis (Épée)
    static void handleSwordCombat(Player& player, std::vector<Enemy>& enemies) {
        // Si le joueur n'est pas en train d'attaquer, on ne fait rien
        if (!player.isAttacking()) return;

        // Configuration de la Hitbox de l'épée
        float swordRange = 60.f;
        Vec2 pPos = player.position();

        // On place le centre de l'impact devant le joueur (selon sa direction)
        float direction = player.isFacingLeft() ? -1.f : 1.f;
        Vec2 hitboxCenter = { pPos.x + (direction * 40.f), pPos.y };

        for (auto& enemy : enemies) {
            if (enemy.isDead()) continue; // On ne tape pas les morts

            // Calcul de distance au carré (plus rapide)
            float dx = hitboxCenter.x - enemy.getPos().x;
            float dy = hitboxCenter.y - enemy.getPos().y;
            float distSq = dx * dx + dy * dy;

            // Si l'ennemi est dans la zone de l'épée
            if (distSq < (swordRange * swordRange)) {
                enemy.takeDamage(1); // L'ennemi prend 1 point de dégât
            }
        }
    }

    // 2. Les Ennemis tapent le Joueur (via FSM)
    static void handleEnemyAttacks(Player& player, std::vector<Enemy>& enemies) {
        for (auto& enemy : enemies) {
            if (enemy.isDead()) continue;

            // On vérifie si l'ennemi est en phase ACTIVE de son attaque
            // et s'il n'a pas déjà infligé ses dégâts pour cette attaque (hitDone)
            if (enemy.getAttackPhase() == AttackPhase::Active && !enemy.isHitDone()) {

                float dx = enemy.getPos().x - player.position().x;
                float dy = enemy.getPos().y - player.position().y;
                float distSq = dx * dx + dy * dy;

                // Si le joueur est à portée
                float range = enemy.getStats().attackRange;
                if (distSq < (range * range)) {
                    player.takeDamage(10); // Le joueur prend 10 dégâts
                    enemy.setHitDone(true); // Empêche de prendre les dégâts 60 fois par seconde
                }
            }
        }
    }
};