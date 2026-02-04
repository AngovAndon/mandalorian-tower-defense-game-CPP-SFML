#include "Game.h"
#include "GridConstants.h"
#include <iostream>
#include <set>
#include <stdexcept>  // for std::runtime_error

#include "Goblin.h"
#include "Wolf.h"
#include "Doomraider.h"

#include "TowerBase.h"
#include "TowerType.h"
#include "ArcherTower.h"
#include "FireTower.h"
#include "BombardmentTower.h"
#include "BarracksTower.h"

const float TOWER_RANGE = 200.f;        // Radius around tower
const float TOWER_DAMAGE = 10.f;        // Damage per hit
const float SHOOT_INTERVAL = 1.f;       // Seconds between shots

std::vector<sf::Vector2f> enemyPath;

//constructor
Game::Game()
:  window(sf::VideoMode::getDesktopMode(), "Mandalorian Tower Defense",
         sf::Style::Titlebar | sf::Style::Close),
    timePerFrame(sf::seconds(1.f / 60.f))
{
    currentState = GameState::MainMenu;

    // --- Fixed "world" view (designed for 1536x1024) ---
    sf::View worldView(sf::FloatRect(0.f, 0.f, 1536.f, 1024.f));

    // Letterbox to keep aspect ratio (no stretching)
    sf::Vector2u ws = window.getSize();
    float windowRatio = (float)ws.x / (float)ws.y;
    float viewRatio = 1536.f / 1024.f;
    float sizeX = 1.f, sizeY = 1.f;
    float posX = 0.f, posY = 0.f;

    if (windowRatio > viewRatio) {
        sizeX = viewRatio / windowRatio;
        posX = (1.f - sizeX) / 2.f;
    } else {
        sizeY = windowRatio / viewRatio;
        posY = (1.f - sizeY) / 2.f;
    }

    worldView.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
    window.setView(worldView);


    if (!backgroundMusic.openFromFile("ThemeSong.ogg")) {
        throw std::runtime_error("Failed to load Mandalorian theme!");
    }
    else {
        backgroundMusic.setLoop(true);      // Keep looping during gameplay
        backgroundMusic.setVolume(36.f);    // Set a nice background volume (0–100)
    }

    if (!backgroundTexture.loadFromFile("MainMap.png"))
        throw std::runtime_error("Failed to load background image!");
    else
        backgroundSprite.setTexture(backgroundTexture);

    if (!level1Texture.loadFromFile("Level1Map.png"))
        throw std::runtime_error("Failed to load level 1 image!");
    else
        level1Sprite.setTexture(level1Texture);

    //4 types of towers
    if (!archerTowerTexture.loadFromFile("ArcherTower.png"))
        throw std::runtime_error("Failed to load ArcherTower image!");

    if (!bombardmentTowerTexture.loadFromFile("BombardmentTower.png"))
        throw std::runtime_error("Failed to load BombardmentTower image!");

    if (!fireTowerTexture.loadFromFile("FireTower.png"))
        throw std::runtime_error("Failed to load FireTower image!");

    if (!barracksTowerTexture.loadFromFile("BarracksTower.png"))
        throw std::runtime_error("Failed to load BarracksTower image!");

    //3 types of enemies
    if (!goblinTexture.loadFromFile("GoblinEnemy.png"))
        throw std::runtime_error("Failed to load GoblinEnemy image!");

    if (!doomraiderTexture.loadFromFile("DoomraiderEnemy.png"))
        throw std::runtime_error("Failed to load DoomraiderEnemy image!");

    if (!wolfTexture.loadFromFile("WolfEnemy.png"))
        throw std::runtime_error("Failed to load WolfEnemy image!");
    
    //our soldiers
    if (!soldierTexture.loadFromFile("Soldier.png"))
        throw std::runtime_error("Failed to load Soldier image!");

    // Left Path from the left cave
    leftEnemyPath = {
        {2 * TILE_SIZE, 5 * TILE_SIZE}, {3 * TILE_SIZE, 5 * TILE_SIZE},
        {4 * TILE_SIZE, 6 * TILE_SIZE}, {4 * TILE_SIZE, 7 * TILE_SIZE},
        {4 * TILE_SIZE, 8 * TILE_SIZE}, {5 * TILE_SIZE, 8 * TILE_SIZE},
        {5 * TILE_SIZE, 9 * TILE_SIZE}, {6 * TILE_SIZE, 9 * TILE_SIZE},
        {7 * TILE_SIZE, 9 * TILE_SIZE}, {8 * TILE_SIZE, 10 * TILE_SIZE},
        {8 * TILE_SIZE, 11 * TILE_SIZE}, {9 * TILE_SIZE, 12 * TILE_SIZE},
        {10 * TILE_SIZE, 13 * TILE_SIZE}, {11 * TILE_SIZE, 13 * TILE_SIZE},
        {12 * TILE_SIZE, 13 * TILE_SIZE}, {13 * TILE_SIZE, 13 * TILE_SIZE},
        {14 * TILE_SIZE, 13 * TILE_SIZE}, {15 * TILE_SIZE, 13 * TILE_SIZE},
        {16 * TILE_SIZE, 12 * TILE_SIZE}, {17 * TILE_SIZE, 11 * TILE_SIZE},
        {18 * TILE_SIZE, 10 * TILE_SIZE}, {19 * TILE_SIZE, 9 * TILE_SIZE},
        {20 * TILE_SIZE, 11 * TILE_SIZE}, {20 * TILE_SIZE, 12 * TILE_SIZE},
        {20 * TILE_SIZE, 13 * TILE_SIZE}, {20 * TILE_SIZE, 14 * TILE_SIZE},
        {21 * TILE_SIZE, 15 * TILE_SIZE}, {22 * TILE_SIZE, 16 * TILE_SIZE},
        {22 * TILE_SIZE, 17 * TILE_SIZE}, {22 * TILE_SIZE, 18 * TILE_SIZE},
        {22 * TILE_SIZE, 19 * TILE_SIZE}
    };

    //Right Path from the right cave
    rightEnemyPath = {
    {18 * TILE_SIZE, 3 * TILE_SIZE},
    {17 * TILE_SIZE, 3 * TILE_SIZE},
    {16 * TILE_SIZE, 3 * TILE_SIZE},
    {15 * TILE_SIZE, 3 * TILE_SIZE},
    {14 * TILE_SIZE, 3 * TILE_SIZE},
    {13 * TILE_SIZE, 4 * TILE_SIZE},
    {13 * TILE_SIZE, 5 * TILE_SIZE},
    {12 * TILE_SIZE, 6 * TILE_SIZE},
    {12 * TILE_SIZE, 7 * TILE_SIZE},
    {11 * TILE_SIZE, 8 * TILE_SIZE},
    {10 * TILE_SIZE, 8 * TILE_SIZE},
    {9 * TILE_SIZE, 9 * TILE_SIZE},
    {8 * TILE_SIZE, 9 * TILE_SIZE},
    {7 * TILE_SIZE, 10 * TILE_SIZE},
    {8 * TILE_SIZE, 10 * TILE_SIZE}, //merge point
    {8 * TILE_SIZE, 11 * TILE_SIZE}, {9 * TILE_SIZE, 12 * TILE_SIZE},
    {10 * TILE_SIZE, 13 * TILE_SIZE}, {11 * TILE_SIZE, 13 * TILE_SIZE},
    {12 * TILE_SIZE, 13 * TILE_SIZE}, {13 * TILE_SIZE, 13 * TILE_SIZE},
    {14 * TILE_SIZE, 13 * TILE_SIZE}, {15 * TILE_SIZE, 13 * TILE_SIZE},
    {16 * TILE_SIZE, 12 * TILE_SIZE}, {17 * TILE_SIZE, 11 * TILE_SIZE},
    {18 * TILE_SIZE, 10 * TILE_SIZE}, {19 * TILE_SIZE, 9 * TILE_SIZE},
    {20 * TILE_SIZE, 11 * TILE_SIZE}, {20 * TILE_SIZE, 12 * TILE_SIZE},
    {20 * TILE_SIZE, 13 * TILE_SIZE}, {20 * TILE_SIZE, 14 * TILE_SIZE},
    {21 * TILE_SIZE, 15 * TILE_SIZE}, {22 * TILE_SIZE, 16 * TILE_SIZE},
    {22 * TILE_SIZE, 17 * TILE_SIZE}, {22 * TILE_SIZE, 18 * TILE_SIZE},
    {22 * TILE_SIZE, 19 * TILE_SIZE}
    };

    // Font
    if (!font.loadFromFile("arial.ttf"))
        throw std::runtime_error("Failed to load font!");
    else {
        menuText.setFont(font);
        menuText.setString("Press 1 / 2 / 3 to choose a level");
        menuText.setCharacterSize(48);
        menuText.setFillColor(sf::Color::White);
        menuText.setPosition(400.f, 800.f);
    }

    waveText.setFont(font);
    waveText.setCharacterSize(28);
    waveText.setFillColor(sf::Color::White);
    waveText.setPosition(30.f, 70.f);
    waveText.setString("Wave 1 of 3");

    //for the choosing one of the 4 towers
    // === Tower Selection Panel and Icons ===
    towerSelectPanel.setSize(sf::Vector2f(300.f, 100.f));
    towerSelectPanel.setFillColor(sf::Color(50, 50, 50, 200));
    towerSelectPanel.setOutlineColor(sf::Color::White);
    towerSelectPanel.setOutlineThickness(2.f);

    towerOptionArcher.setTexture(archerTowerTexture);
    towerOptionBombardment.setTexture(bombardmentTowerTexture);
    towerOptionFire.setTexture(fireTowerTexture);
    towerOptionBarracks.setTexture(barracksTowerTexture);

    towerOptionArcher.setScale(0.08f, 0.08f);
    towerOptionBombardment.setScale(0.08f, 0.08f);
    towerOptionFire.setScale(0.08f, 0.08f);
    towerOptionBarracks.setScale(0.08f, 0.08f);

    priceArcher.setFont(font);
    priceArcher.setCharacterSize(14);
    priceArcher.setFillColor(sf::Color::Yellow);
    priceArcher.setString("40G");

    priceBombardment.setFont(font);
    priceBombardment.setCharacterSize(14);
    priceBombardment.setFillColor(sf::Color::Yellow);
    priceBombardment.setString("60G");

    priceFire.setFont(font);
    priceFire.setCharacterSize(14);
    priceFire.setFillColor(sf::Color::Yellow);
    priceFire.setString("50G");

    priceBarracks.setFont(font);
    priceBarracks.setCharacterSize(14);
    priceBarracks.setFillColor(sf::Color::Yellow);
    priceBarracks.setString("30G");
    //up to here

    // Lives
    if (!heartTexture.loadFromFile("heart.png"))
        throw std::runtime_error("Failed to load heart icon!");
    else {
        heartIcon.setTexture(heartTexture);
        heartIcon.setScale(0.05f, 0.05f);
        heartIcon.setPosition(1430.f, 30.f);
    }

    livesText.setFont(font);
    livesText.setCharacterSize(32);
    livesText.setFillColor(sf::Color::Red);
    livesText.setPosition(1480.f, 32.f);
    livesText.setString(std::to_string(currentLives));

    // Next Wave Button
    nextWaveButton.setSize(sf::Vector2f(200.f, 50.f));
    nextWaveButton.setFillColor(sf::Color(100, 100, 255));
    nextWaveButton.setPosition(30.f, 110.f);

    nextWaveLabel.setFont(font);
    nextWaveLabel.setCharacterSize(22);
    nextWaveLabel.setFillColor(sf::Color::White);
    nextWaveLabel.setString("Start Next Wave");
    nextWaveLabel.setPosition(45.f, 122.f);

    // Overlay
    overlay.setSize(sf::Vector2f(1536, 1024));
    overlay.setFillColor(sf::Color(0, 0, 0, 160)); // darker semi-transparent overlay

    // Result text
    resultText.setFont(font);
    resultText.setCharacterSize(48);
    resultText.setFillColor(sf::Color::White);
    resultText.setString("You Win!"); // Required BEFORE centering

    // Game Over Text
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(64);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setString("Game Over!"); // Required BEFORE centering

    // Back to Menu Button
    backToMenuButton.setSize(sf::Vector2f(250.f, 60.f));
    backToMenuButton.setFillColor(sf::Color(100, 100, 255));

    backToMenuLabel.setFont(font);
    backToMenuLabel.setCharacterSize(26);
    backToMenuLabel.setFillColor(sf::Color::White);
    backToMenuLabel.setString("Back to Menu");

    // === Proper Centering ===
    sf::Vector2f center(1536.f / 2.f, 1024.f / 2.f);

    // Center result text
    sf::FloatRect resultBounds = resultText.getLocalBounds();
    resultText.setOrigin(resultBounds.width / 2.f, resultBounds.height / 2.f);
    resultText.setPosition(center.x, center.y - 150.f);

    // Center game over text
    sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(gameOverBounds.width / 2.f, gameOverBounds.height / 2.f);
    gameOverText.setPosition(center.x, center.y - 150.f);

    if (!starTexture.loadFromFile("star.png")) {
        throw std::runtime_error("Failed to load star image!");
    }
    else {
        star1.setTexture(starTexture);
        star2.setTexture(starTexture);
        star3.setTexture(starTexture);

        star1.setScale(0.1f, 0.1f);
        star2.setScale(0.1f, 0.1f);
        star3.setScale(0.1f, 0.1f);

        // Ensure default color is white so they're visible
        star1.setColor(sf::Color::White);
        star2.setColor(sf::Color::White);
        star3.setColor(sf::Color::White);
    }


    // Center stars (after scale is applied AND ensuring texture is valid)
    if (starTexture.getSize().x > 0) {
        positionStars(); // Default offset = -40.f
    }

    // Center Back to Menu button
    backToMenuButton.setPosition(center.x - backToMenuButton.getSize().x / 2.f, center.y + 80.f);

    // Center label on the button
    sf::FloatRect labelBounds = backToMenuLabel.getLocalBounds();

    // Adjust origin more accurately for vertical centering
    backToMenuLabel.setOrigin(
        labelBounds.left + labelBounds.width / 2.f,
        labelBounds.top + labelBounds.height / 2.f
    );

    // Center exactly on the button
    sf::Vector2f buttonCenter = backToMenuButton.getPosition() + (backToMenuButton.getSize() / 2.f);
    backToMenuLabel.setPosition(buttonCenter);
}

