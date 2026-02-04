#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "EnemyBase.h"
#include "TowerType.h"


class TowerBase {
public:
    virtual ~TowerBase() = default;

    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) const = 0;

    virtual void attack(std::vector<std::unique_ptr<EnemyBase>>& enemies) = 0;

    virtual sf::Vector2f getPosition() const = 0;
    virtual TowerType getType() const = 0;
};
