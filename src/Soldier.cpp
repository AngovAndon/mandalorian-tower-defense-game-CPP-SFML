#include "Soldier.h"
#include "EnemyBase.h"
#include <iostream>
#include <SFML/Graphics.hpp>

Soldier::Soldier(const sf::Texture& texture, sf::Vector2f position)
    : health(30.f), active(true) // initialize hp here too!
{
    sprite.setTexture(texture);
    sprite.setScale(0.08f, 0.08f); // Adjust size as needed
    sprite.setPosition(position);

    this->position = position;
}

void Soldier::update(float deltaTime) {
    // For now, no movement or logic - just exists
}

void Soldier::render(sf::RenderWindow& window) const {
    if (active) {
        window.draw(sprite);
    }
}

sf::Vector2f Soldier::getPosition() const {
    return sprite.getPosition();
}

bool Soldier::isAlive() const {
    return active && health > 0.f;
}

void Soldier::takeDamage(float amount) {
    health -= amount;
    if (health <= 0.f) {
        disengageInternal();  
        active = false;
    }
}


void Soldier::deactivate() {
    active = false;
    disengageInternal();
}

void Soldier::disengage(std::vector<std::unique_ptr<EnemyBase>>& enemies) {
    if (!engagedEnemy) return;

    // Check if engagedEnemy is still in the enemies list
    bool stillExists = false;
    for (const auto& enemy : enemies) {
        if (enemy.get() == engagedEnemy) {
            stillExists = true;
            break;
        }
    }

    if (stillExists) {
        disengageInternal();  // safe cleanup
    }
    else {
        // Enemy already deleted
        engagedEnemy = nullptr;
        engaging = false;
        std::cerr << "[Warning] tried to disengage a destroyed enemy\n";
    }
}


void Soldier::disengageInternal() {
    if (!engagedEnemy) return;

    engagedEnemy->setEngaged(false);
    engagedEnemy->setSpeedMultiplier(1.f);

    engagedEnemy = nullptr;
    engaging = false;
}