void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        timeSinceLastUpdate += deltaTime;

        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            processEvents();
            update(timePerFrame);
        }

        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        // --- Check for Back to Menu button click (after Win or Loss) ---
        if (gameEnded && event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y }
                );
                if (backToMenuButton.getGlobalBounds().contains(mousePos)) {
                    std::cout << "Returning to main menu.\n";
                    resetWaveState();
                }
            }
        }

        // Level Selection from Main Menu
        if (currentState == GameState::MainMenu && event.type == sf::Event::KeyPressed) {

            auto restartMusicWithFadeIn = [&]() {
                backgroundMusic.stop();                          // restart from beginning
                backgroundMusic.setPlayingOffset(sf::Time::Zero);
                backgroundMusic.setVolume(0.f);                  // start silent
                backgroundMusic.play();                          // start playing now

                fadingInMusic = true;                            // fade in
                fadingOutMusic = false;
                musicFadeTimer = 0.f;
            };

            if (event.key.code == sf::Keyboard::Num1) {
                std::cout << "Level 1 selected\n";
                resetWaveState();
                currentState = GameState::Level1;
                restartMusicWithFadeIn();
            }
            else if (event.key.code == sf::Keyboard::Num2) {
                std::cout << "Level 2 selected\n";
                currentState = GameState::Level2;
                restartMusicWithFadeIn();
            }
            else if (event.key.code == sf::Keyboard::Num3) {
                std::cout << "Level 3 selected\n";
                currentState = GameState::Level3;
                restartMusicWithFadeIn();
            }
        }

        // Tower Placement & Wave Button inside Level1
        if (currentState == GameState::Level1 && event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mouseWorld = window.mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y }
                );

                int x = static_cast<int>(mouseWorld.x / TILE_SIZE);
                int y = static_cast<int>(mouseWorld.y / TILE_SIZE);


                // Tower placement check (2x2 tiles of '3')
                // Tower placement UI trigger
                if (x + 1 < GRID_WIDTH && y + 1 < GRID_HEIGHT) {
                    if (grid[y][x] == 3 && grid[y][x + 1] == 3 &&
                        grid[y + 1][x] == 3 && grid[y + 1][x + 1] == 3) {

                        selectedBuildTile = sf::Vector2i(x, y);
                        showTowerOptions = true;

                        // UI Panel positioning
                        float panelX = x * TILE_SIZE;
                        float panelY = y * TILE_SIZE - 130.f;

                        towerSelectPanel.setSize(sf::Vector2f(360.f, 140.f));
                        towerSelectPanel.setFillColor(sf::Color(0, 0, 0, 180));
                        towerSelectPanel.setPosition(panelX, panelY);

                        float spacing = 20.f;
                        float startX = panelX + spacing;
                        float yTop = panelY + spacing + 10.f;
                        sf::FloatRect iconBounds;
                        sf::FloatRect textBounds;

                        // Tower icon + price: Archer
                        // Archer Tower
                        towerOptionArcher.setTexture(archerTowerTexture);
                        towerOptionArcher.setScale(0.08f, 0.08f); // Make slightly smaller
                        towerOptionArcher.setPosition(startX, yTop);

                        priceArcher.setFont(font);
                        priceArcher.setCharacterSize(18);
                        priceArcher.setFillColor(sf::Color::White);
                        priceArcher.setString(std::to_string(archerCost));

                        // Center price above the icon (rough manual centering)
                        iconBounds = towerOptionArcher.getGlobalBounds();
                        textBounds = priceArcher.getLocalBounds();

                        priceArcher.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
                        priceArcher.setPosition(iconBounds.left + iconBounds.width / 2.f, iconBounds.top - 10.f);

                        // Bombardment Tower
                        towerOptionBombardment.setTexture(bombardmentTowerTexture);
                        towerOptionBombardment.setScale(0.08f, 0.08f);
                        towerOptionBombardment.setPosition(startX + 80.f, yTop);

                        priceBombardment.setFont(font);
                        priceBombardment.setCharacterSize(18);
                        priceBombardment.setFillColor(sf::Color::White);
                        priceBombardment.setString(std::to_string(bombardmentCost));

                        // Center price above the icon (rough manual centering)
                        iconBounds = towerOptionBombardment.getGlobalBounds();
                        textBounds = priceBombardment.getLocalBounds();

                        priceBombardment.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
                        priceBombardment.setPosition(iconBounds.left + iconBounds.width / 2.f, iconBounds.top - 10.f);

                        // Fire Tower
                        towerOptionFire.setTexture(fireTowerTexture);
                        towerOptionFire.setScale(0.08f, 0.08f);
                        towerOptionFire.setPosition(startX + 160.f, yTop);

                        priceFire.setFont(font);
                        priceFire.setCharacterSize(18);
                        priceFire.setFillColor(sf::Color::White); 
                        priceFire.setString(std::to_string(fireCost));

                        // Center price above the icon (rough manual centering)
                        iconBounds = towerOptionFire.getGlobalBounds();
                        textBounds = priceFire.getLocalBounds();

                        priceFire.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
                        priceFire.setPosition(iconBounds.left + iconBounds.width / 2.f, iconBounds.top - 10.f);

                        // Barracks Tower
                        towerOptionBarracks.setTexture(barracksTowerTexture);
                        towerOptionBarracks.setScale(0.055f, 0.055f); // smaller than the other because bigger pic
                        towerOptionBarracks.setPosition(startX + 240.f, yTop);

                        priceBarracks.setFont(font);
                        priceBarracks.setCharacterSize(18);
                        priceBarracks.setFillColor(sf::Color::White);
                        priceBarracks.setString(std::to_string(barracksCost));

                        // Center price above the icon (rough manual centering)
                        iconBounds = towerOptionBarracks.getGlobalBounds();
                        textBounds = priceBarracks.getLocalBounds();

                        priceBarracks.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
                        priceBarracks.setPosition(iconBounds.left + iconBounds.width / 2.f, iconBounds.top - 10.f);
                    }
                }

                //handle the selection of the exact tower type
                if (showTowerOptions) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(
                { event.mouseButton.x, event.mouseButton.y }
                    );


                    sf::Vector2f buildPos(selectedBuildTile.x * TILE_SIZE, selectedBuildTile.y * TILE_SIZE);

                    auto placeTower = [&](TowerType type, const sf::Texture& texture, int cost) {
                        if (gold >= cost) {
                            switch (type) {
                            case TowerType::Archer:
                                towers.push_back(std::make_unique<ArcherTower>(texture, buildPos));
                                break;
                            case TowerType::Fire:
                                towers.push_back(std::make_unique<FireTower>(texture, buildPos));
                                break;
                            case TowerType::Bombardment:
                                towers.push_back(std::make_unique<BombardmentTower>(texture, buildPos));
                                break;
                            case TowerType::Barracks: {
                                float searchRadius = 3 * TILE_SIZE;
                                sf::Vector2f closestPathTile = buildPos;
                                float minDist = std::numeric_limits<float>::max();

                                auto checkPath = [&](const std::vector<sf::Vector2f>& path) {
                                    for (const auto& point : path) {
                                        float dist = std::hypot(buildPos.x - point.x, buildPos.y - point.y);
                                        if (dist < minDist && dist <= searchRadius) {
                                            minDist = dist;
                                            closestPathTile = point;
                                        }
                                    }
                                    };

                                checkPath(leftEnemyPath);
                                checkPath(rightEnemyPath);

                                towers.push_back(std::make_unique<BarracksTower>(
                                    texture, soldierTexture, buildPos, closestPathTile));

                                auto* newTower = dynamic_cast<BarracksTower*>(towers.back().get());
                                if (newTower) {
                                    newTower->setEnemyReference(&enemies);
                                }

                                break;
                            }
                            }

                            gold -= cost;

                            grid[selectedBuildTile.y][selectedBuildTile.x] = 4;
                            grid[selectedBuildTile.y][selectedBuildTile.x + 1] = 4;
                            grid[selectedBuildTile.y + 1][selectedBuildTile.x] = 4;
                            grid[selectedBuildTile.y + 1][selectedBuildTile.x + 1] = 4;

                            goldText.setString("Gold: " + std::to_string(gold));
                            showTowerOptions = false;
                        }
                        };


                    if (towerOptionArcher.getGlobalBounds().contains(mousePos)) {
                        placeTower(TowerType::Archer, archerTowerTexture, archerCost);
                    }
                    else if (towerOptionBombardment.getGlobalBounds().contains(mousePos)) {
                        placeTower(TowerType::Bombardment, bombardmentTowerTexture, bombardmentCost);
                    }
                    else if (towerOptionFire.getGlobalBounds().contains(mousePos)) {
                        placeTower(TowerType::Fire, fireTowerTexture, fireCost);
                    }
                    else if (towerOptionBarracks.getGlobalBounds().contains(mousePos)) {
                        placeTower(TowerType::Barracks, barracksTowerTexture, barracksCost);
                    }
                }


                // Next Wave Button click
                sf::Vector2f mousePos = window.mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y }
                );
                if (nextWaveButton.getGlobalBounds().contains(mousePos)) {
                    if (currentWave < totalWaves && !nextWaveRequested && !nextWaveQueued) {
                        if (spawnedThisWave < enemiesToSpawn) {
                            nextWaveQueued = true;
                            std::cout << "Next wave manually queued!\n";
                        }
                        else {
                            nextWaveRequested = true;
                            std::cout << "Next wave manually triggered!\n";
                        }
                    }
                }
                // === Close tower selection panel if clicked elsewhere ===

                if (showTowerOptions &&
                    !towerOptionArcher.getGlobalBounds().contains(mousePos) &&
                    !towerOptionBombardment.getGlobalBounds().contains(mousePos) &&
                    !towerOptionFire.getGlobalBounds().contains(mousePos) &&
                    !towerOptionBarracks.getGlobalBounds().contains(mousePos) &&
                    !(grid[y][x] == 3 && grid[y][x + 1] == 3 && grid[y + 1][x] == 3 && grid[y + 1][x + 1] == 3)) {
                    showTowerOptions = false;
                }
            }
        }
    }
}

