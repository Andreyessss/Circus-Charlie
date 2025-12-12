#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Obstacle {
private:
    b2Body* body;
    
    sf::CircleShape shape;
    
    float radius;
    float speed;
    bool active;
    
public:
    Obstacle(b2World* world, float x, float y);
    ~Obstacle();
    
    void update(float dt);
    void render(sf::RenderWindow& window);
    
    sf::Vector2f getPosition() const;
    float getRadius() const { return radius; }
    bool isActive() const { return active; }
    bool isOffScreen() const;
};

#endif