void Player::update(float dt) {
    // Update invulnerability
    if (isInvulnerable) {
        invulnerableTimer -= dt;
        if (invulnerableTimer <= 0.0f) {
            isInvulnerable = false;
            shape.setFillColor(sf::Color::Blue);
        } else {
            // Flash effect
            int alpha = static_cast<int>(std::abs(std::sin(invulnerableTimer * 15.0f)) * 255);
            shape.setFillColor(sf::Color(100, 100, 255, alpha));
        }
    }
    
    // Check if on ground - ✅ CORREGIDO: detección más robusta
    b2Vec2 vel = body->GetLinearVelocity();
    canJump = (vel.y >= -0.5f && vel.y <= 0.5f);  // Mejor tolerancia
}