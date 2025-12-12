#include "Obstacle.h"

Obstacle::Obstacle(float x, float y)
        : position(x, y),
          radius(70.0f),
          speed(-220.0f),
          active(true),
          type(Type::FIRE),
          passed(false)
{
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::Red);
    shape.setOutlineColor(sf::Color::Yellow);
    shape.setOutlineThickness(3.0f);
    shape.setOrigin(sf::Vector2f(radius, radius));
    shape.setPosition(position);
}

Obstacle::~Obstacle() {
}

void Obstacle::update(float dt) {
    position.x += speed * dt;
    shape.setPosition(position);
}

void Obstacle::render(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::Vector2f Obstacle::getPosition() const {
    return position;
}

bool Obstacle::isOffScreen() const {
    return (position.x < -100.0f);
}