void Game::handleLevelSelection() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        std::cout << "Level 1 selected!" << std::endl;
        resetWaveState();
        currentState = GameState::Level1;

        if (backgroundMusic.getStatus() != sf::Music::Playing) {
            backgroundMusic.setVolume(0.f);
            backgroundMusic.play();
            fadingInMusic = true;
            musicFadeTimer = 0.f;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
        currentState = GameState::Level2;
        std::cout << "Level 2 selected!" << std::endl;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
        currentState = GameState::Level3;
        std::cout << "Level 3 selected!" << std::endl;
    }
}

int Game::getEnemyDamage(const EnemyBase* enemy) const {
    switch (enemy->getType()) {
    case EnemyType::Goblin: return 1;
    case EnemyType::Wolf: return 2;
    case EnemyType::Doomraider: return 3;
    default: return 1;
    }
}

void Game::handleEnemySpawning(float deltaTime) {
    spawnTimer += deltaTime;

    if (spawnedThisWave < enemiesToSpawn && spawnTimer >= spawnInterval) {
        spawnTimer = 0.f;

        EnemyType type;
        if (spawnedThisWave < enemiesToSpawn / 2) {
            type = EnemyType::Goblin;
        }
        else {
            type = (spawnedThisWave % 2 == 0) ? EnemyType::Doomraider : EnemyType::Wolf;
        }

        const std::vector<sf::Vector2f>& chosenPath = (spawnedThisWave < enemiesToSpawn / 2)
            ? leftEnemyPath
            : rightEnemyPath;

        switch (type) {
        case EnemyType::Goblin:
            enemies.push_back(std::make_unique<Goblin>(goblinTexture, chosenPath));
            break;
        case EnemyType::Doomraider:
            enemies.push_back(std::make_unique<Doomraider>(doomraiderTexture, chosenPath));
            break;
        case EnemyType::Wolf:
            enemies.push_back(std::make_unique<Wolf>(wolfTexture, chosenPath));
            break;
        }

        spawnedThisWave++;

        if (spawnedThisWave == enemiesToSpawn && nextWaveQueued && currentWave < totalWaves) {
            nextWaveRequested = true;
            nextWaveQueued = false;
            std::cout << "Next wave auto-triggered after finishing current spawn.\n";
        }
    }
}

