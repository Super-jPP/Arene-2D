#pragma once

#include "core/Scene/Scene.h"
#include "gameplay/Player.h"
#include "gameplay/Spawner.h"
#include "gameplay/Obstacles.h"
#include "gameplay/Weapons.h"
#include "render/RepeatingMap.h"
#include <gameplay/Damage.h>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
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

    // Spawns and owns all enemies (placeholder circles for now)
    Spawner m_spawner;
    Obstacles m_obstacles;

    sf::View m_camera;

    render::RepeatingMap m_map;
    Vec2 m_worldSize{ 0.f, 0.f };

    int m_score = 0;
    sf::Font m_font;
    sf::Text m_scoreText;
};
