// FireTower.cpp
#include "FireTower.h"
#include <cmath>

FireTower::FireTower(const sf::Texture& texture, const sf::Vector2f& pos)
    : position(pos)
{
    sprite.setTexture(texture);
    sprite.setScale(0.13f, 0.13f);
    sprite.setPosition(position);
}

void FireTower::update(float deltaTime) {
    timeSinceLastShot += deltaTime;
}

void FireTower::render(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void FireTower::attack(std::vector<std::unique_ptr<EnemyBase>>& enemies) {
    if (timeSinceLastShot < cooldown) return;

    // FireTower deals damage to all enemies in range
    for (auto& enemy : enemies) {
        if (enemy->isDead() || enemy->hasReachedEnd()) continue;

        sf::Vector2f ePos = enemy->getPosition();
        float dx = position.x - ePos.x;
        float dy = position.y - ePos.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= range) {
            enemy->takeDamage(damage); // low AoE damage
        }
    }

    timeSinceLastShot = 0.f;
}

sf::Vector2f FireTower::getPosition() const {
    return position;
}

TowerType FireTower::getType() const {
    return TowerType::Fire;
}
