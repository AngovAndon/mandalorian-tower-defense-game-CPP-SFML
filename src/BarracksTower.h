// BarracksTower.h
#pragma once
#include "TowerBase.h"
#include "TowerType.h"
#include "EnemyBase.h"
#include "Soldier.h"
#include <memory>

class BarracksTower : public TowerBase {
private:
    sf::Sprite sprite;
    sf::Vector2f position;

    float cooldown = 8.f;
    float timeSinceLastSpawn = 0.f;

    std::unique_ptr<Soldier> soldier;
    sf::Texture soldierTexture;
    sf::Vector2f spawnPosition;

    std::vector<std::unique_ptr<EnemyBase>>* enemiesRef = nullptr;

public:
    BarracksTower(const sf::Texture& towerTexture,
        const sf::Texture& soldierTexture,
        const sf::Vector2f& position,
        const sf::Vector2f& spawnPosition);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) const override;

    void attack(std::vector<std::unique_ptr<EnemyBase>>& enemies) override;

    sf::Vector2f getPosition() const override;
    TowerType getType() const override;

    Soldier* getSoldier() const;

    void setEnemyReference(std::vector<std::unique_ptr<EnemyBase>>* ref);
};
