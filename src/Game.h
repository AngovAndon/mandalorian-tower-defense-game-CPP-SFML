#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "EnemyType.h"
#include "EnemyBase.h"
#include "Goblin.h"
#include "Wolf.h"
#include "Doomraider.h"
#include <memory>  // for unique_ptr
#include "TowerBase.h"
#include "TowerType.h"
#include "ArcherTower.h"
#include "FireTower.h"
#include "BombardmentTower.h"
#include "BarracksTower.h"
enum class GameState {
    MainMenu,
    Level1,
    Level2,
    Level3,
    GameWin,
    GameOver
};
class Game {
public:
    Game();
    void run();
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
private:
    GameState currentState;

    sf::Music backgroundMusic;

    bool fadingInMusic = false;
    bool fadingOutMusic = false;
    float musicFadeTimer = 0.f;
    float musicFadeDuration = 1.5f;  // Duration in seconds

    sf::RenderWindow window;
    sf::Time timePerFrame;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture level1Texture;
    sf::Sprite level1Sprite;

    // Tower Textures for each type
    sf::Texture archerTowerTexture;
    sf::Texture bombardmentTowerTexture;
    sf::Texture fireTowerTexture;
    sf::Texture barracksTowerTexture;

    std::vector<std::unique_ptr<TowerBase>> towers;

    // Tower Selection UI
    sf::RectangleShape towerSelectPanel;
    sf::Sprite towerOptionArcher;
    sf::Sprite towerOptionBombardment;
    sf::Sprite towerOptionFire;
    sf::Sprite towerOptionBarracks;

    bool showTowerOptions = false;

    sf::Text priceArcher;
    sf::Text priceBombardment;
    sf::Text priceFire;
    sf::Text priceBarracks;

    sf::Vector2i selectedBuildTile = { -1, -1 };
    bool isSelectingTower = false;

    sf::Font font;
    sf::Text menuText;

    //our soldier
    sf::Texture soldierTexture;

    //std::vector<Enemy> enemies;
    std::vector<std::unique_ptr<EnemyBase>> enemies;

    sf::Texture goblinTexture;       // Base enemy
    sf::Texture doomraiderTexture;   // Stronger, slower
    sf::Texture wolfTexture;         // Faster, weaker

    float shootTimer = 0.f;  // Keeps track of time between shots
    
    // Starting gold
    int gold = 130; 
    
    // Cost per tower
    const int archerCost = 40;
    const int bombardmentCost = 70;
    const int fireCost = 60;
    const int barracksCost = 50;
    

    sf::Text goldText;         // UI display

    int currentWave = 1;
    int enemiesToSpawn = 5;           // enemies per wave
    float spawnInterval = 1.5f;       // time between spawns
    float spawnTimer = 0.f;

    int spawnedThisWave = 0;
    bool waitingToStartNextWave = false;

    const int totalWaves = 3;
    bool nextWaveRequested = false;

    sf::Text waveText;
    sf::RectangleShape nextWaveButton;
    sf::Text nextWaveLabel;

    bool nextWaveQueued = false;

    //hearts/lives
    int maxLives = 20;
    int currentLives = 20;

    sf::Texture heartTexture;
    sf::Sprite heartIcon;
    sf::Text livesText;

    bool gameEnded = false;
    sf::RectangleShape overlay;
    sf::Text resultText;
    sf::Sprite star1, star2, star3;
    sf::Texture starTexture;

    sf::Text gameOverText;
    sf::RectangleShape gameOverOverlay;
    sf::RectangleShape backToMenuButton;
    sf::Text backToMenuLabel;

    void positionStars(float offsetY = -80.f);

    //left path from the left cave
    std::vector<sf::Vector2f> leftEnemyPath;

    //the right path from the right cave
    std::vector<sf::Vector2f> rightEnemyPath;

    void handleLevelSelection();

    int getEnemyDamage(const EnemyBase* enemy) const;

    void handleEnemySpawning(float deltaTime);

    void handleEnemyCleanupAndGold();

    bool areAllEnemiesHandled() const;

    void handleNextWaveLogic();

    void handleVictoryCondition();

    void checkForGameOver();

    void updateMusicFade(float dt);

    void resetWaveState();
};
