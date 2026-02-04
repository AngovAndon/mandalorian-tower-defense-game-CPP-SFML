#pragma once
#include <SFML/Graphics.hpp>
#include "EnemyBase.h" 

class Soldier {
private:
    sf::Sprite sprite;
    sf::Vector2f position;
    float health = 40.f;
    float damage = 25.f;
    bool active = true;
    bool engaging = false; // true if this soldier is currently fighting an enemy

    EnemyBase* engagedEnemy = nullptr; // <-- NEW: pointer to track which enemy it's fighting

    void disengageInternal();  // private fallback
    
public:
    float getDamage() const { return damage; }
    Soldier(const sf::Texture& texture, sf::Vector2f position);

    void update(float deltaTime);
    void render(sf::RenderWindow& window) const;

    sf::Vector2f getPosition() const;
    bool isAlive() const;
    void takeDamage(float amount);
    void deactivate();
    bool isEngaging() const { return engaging; }
    void setEngaging(bool value) { engaging = value; }

    void setEngagedEnemy(EnemyBase* enemy) { engagedEnemy = enemy; }
    EnemyBase* getEngagedEnemy() const { return engagedEnemy; }

    void disengage(std::vector<std::unique_ptr<EnemyBase>>& enemies);

};
