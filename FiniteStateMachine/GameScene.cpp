#include "GameScene.h"

GameScene::GameScene() {
    // Initialisation de l'arène (ex: spawn du player, ajout d'obstacles)
}

void GameScene::update(float dt) {
    // 1. Update du Player (déplacement via clavier)
    m_player.update(dt);

    // 2. Gestion du Spawner (timer interne pour créer des dummy enemies)
    m_spawner.update(dt, m_world);

    // 3. Update du World (déplacement des ennemis dummy + collisions simples)
    m_world.update(dt);

    // 4. Logique de collision simple (ex: Player vs Obstacles)
    // À faire ici ou dans m_world
}

void GameScene::draw(sf::RenderWindow& window) {
    // L'ordre d'affichage est important pour les couches (Layers)

    m_world.draw(window);   // Dessine les obstacles et les ennemis
    m_player.draw(window);  // Dessine le joueur par-dessus
}
