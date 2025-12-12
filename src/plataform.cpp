#include "Platform.h"

Platform::Platform(float x, float y, float w, float h)
    : width(w), height(h)
{
    // Graphics only
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(50,200,50)); // Green ground
    shape.setOutlineColor(sf::Color(40,160,40));
    shape.setOutlineThickness(1.0f);
    shape.setOrigin(sf::Vector2f(width / 2.0f, height / 2.0f));
    shape.setPosition(sf::Vector2f(x, y));
}

Platform::~Platform() {
}

void Platform::render(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Platform::getBounds() const {
    return shape.getGlobalBounds();
}

float Platform::getTop() const {
    // shape origin is centered
    return shape.getPosition().y - (shape.getSize().y / 2.0f);
}