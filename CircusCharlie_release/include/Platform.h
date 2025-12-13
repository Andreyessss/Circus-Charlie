#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>

class Platform {
private:
    sf::RectangleShape shape;
    float width;
    float height;
public:
    Platform(float x, float y, float w, float h);
    ~Platform();
    void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    float getTop() const;
};

#endif

