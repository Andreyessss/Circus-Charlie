void Game::checkCollisions() {
    // ✅ CORREGIDO: lógica invertida en la condición
    if (!player || player->isProtected()) return;  // No colisiona si está protegido
    
    sf::Vector2f playerPos = player->getPosition();
    
    for (auto& obstacle : obstacles) {
        if (!obstacle->isActive()) continue;
        
        sf::Vector2f obstaclePos = obstacle->getPosition();
        float dist = std::sqrt(
            std::pow(playerPos.x - obstaclePos.x, 2) + 
            std::pow(playerPos.y - obstaclePos.y, 2)
        );
        
        if (dist < obstacle->getRadius() + 25.0f) {
            loseLife();
            break;
        }
    }
}