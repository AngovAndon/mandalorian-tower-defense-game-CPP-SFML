// BombardmentTower.cpp
#include "BombardmentTower.h"
#include <cmath>

BombardmentTower::BombardmentTower(const sf::Texture& texture, const sf::Vector2f& pos)
    : position(pos)
{
    sprite.setTexture(texture);
    sprite.setScale(0.13f, 0.13f);
    sprite.setPosition(position);
}

void BombardmentTower::update(float deltaTime) {
    timeSinceLastShot += deltaTime;
}

void BombardmentTower::render(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void BombardmentTower::attack(std::vector<std::unique_ptr<EnemyBase>>& enemies) {
    if (timeSinceLastShot < cooldown) return;

    // Bombardment tower deals damage to all enemies in a medium AoE
    for (auto& enemy : enemies) {
        if (enemy->isDead() || enemy->hasReachedEnd()) continue;

        sf::Vector2f ePos = enemy->getPosition();
        float dx = position.x - ePos.x;
        float dy = position.y - ePos.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= range) {
            enemy->takeDamage(damage);
        }
    }

    timeSinceLastShot = 0.f;
}

sf::Vector2f BombardmentTower::getPosition() const {
    return position;
}

TowerType BombardmentTower::getType() const {
    return TowerType::Bombardment;
}
