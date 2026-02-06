#pragma once

#include "core/Scene/Scene.h"
#include "gameplay/Player.h"
#include "gameplay/Spawner.h"
#include "gameplay/Weapons.h"
#include "render/RepeatingMap.h"

#include <SFML/Graphics/View.hpp>

class GameScene : public Scene
{
public:
    GameScene();

    void update(float dt, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;

private:
    Player  m_player;
    Weapons m_weapons;

    sf::View m_camera;

    render::RepeatingMap m_map;
    Vec2 m_worldSize{ 0.f, 0.f };
};
