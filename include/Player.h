#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Player {
private:
    b2Body* body;
    
    sf::RectangleShape shape;
    
    float width;
    float height;
    float speed;
    float jumpForce;
    bool canJump;
    bool isInvulnerable;
    float invulnerableTimer;
    
public:
    Player(b2World* world, float x, float y);
    ~Player();
    
    void update(float dt);
    void render(sf::RenderWindow& window);
    
    void moveLeft();
    void moveRight();
    void jump();
    void stop();
    
    void takeDamage();
    
    sf::Vector2f getPosition() const;
    bool vulnerable() const { return !isInvulnerable; }
    void setCanJump(bool value) { canJump = value; }
};

#endif