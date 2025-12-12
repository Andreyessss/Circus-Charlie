#include "Platform.h"

Platform::Platform(b2World* world, float x, float y, float w, float h)
    : width(w), height(h)
{
    // Create physics body
    b2BodyDef bodyDef;
    bodyDef.type = b2BodyType::b2_staticBody;
    bodyDef.position.Set(x / 30.0f, y / 30.0f);
    body = world->CreateBody(&bodyDef);
    
    // Create shape
    b2PolygonShape boxShape;
    boxShape.SetAsBox(width / 60.0f, height / 60.0f);
    
    // Create fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.friction = 0.8f;
    
    body->CreateFixture(&fixtureDef);
    
    // Graphics
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(139, 69, 19)); // Brown
    shape.setOutlineColor(sf::Color(101, 67, 33));
    shape.setOutlineThickness(2.0f);
    shape.setOrigin(width / 2.0f, height / 2.0f);
    shape.setPosition(x, y);
}

Platform::~Platform() {
    if (body && body->GetWorld()) {
        body->GetWorld()->DestroyBody(body);
    }
}

void Platform::render(sf::RenderWindow& window) {
    window.draw(shape);
}