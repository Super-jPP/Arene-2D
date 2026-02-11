#include "core/Scene/GameScene.h"

#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <string>


GameScene::GameScene() 
    : m_scoreText(m_font)
{
    // Camera 1920x1080
    m_camera.setSize({ 1920.f, 1080.f });

    // Load map
    m_map.loadFromFile("assets/Maps/forest_clear.png", 25, 25);

    const sf::Vector2f ws = m_map.worldSize();
    m_worldSize = { ws.x, ws.y };

    // Give world bounds to player
    m_player.setWorldBounds(m_worldSize);

    // Build static obstacles (CDC requirement)
    m_obstacles.buildDefault({ m_worldSize.x, m_worldSize.y });

    // Init enemy spawner (enemies are simple debug circles for now)
    m_spawner.init(m_worldSize);
    m_spawner.setSpawnRate(4.0f);
    m_spawner.setMaxEnemies(120);

    // Camera starts on player
    m_camera.setCenter({
        m_player.position().x,
        m_player.position().y
        });

    // --- Initialisation Score ---
    m_score = 0;

    // Charge une police (mets un fichier .ttf valide dans tes assets !)
    if (!m_font.openFromFile("assets/arial.ttf")) {
        // Gestion d'erreur si pas de police (optionnel mais conseillé)
        // printf("ERREUR: Impossible de charger la police !\n");
    }

    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(20); // Taille du texte
    m_scoreText.setFillColor(sf::Color::White);
    m_scoreText.setPosition(sf::Vector2f(20.f, 50.f)); // Juste sous la barre de vie
    m_scoreText.setString("Score: 0");
}

void GameScene::update(float dt, sf::RenderWindow& window)
{
    // --- Keyboard input (SFML direct)
    Vec2 moveDir{ 0.f, 0.f };
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) moveDir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) moveDir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) moveDir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) moveDir.x += 1.f;

    // Normalize
    const float len = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
    if (len > 0.001f) {
        moveDir.x /= len;
        moveDir.y /= len;
    }

    // Attack input
    bool wantsAttack = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    // Direction du regard (souris) pour l'attaque
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldMouse = window.mapPixelToCoords(mousePos, m_camera);
    bool isLeft = (worldMouse.x < m_player.position().x);

    // Update Player
    m_player.update(dt, moveDir, wantsAttack);

    Vec2 feet = m_player.feetPosition();
    float r = m_player.feetRadius();
    Vec2 feet0 = feet;

    // 1. On convertit ton Vec2 en sf::Vector2f pour la fonction SFML
    sf::Vector2f feetSF(feet.x, feet.y);

    // 2. On passe la version SFML à la fonction (elle va modifier feetSF si collision)
    if (m_obstacles.resolveCircle(feetSF, r))
    {
        // 3. On calcule le déplacement basé sur le résultat corrigé
        // feetSF contient maintenant la position "sortie du mur"
        const float dx = feetSF.x - feet0.x;
        const float dy = feetSF.y - feet0.y;

        // 4. On applique le déplacement au joueur
        Vec2 currentPos = m_player.position();
        m_player.setPosition({ currentPos.x + dx, currentPos.y + dy });
    }

    // --- Update Weapons & Spawner
    m_weapons.update(dt);
    m_spawner.update(dt, m_player.position(), m_camera);

    // --- COMBATS (Correction de la redéfinition ici) ---

    // On récupère la liste UNE SEULE FOIS ici
    auto& enemiesList = m_spawner.getEnemies();

    // On utilise cette liste partout :
    m_weapons.swingSword(m_player.position(), isLeft, enemiesList);
    Damage::handleSwordCombat(m_player, enemiesList);
    Damage::handleEnemyAttacks(m_player, enemiesList);

    // --- GESTION DU SCORE ---
    int enemiesKilled = m_spawner.pruneDeadEnemies();

    if (enemiesKilled > 0)
    {
        m_score += enemiesKilled * 50;
        m_scoreText.setString("Score: " + std::to_string(m_score));
    }

    if (m_player.isDead())
    {
        printf("GAME OVER\n");
    }

    // --- Camera clamp
    const sf::Vector2f viewSize = m_camera.getSize();
    const sf::Vector2f half = viewSize * 0.5f;

    sf::Vector2f center(m_player.position().x, m_player.position().y);

    center.x = std::clamp(center.x, half.x, m_worldSize.x - half.x);
    center.y = std::clamp(center.y, half.y, m_worldSize.y - half.y);

    m_camera.setCenter(center);
}

void GameScene::draw(sf::RenderWindow& window)
{
    // 1. DESSIN DU MONDE (Vue Caméra)
    window.setView(m_camera);

    window.draw(m_map);
    m_obstacles.draw(window);
    m_spawner.draw(window);
    m_player.draw(window);

    // 2. DESSIN DE L'INTERFACE / HUD (Vue Écran par défaut)
    // On remet la vue par défaut pour que la barre reste "collée" à l'écran
    window.setView(window.getDefaultView());

    // --- Paramètres de la barre de vie ---
    const float paddingX = 20.f;
    const float paddingY = 20.f;
    const float barWidth = 200.f;
    const float barHeight = 20.f;

    // Récupération de la vie
    int hp = m_player.getHp();
    int maxHp = m_player.getMaxHp();
    // Sécurité pour éviter la division par zéro ou barre négative
    float ratio = (maxHp > 0) ? static_cast<float>(std::max(0, hp)) / maxHp : 0.f;

    sf::RectangleShape backBar(sf::Vector2f(barWidth, barHeight));

    // Position : sf::Vector2f obligatoire
    backBar.setPosition(sf::Vector2f(paddingX, paddingY));

    backBar.setFillColor(sf::Color(50, 50, 50));
    backBar.setOutlineColor(sf::Color::White);
    backBar.setOutlineThickness(2.f);

    // Vie actuelle (Rouge)
    sf::RectangleShape frontBar(sf::Vector2f(barWidth * ratio, barHeight));
    frontBar.setPosition(sf::Vector2f(paddingX, paddingY));
    frontBar.setFillColor(sf::Color::Red);

    // Dessin
    window.draw(backBar);
    window.draw(frontBar);
    window.draw(m_scoreText); window.draw(m_scoreText);
}
