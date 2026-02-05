#pragma once
#include "core/Scene.h"
#include "ecs/World.h"
#include "gameplay/Player.h"
#include "gameplay/Spawner.h"

class GameScene : public Scene {
private:
    //World m_world;          // Stockage des entités (obstacles, ennemis)
    //Player m_player;        // Ton instance de joueur
    //Spawner m_spawner;      // Ton système de spawn d'ennemis

public:
    GameScene();

    void update(float dt) override;     // Gestion entrées + mouvement + collisions
    void draw(sf::RenderWindow& window) override; // Rendu de l'arène
};