void Game::handleEnemyCleanupAndGold() {
    int goldEarned = 0;

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [&](const std::unique_ptr<EnemyBase>& e) {
                if (e->isDead()) {
                    goldEarned += e->getGoldReward();
                    return true;
                }
                return false;
            }),
        enemies.end()
    );

    gold += goldEarned;
    goldText.setString("Gold: " + std::to_string(gold));
}

bool Game::areAllEnemiesHandled() const {
    for (const auto& e : enemies) {
        if (!e->isDead() && !e->hasReachedEnd()) {
            return false;
        }
    }
    return true;
}

void Game::handleNextWaveLogic() {
    if ((waitingToStartNextWave ||
        (nextWaveRequested && spawnedThisWave >= enemiesToSpawn))
        && currentWave < totalWaves)
    {
        currentWave++;
        enemiesToSpawn += 2;
        spawnedThisWave = 0;
        spawnTimer = 0.f;
        waitingToStartNextWave = false;
        nextWaveRequested = false;
        nextWaveQueued = false;

        waveText.setString("Wave " + std::to_string(currentWave) + " of " + std::to_string(totalWaves));
        std::cout << "Wave " << currentWave << " started!\n";
    }
}

void Game::handleVictoryCondition() {
    if (!gameEnded && currentWave == totalWaves && spawnedThisWave >= enemiesToSpawn) {
        bool allEnemiesHandled = areAllEnemiesHandled();

        if (allEnemiesHandled) {
            gameEnded = true;
            currentState = GameState::GameWin;

            fadingOutMusic = true;
            musicFadeTimer = 0.f;

            if (currentLives > 0) {
                resultText.setString("You Win!");

                sf::FloatRect resultBounds = resultText.getLocalBounds();
                resultText.setOrigin(resultBounds.width / 2.f, resultBounds.height / 2.f);
                sf::Vector2f center(1536.f / 2.f, 1024.f / 2.f);
                resultText.setPosition(center.x, center.y - 150.f);

                int stars = 0;
                if (currentLives >= 17) stars = 3;
                else if (currentLives >= 10) stars = 2;
                else if (currentLives >= 3) stars = 1;

                star1.setColor(stars >= 1 ? sf::Color::White : sf::Color(255, 255, 255, 50));
                star2.setColor(stars >= 2 ? sf::Color::White : sf::Color(255, 255, 255, 50));
                star3.setColor(stars == 3 ? sf::Color::White : sf::Color(255, 255, 255, 50));

                positionStars();

                std::cout << "Game Finished! Lives left: " << currentLives << " -> Stars: " << stars << "\n";
            }
            else {
                gameOverText.setString("Game Over!");
                std::cout << "Game Lost at end of wave.\n";
            }
        }
    }
}

