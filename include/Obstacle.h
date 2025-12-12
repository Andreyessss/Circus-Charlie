#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SFML/Graphics.hpp>

class Obstacle {
private:
    sf::Vector2f position;
    sf::CircleShape shape;
    float radius;
    float speed;
    bool active;
    enum class Type { HOOP, FIRE } type;
    bool passed;
public:
    Obstacle(float x, float y);
    ~Obstacle();
    void update(float dt);
    void render(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    float getRadius() const { return radius; }
    bool isActive() const { return active; }
    bool isOffScreen() const;
    void setTexture(const sf::Texture* tex) { shape.setTexture(tex); }
    void setRadius(float r) { radius = r; shape.setRadius(r); shape.setOrigin(sf::Vector2f(r, r)); }
    void setTypeHoop() { type = Type::HOOP; }
    void setTypeFire() { type = Type::FIRE; }
    bool isHoop() const { return type == Type::HOOP; }
    bool isPassed() const { return passed; }
    void markPassed() { passed = true; }
};

#endif
