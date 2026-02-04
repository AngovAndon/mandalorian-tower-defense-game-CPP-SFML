// ArcherTower.cpp
#include "ArcherTower.h"
#include <cmath>

ArcherTower::ArcherTower(const sf::Texture& texture, const sf::Vector2f& pos)
    : position(pos)
{
    sprite.setTexture(texture);
    sprite.setScale(0.13f, 0.13f);
    sprite.setPosition(position);
}

void ArcherTower::update(float deltaTime) {
    timeSinceLastShot += deltaTime;
}

void ArcherTower::render(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void ArcherTower::attack(std::vector<std::unique_ptr<EnemyBase>>& enemies) {
    if (timeSinceLastShot < shootCooldown) return;

    for (auto& enemy : enemies) {
        if (enemy->isDead() || enemy->hasReachedEnd()) continue;

        sf::Vector2f ePos = enemy->getPosition();
        float dx = position.x - ePos.x;
        float dy = position.y - ePos.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= range) {
            enemy->takeDamage(damage);
            timeSinceLastShot = 0.f;
            break; // only one enemy per cooldown
        }
    }
}

sf::Vector2f ArcherTower::getPosition() const {
    return position;
}

TowerType ArcherTower::getType() const {
    return TowerType::Archer;
}
