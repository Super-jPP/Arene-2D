#include "core/Game.h"
#include "core/Scene/GameScene.h"

Game::Game()
    : m_window(sf::VideoMode({ 1920, 1080 }), "Vampire Survivors - FSM")
{
    m_window.setFramerateLimit(60);
    m_scene = std::make_unique<GameScene>();
}

void Game::run()
{
    sf::Clock clock;

    while (m_window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }
    }
}

void Game::update(float dt)
{
    m_scene->update(dt, m_window);
}

void Game::render()
{
    m_window.clear(sf::Color::Black);
    m_scene->draw(m_window);
    m_window.display();
}