#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "core/Scene/Scene.h"
#include "core/Scene/GameScene.h"

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

private:
    sf::RenderWindow m_window;
    std::unique_ptr<Scene> m_scene;
};