void Game::checkForGameOver() {
    if (currentLives <= 0 && currentState == GameState::Level1) {
        gameEnded = true;
        currentState = GameState::GameOver;
        fadingOutMusic = true;
        musicFadeTimer = 0.f;
        std::cout << "You lost! All lives are gone.\n";
    }
}

void Game::updateMusicFade(float dt) {
    if (fadingInMusic) {
        musicFadeTimer += dt;
        float t = std::min(musicFadeTimer / musicFadeDuration, 1.f);
        backgroundMusic.setVolume(t * 36.f);

        if (t >= 1.f) {
            fadingInMusic = false;
        }
    }

    if (fadingOutMusic) {
        musicFadeTimer += dt;
        float t = std::min(musicFadeTimer / musicFadeDuration, 1.f);
        backgroundMusic.setVolume((1.f - t) * 36.f);

        if (t >= 1.f) {
            fadingOutMusic = false;
            backgroundMusic.stop();
        }
    }
}

void Game::update(sf::Time deltaTime) {
        // if (currentState == GameState::MainMenu) {
        //     handleLevelSelection();
        // }
        // Must update enemy movement AFTER engagement handling
        for (auto& enemy : enemies) {
            enemy->update(deltaTime.asSeconds());
        }
        // Check for reached-end enemies and subtract lives
        for (auto& enemy : enemies) {
            if (enemy->hasReachedEnd() && !enemy->hasReachedEndOnce()) {
                int damage = getEnemyDamage(enemy.get());

                currentLives -= damage;
                if (currentLives < 0) currentLives = 0;

                enemy->markReachedEnd();                
                std::cout << "An enemy reached the end! Lives left: " << currentLives << "\n";
            }
        }
        checkForGameOver();
        livesText.setString(std::to_string(currentLives));
        // Step 2: Towers shoot at nearest enemies every X seconds
        for (auto& tower : towers) {
            tower->update(deltaTime.asSeconds());
            tower->attack(enemies);
        }
        // --- Wave Spawning ---
        handleEnemySpawning(deltaTime.asSeconds());
        // Check if all enemies are done (killed or reached end)
        bool allEnemiesHandled = areAllEnemiesHandled();
        if (spawnedThisWave >= enemiesToSpawn && allEnemiesHandled && currentWave < totalWaves && !waitingToStartNextWave) {
            waitingToStartNextWave = true;
            std::cout << "Wave " << currentWave << " completed.\n";
        }
        // --- Manual or Auto Start Next Wave ---
        handleNextWaveLogic();
        handleEnemyCleanupAndGold();
        // --- Final Result Screen Logic ---
        handleVictoryCondition();
        // === MUSIC FADE LOGIC ===
        updateMusicFade(deltaTime.asSeconds());
}

