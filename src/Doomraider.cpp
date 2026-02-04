// Doomraider.cpp
#include "Doomraider.h"
#include "EnemyType.h"
#include <cmath>

Doomraider::Doomraider(const sf::Texture& texture, const std::vector<sf::Vector2f>& path)
    : path(path)
{
    sprite.setTexture(texture);
    sprite.setScale(0.12f, 0.12f); // Doomraider size
    position = path[0];
    sprite.setPosition(position);
    currentHealth = maxHealth;
}

EnemyType Doomraider::getType() const {
    return EnemyType::Doomraider;
}


void Doomraider::update(float deltaTime) {
    if (reachedEnd || isDead()) return;

    if (currentWaypoint < path.size()) {
        sf::Vector2f target = path[currentWaypoint];
        sf::Vector2f direction = target - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > 1.f) {
            direction /= distance;
            position += direction * speed * speedMultiplier * deltaTime;
            sprite.setPosition(position);
        }
        else {
            currentWaypoint++;
        }
    }
    else {
        reachedEnd = true;
    }
}

void Doomraider::render(sf::RenderWindow& window) const {
    window.draw(sprite);

    // Health bar
    sf::Vector2f barSize(40.f, 5.f);
    float healthPercent = currentHealth / maxHealth;
    sf::FloatRect bounds = sprite.getGlobalBounds();

    sf::RectangleShape backgroundBar(barSize);
    backgroundBar.setFillColor(sf::Color::Red);
    backgroundBar.setPosition(bounds.left + bounds.width / 2.f - barSize.x / 2.f, bounds.top - 8.f);

    sf::RectangleShape foregroundBar(barSize);
    foregroundBar.setSize(sf::Vector2f(barSize.x * healthPercent, barSize.y));
    foregroundBar.setFillColor(sf::Color::Green);
    foregroundBar.setPosition(backgroundBar.getPosition());

    window.draw(backgroundBar);
    window.draw(foregroundBar);
}

void Doomraider::takeDamage(float amount) {
    currentHealth -= amount;
    if (currentHealth < 0.f) currentHealth = 0.f;
}

bool Doomraider::isDead() const {
    return currentHealth <= 0.f;
}

bool Doomraider::hasReachedEnd() const {
    return currentWaypoint >= path.size();
}

bool Doomraider::hasReachedEndOnce() const {
    return reachedEnd;
}

void Doomraider::markReachedEnd() {
    reachedEnd = true;
}

sf::Vector2f Doomraider::getPosition() const {
    return position;
}

int Doomraider::getGoldReward() const {
    return 40;
}

void Doomraider::setEngaged(bool engaged) {
    engagedWithSoldier = engaged;
}

bool Doomraider::isEngaged() const {
    return engagedWithSoldier;
}

void Doomraider::setSpeedMultiplier(float mult) {
    speedMultiplier = mult;
}
