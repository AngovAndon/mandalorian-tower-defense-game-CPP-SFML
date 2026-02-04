// ArcherTower.h
#pragma once
#include "TowerBase.h"
#include "TowerType.h"

class ArcherTower : public TowerBase {
private:
    sf::Sprite sprite;
    sf::Vector2f position;

    float range = 200.f;
    float damage = 10.f;
    float shootCooldown = 1.0f;
    float timeSinceLastShot = 0.f;

public:
    ArcherTower(const sf::Texture& texture, const sf::Vector2f& position);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) const override;

    void attack(std::vector<std::unique_ptr<EnemyBase>>& enemies) override;

    sf::Vector2f getPosition() const override;
    TowerType getType() const override;
};
