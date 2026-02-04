// BarracksTower.cpp
#include "BarracksTower.h"
#include <cmath>
#include <iostream>

BarracksTower::BarracksTower(const sf::Texture& towerTex,
    const sf::Texture& soldierTex,
    const sf::Vector2f& pos,
    const sf::Vector2f& spawnPos)
    : position(pos), soldierTexture(soldierTex), spawnPosition(spawnPos)
{
    sprite.setTexture(towerTex);
    sprite.setScale(0.085f, 0.085f);
    sprite.setPosition(position);

    soldier = std::make_unique<Soldier>(soldierTexture, spawnPosition);
}

void BarracksTower::update(float deltaTime) {
    if (soldier) {
        if (soldier->isAlive()) {
            soldier->update(deltaTime);
        }
        else {
            if (enemiesRef)
                soldier->disengage(*enemiesRef); 
            timeSinceLastSpawn += deltaTime;
            if (timeSinceLastSpawn >= cooldown) {
                soldier = std::make_unique<Soldier>(soldierTexture, spawnPosition);
                timeSinceLastSpawn = 0.f;
            }
        }
    }
}

void BarracksTower::attack(std::vector<std::unique_ptr<EnemyBase>>& enemies) {
    if (!soldier || !soldier->isAlive()) return;

    EnemyBase* engaged = soldier->getEngagedEnemy();

    //  Check if engaged enemy still exists in the vector
    bool stillExists = false;
    for (const auto& enemy : enemies) {
        if (enemy.get() == engaged) {
            stillExists = true;
            break;
        }
    }

    //  If already engaged, keep fighting if still valid
    if (engaged && stillExists) {
        if (engaged->isDead() || engaged->hasReachedEnd()) {
            soldier->disengage(enemies);  // Cleanup
        }
        else {
            float dx = soldier->getPosition().x - engaged->getPosition().x;
            float dy = soldier->getPosition().y - engaged->getPosition().y;
            float distSq = dx * dx + dy * dy;

            if (distSq > 30.f * 30.f) {
                soldier->disengage(enemies); // Too far
            }
            else {
                engaged->setEngaged(true);
                engaged->setSpeedMultiplier(0.f);
                engaged->takeDamage(soldier->getDamage() * 0.016f);
                soldier->takeDamage(10.f * 0.016f);
                return;
            }
        }
    }
    else {
        // Pointer was dangling or nullptr
        soldier->disengage(enemies); // Clear it safely
    }

    // Search for a new enemy if none engaged
    for (auto& enemy : enemies) {
        if (enemy->isDead() || enemy->hasReachedEnd() || enemy->isEngaged()) continue;

        float dx = soldier->getPosition().x - enemy->getPosition().x;
        float dy = soldier->getPosition().y - enemy->getPosition().y;
        float distSq = dx * dx + dy * dy;

        if (distSq <= 30.f * 30.f) {
            soldier->setEngagedEnemy(enemy.get());
            enemy->setEngaged(true);
            enemy->setSpeedMultiplier(0.f);
            enemy->takeDamage(soldier->getDamage() * 0.016f);
            soldier->takeDamage(10.f * 0.016f);
            break;
        }
    }
}

void BarracksTower::render(sf::RenderWindow& window) const {
    window.draw(sprite);
    if (soldier && soldier->isAlive()) {
        soldier->render(window);
    }
}

sf::Vector2f BarracksTower::getPosition() const {
    return position;
}

TowerType BarracksTower::getType() const {
    return TowerType::Barracks;
}

Soldier* BarracksTower::getSoldier() const {
    return soldier.get();
}

void BarracksTower::setEnemyReference(std::vector<std::unique_ptr<EnemyBase>>* ref) {
    enemiesRef = ref;
}

