#include "core/Game.h"
#include "gameplay/GameScene.h" // Ta scène concrète

Game::Game()
    : m_window(sf::VideoMode(1280, 720), "Mon Jeu - Dev B") {
    m_window.setFramerateLimit(60);

    // On initialise la première scène (l'arène)
    m_currentScene = std::make_unique<GameScene>();
}

void Game::run() {
    while (m_window.isOpen()) {
        // 1. Calcul du DeltaTime
        sf::Time elapsed = m_clock.restart();
        float dt = elapsed.asSeconds();

        // 2. Événements (Input)
        processEvents();

        // 3. Logique
        update(dt);

        // 4. Affichage
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            m_window.close();
    }
}

void Game::update(float dt) {
    if (m_currentScene) {
        m_currentScene->update(dt);
    }
}

void Game::render() {
    m_window.clear(sf::Color(30, 30, 30)); // Fond gris foncé pro

    if (m_currentScene) {
        m_currentScene->draw(m_window);
    }

    m_window.display();
}