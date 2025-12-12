#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Player {
private:
    sf::Vector2f position;
    sf::Vector2f velocity;

    sf::RectangleShape shape;

    float width;
    float height;
    float speed;
    float jumpForce;
    bool canJump;
    bool isInvulnerable;
    float invulnerableTimer;
    // animation
    float facing; // 1 = right, -1 = left
    float walkTimer;
    float scaleMultiplier;
    // climbing
    bool climbing;
    float climbSpeed;
    float prevY;

public:
    Player(float x, float y);
    ~Player();
    
    void update(float dt);
    void render(sf::RenderWindow& window);
    
    void moveLeft();
    void moveRight();
    void jump();
    void stop();
    
    void takeDamage();
    
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    void landOnPlatform(float platformTop);
    float getBottom() const;
    float getTop() const;
    float getLeft() const;
    float getRight() const;
    float getPrevY() const;
    bool isProtected() const { return isInvulnerable; }  // ✅ CORREGIDO: nombre más claro
    void setCanJump(bool value) { canJump = value; }
    
    // Texture assignment (keep texture owned by Game)
    void setTexture(const sf::Texture* tex);
    // climbing control
    void startClimb(float towerX);
    void stopClimb();
    bool isClimbing() const { return climbing; }
    void climbUp();
    void climbDown();
};

#endif