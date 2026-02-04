// BombardmentTower.h
#pragma once
#include "TowerBase.h"
#include "TowerType.h"


class BombardmentTower : public TowerBase {
private:
    sf::Sprite sprite;
    sf::Vector2f position;

    float range = 220.f;
    float damage = 16.f;
    float cooldown = 1.8f;
    float timeSinceLastShot = 0.f;

public:
    BombardmentTower(const sf::Texture& texture, const sf::Vector2f& position);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) const override;

    void attack(std::vector<std::unique_ptr<EnemyBase>>& enemies) override;

    sf::Vector2f getPosition() const override;
    TowerType getType() const override;
};
