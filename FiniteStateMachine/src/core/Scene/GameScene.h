#pragma once
#include "core/Scene/Scene.h"
#include "ecs/World.h"
#include "gameplay/Player.h"
#include "gameplay/Spawner.h"
#include "input/Input.h"
#include "gameplay/Weapons.h"

class GameScene : public Scene {
public:
    GameScene();

    void update(float dt, sf::RenderWindow& window) override;     // Gestion entrées + mouvement + collisions
    void draw(sf::RenderWindow& window) override; // Rendu de l'arène

private:
    Player m_player;
    Input  m_input;
    Weapons m_weapons;
    World m_world;
};