void Game::render() {
    window.clear();

    if (currentState == GameState::MainMenu) {
        window.draw(backgroundSprite);
        window.draw(menuText);
    }

    else if (currentState == GameState::Level1 || currentState == GameState::GameOver || currentState == GameState::GameWin) {

        // Draw the background map for Level 1
        window.draw(level1Sprite);
        window.draw(goldText);

        //Draw wave display + button
        window.draw(waveText);
        window.draw(nextWaveButton);
        window.draw(nextWaveLabel);

        // Lives (hearts) UI
        window.draw(heartIcon);
        window.draw(livesText);

        // Draw buildable tower tiles (where grid[row][col] == 3)
        for (int row = 0; row < GRID_HEIGHT; ++row) {
            for (int col = 0; col < GRID_WIDTH; ++col) {
                if (grid[row][col] == 3) {
                    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE - 2, TILE_SIZE - 2));
                    tile.setFillColor(sf::Color(0, 0, 255, 100)); // Blue semi-transparent
                    tile.setPosition(col * TILE_SIZE, row * TILE_SIZE);
                    window.draw(tile);
                }
            }
        }

        // --- Show Tower Selection UI if active ---
        if (showTowerOptions) {
            window.draw(towerSelectPanel);
            window.draw(towerOptionArcher);
            window.draw(priceArcher);

            window.draw(towerOptionBombardment);
            window.draw(priceBombardment);

            window.draw(towerOptionFire);
            window.draw(priceFire);

            window.draw(towerOptionBarracks);
            window.draw(priceBarracks);
        }

        // Draw all placed towers
        for (const auto& tower : towers) {
            tower->render(window);
        }

        // Draw all enemies
        for (auto& enemy : enemies) {
            enemy->render(window);
        }


        //show the final result popup if game ended
        // Show overlay when game ends (either win or loss)
        // --- Game End Screen ---
        if (gameEnded) {
            window.draw(overlay);

            if (currentLives > 0) {
                // WIN SCREEN
                window.draw(resultText);  // "You Win!"
                window.draw(star1);
                window.draw(star2);
                window.draw(star3);
            }
            else {
                // LOSS SCREEN
                window.draw(gameOverText);  // "Game Over!"
            }

            // In both cases show button
            window.draw(backToMenuButton);
            window.draw(backToMenuLabel);
        }



    }

    // TODO: Add logic for Level2 and Level3 if needed

    window.display();
}

