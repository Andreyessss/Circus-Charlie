#include "Obstacle.h"

Obstacle::Obstacle(b2World* world, float x, float y)
    : radius(30.0f),
      speed(-6.0f),
      active(true)
{
    // Create physics body
    b2BodyDef bodyDef;
    bodyDef.type = b2BodyType::b2_kinematicBody;
    bodyDef.position.Set(x / 30.0f, y / 30.0f);
    body = world->CreateBody(&bodyDef);
    
    // Create shape
    b2CircleShape circleShape;
    circleShape.m_radius = radius / 30.0f;
    
    // Create fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.isSensor = true;
    
    body->CreateFixture(&fixtureDef);
    
    // Set velocity
    body->SetLinearVelocity(b2Vec2(speed, 0.0f));
    
    // Graphics
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::Red);
    shape.setOutlineColor(sf::Color::Yellow);
    shape.setOutlineThickness(3.0f);
    shape.setOrigin(radius, radius);
}

Obstacle::~Obstacle() {
    if (body && body->GetWorld()) {
        body->GetWorld()->DestroyBody(body);
    }
}

void Obstacle::update(float dt) {
    // Keep moving
    body->SetLinearVelocity(b2Vec2(speed, 0.0f));
}

void Obstacle::render(sf::RenderWindow& window) {
    b2Vec2 pos = body->GetPosition();
    shape.setPosition(pos.x * 30.0f, pos.y * 30.0f);
    window.draw(shape);
}

sf::Vector2f Obstacle::getPosition() const {
    b2Vec2 pos = body->GetPosition();
    return sf::Vector2f(pos.x * 30.0f, pos.y * 30.0f);
}

bool Obstacle::isOffScreen() const {
    b2Vec2 pos = body->GetPosition();
    return (pos.x * 30.0f < -100.0f);
}