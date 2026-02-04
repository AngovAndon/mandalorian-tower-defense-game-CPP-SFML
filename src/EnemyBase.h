//EnemyBase.h
#pragma once
#include <SFML/Graphics.hpp>
#include "EnemyType.h"

class EnemyBase {
protected:
    bool engaged = false;
    float speedMultiplier = 1.f;

public:
    virtual EnemyType getType() const = 0;

    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) const = 0;

    virtual void takeDamage(float amount) = 0;
    virtual bool isDead() const = 0;
    virtual bool hasReachedEnd() const = 0;
    virtual bool hasReachedEndOnce() const = 0;
    virtual void markReachedEnd() = 0;

    virtual sf::Vector2f getPosition() const = 0;
    virtual int getGoldReward() const = 0;

    virtual void setEngaged(bool val) { engaged = val; }
    virtual bool isEngaged() const { return engaged; }

    virtual void setSpeedMultiplier(float mult) { speedMultiplier = mult; }
    virtual float getSpeedMultiplier() const { return speedMultiplier; }

    virtual ~EnemyBase() = default;
};
