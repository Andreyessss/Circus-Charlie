#include "Player.h"
#include <cmath>

Player::Player(float x, float y)
    : position(x, y), velocity(0.f, 0.f), width(80.f), height(110.f), speed(260.f), jumpForce(-500.f), canJump(false), isInvulnerable(false), invulnerableTimer(0.f), facing(1.f), walkTimer(0.f), scaleMultiplier(1.0f), climbing(false), climbSpeed(120.f), prevY(y)
{
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(sf::Vector2f(width / 2.f, height / 2.f));
    shape.setPosition(position);
}

Player::~Player() {}

void Player::update(float dt) {
    // store previous Y at start of update for hoop detection
    prevY = position.y;
    if (isInvulnerable) {
        invulnerableTimer -= dt;
        if (invulnerableTimer <= 0.0f) {
            isInvulnerable = false;
            shape.setFillColor(sf::Color::Blue);
        } else {
            int alpha = static_cast<int>(std::fabs(std::sin(invulnerableTimer * 15.0f)) * 255);
            shape.setFillColor(sf::Color(100, 100, 255, alpha));
        }
    }

    // simple gravity
    if (!climbing) {
        velocity.y += 980.f * dt; // px/s^2
        position += velocity * dt;
    } else {
        // while climbing, vertical movement is controlled by climbUp/climbDown
        position += velocity * dt;
    }
    shape.setPosition(position);

    // walking animation: when moving horizontally on ground, small scale bob and flip
    if (std::abs(velocity.x) > 1.0f && canJump) {
        walkTimer += dt * 10.0f; // speed of bobbing
        float bob = 0.05f * std::sin(walkTimer);
        scaleMultiplier = 1.0f + bob;
    } else {
        walkTimer = 0.f;
        scaleMultiplier = 1.0f;
    }
    // apply facing and scale
    shape.setScale(sf::Vector2f(facing * scaleMultiplier, scaleMultiplier));

    // ground check (y near ground, e.g., > 500)
    canJump = (std::abs(velocity.y) < 1.0f && position.y >= 500.f) && !climbing;

}

void Player::climbUp() {
    if (climbing) velocity.y = -climbSpeed;
}

void Player::climbDown() {
    if (climbing) velocity.y = climbSpeed;
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}

void Player::landOnPlatform(float platformTop) {
    float h = shape.getSize().y;
    // shape origin is centered, so center y should be platformTop - h/2
    position.y = platformTop - (h / 2.0f);
    velocity.y = 0.f;
    shape.setPosition(position);
    canJump = true;
}

float Player::getBottom() const {
    return position.y + (shape.getSize().y / 2.0f);
}

float Player::getTop() const {
    return position.y - (shape.getSize().y / 2.0f);
}

float Player::getLeft() const {
    return position.x - (shape.getSize().x / 2.0f);
}

float Player::getRight() const {
    return position.x + (shape.getSize().x / 2.0f);
}

float Player::getPrevY() const { return prevY; }

void Player::render(sf::RenderWindow& window) {
    window.draw(shape);
}

void Player::moveLeft() { velocity.x = -speed; facing = -1.f; }
void Player::moveRight() { velocity.x = speed; facing = 1.f; }
void Player::stop() { velocity.x = 0.f; }
void Player::jump() { if (canJump) { velocity.y = jumpForce; canJump = false; } }

void Player::takeDamage() { isInvulnerable = true; invulnerableTimer = 2.0f; // knockback
    velocity.x = (facing > 0.f) ? -300.f : 300.f; velocity.y = -200.f; }

// set texture and adjust shape to texture if available
void Player::setTexture(const sf::Texture* tex) {
    if (tex && tex->getSize().x > 0) {
        shape.setTexture(tex);
        shape.setFillColor(sf::Color::White);
        // scale shape to texture while keeping desired height
        sf::Vector2u tsize = tex->getSize();
        float texAspect = static_cast<float>(tsize.x) / static_cast<float>(tsize.y);
        float desiredH = height;
        float desiredW = desiredH * texAspect;
        shape.setSize(sf::Vector2f(desiredW, desiredH));
        shape.setOrigin(sf::Vector2f(desiredW / 2.f, desiredH / 2.f));
    }
}

void Player::startClimb(float towerX) {
    climbing = true;
    // lock X to towerX (center)
    position.x = towerX;
    velocity.y = 0.f;
}

void Player::stopClimb() {
    climbing = false;
    // when stop climbing, give small downward velocity
    velocity.y = 50.f;
}

sf::Vector2f Player::getPosition() const { return position; }