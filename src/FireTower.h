// FireTower.h
#pragma once
#include "TowerBase.h"
#include "TowerType.h"


class FireTower : public TowerBase {
private:
    sf::Sprite sprite;
    sf::Vector2f position;

    float range = 180.f;
    float damage = 10.f;
    float cooldown = 0.6f;
    float timeSinceLastShot = 0.f;

public:
    FireTower(const sf::Texture& texture, const sf::Vector2f& position);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) const override;

    void attack(std::vector<std::unique_ptr<EnemyBase>>& enemies) override;

    sf::Vector2f getPosition() const override;
    TowerType getType() const override;
};
