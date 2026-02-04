// Wolf.cpp
#include "Wolf.h"
#include "EnemyType.h"
#include <cmath>

Wolf::Wolf(const sf::Texture& texture, const std::vector<sf::Vector2f>& path)
    : path(path)
{
    sprite.setTexture(texture);
    sprite.setScale(0.10f, 0.10f); // Wolf size
    position = path[0];
    sprite.setPosition(position);
    currentHealth = maxHealth;
}

EnemyType Wolf::getType() const {
    return EnemyType::Wolf;
}

void Wolf::update(float deltaTime) {
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

void Wolf::render(sf::RenderWindow& window) const {
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

void Wolf::takeDamage(float amount) {
    currentHealth -= amount;
    if (currentHealth < 0.f) currentHealth = 0.f;
}

bool Wolf::isDead() const {
    return currentHealth <= 0.f;
}

bool Wolf::hasReachedEnd() const {
    return currentWaypoint >= path.size();
}

bool Wolf::hasReachedEndOnce() const {
    return reachedEnd;
}

void Wolf::markReachedEnd() {
    reachedEnd = true;
}

sf::Vector2f Wolf::getPosition() const {
    return position;
}

int Wolf::getGoldReward() const {
    return 25;
}

void Wolf::setEngaged(bool engaged) {
    engagedWithSoldier = engaged;
}

bool Wolf::isEngaged() const {
    return engagedWithSoldier;
}

void Wolf::setSpeedMultiplier(float mult) {
    speedMultiplier = mult;
}