void Game::positionStars(float offsetY) {
    sf::Vector2f center(1536.f / 2.f, 1024.f / 2.f);

    float spacing = 80.f;
    float scaledWidth = star1.getGlobalBounds().width;

    float totalWidth = 3 * scaledWidth + 2 * spacing;
    float startX = center.x - totalWidth / 2.f;
    float starY = center.y + offsetY;

    star1.setPosition(startX, starY);
    star2.setPosition(startX + scaledWidth + spacing, starY);
    star3.setPosition(startX + 2 * (scaledWidth + spacing), starY);
}

void Game::resetWaveState() {
    currentState = GameState::MainMenu;
    gameEnded = false;
    currentWave = 1;
    enemiesToSpawn = 5;
    spawnedThisWave = 0;
    spawnTimer = 0.f;
    waitingToStartNextWave = false;
    nextWaveRequested = false;
    nextWaveQueued = false;

    gold = 130;
    currentLives = maxLives;

    // Clear out all old enemies, towers, soldiers, barracks state
    enemies.clear();
    towers.clear();

    // Reset grid tiles to allow building again
    for (int row = 0; row < GRID_HEIGHT; ++row) {
        for (int col = 0; col < GRID_WIDTH; ++col) {
            grid[row][col] = originalGrid[row][col];
        }
    }

    waveText.setString("Wave 1 of 3");
    goldText.setFont(font);
    goldText.setCharacterSize(32);
    goldText.setFillColor(sf::Color::Yellow);
    goldText.setPosition(30.f, 30.f);
    goldText.setString("Gold: " + std::to_string(gold));
    livesText.setString(std::to_string(currentLives));

    // ⭐ Reset stars visibility
    star1.setColor(sf::Color::White);
    star2.setColor(sf::Color::White);
    star3.setColor(sf::Color::White);


    std::cout << "[Reset] Wave state fully reset for new game\n";
}
