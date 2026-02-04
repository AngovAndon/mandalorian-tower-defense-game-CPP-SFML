// Doomraider.h
#pragma once
#include "EnemyBase.h"

#include "EnemyType.h"


class Doomraider : public EnemyBase {
private:
    sf::Sprite sprite;
    sf::Vector2f position;
    std::vector<sf::Vector2f> path;
    size_t currentWaypoint = 0;

    float speed = 48.f;
    float maxHealth = 200.f;
    float currentHealth = 200.f;
    float speedMultiplier = 1.f;

    bool reachedEnd = false;
    bool engagedWithSoldier = false;

public:
    Doomraider(const sf::Texture& texture, const std::vector<sf::Vector2f>& path);

    EnemyType getType() const override;

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) const override;

    void takeDamage(float amount) override;
    bool isDead() const override;
    bool hasReachedEnd() const override;
    bool hasReachedEndOnce() const override;
    void markReachedEnd() override;

    sf::Vector2f getPosition() const override;
    int getGoldReward() const override;

    void setEngaged(bool engaged) override;
    bool isEngaged() const override;
    void setSpeedMultiplier(float mult) override;
};
