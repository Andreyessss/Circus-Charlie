#include "Game.h"
#include "Player.h"
#include "Obstacle.h"
#include "Platform.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <filesystem>

void Game::initWindow() {
    window.create(sf::VideoMode(sf::Vector2u(800, 600)), "Circus Charlie");
    window.setFramerateLimit(60);
    gameView = window.getView();
}

void Game::initLevel() {
    levelLeft = 0.f;
    levelWidth = 5000.f;
    levelRight = levelLeft + levelWidth;
    player = std::make_unique<Player>(100.f, 500.f);
    platforms.clear();
    platforms.emplace_back(std::make_unique<Platform>(levelWidth / 2.f, 560.f, levelWidth, 80.f));
    obstacles.clear();
    obstacleTimer = 0.f;
    // Solo asignar la textura del jugador, nunca la de vidas
    if (charlieTexture.getSize().x > 0) player->setTexture(&charlieTexture);
    obstacleTimer = 0.f;
    obstacleSpawnInterval = 2.0f;
    goalReached = false;
    if (goalTexture.getSize().x > 0) {
        goalShape.setSize(sf::Vector2f(static_cast<float>(goalTexture.getSize().x), static_cast<float>(goalTexture.getSize().y)));
        goalShape.setTexture(&goalTexture);
    } else {
        goalShape.setSize(sf::Vector2f(60.f, 140.f));
        goalShape.setFillColor(sf::Color(50, 200, 50));
    }
    goalShape.setOrigin(goalShape.getSize() / 2.f);
    // Meta al final del nivel
    goalShape.setPosition(sf::Vector2f(levelRight - 60.f, 500.f));
    // Solo una torre, bien posicionada
    float charlieHeight = 80.f; // Ajusta según el sprite real
    float charlieWidth = 40.f;
    towerShape.setSize(sf::Vector2f(charlieWidth * 1.3f, charlieHeight * 1.2f));
    // Torre con fondo transparente y borde visible
    towerShape.setFillColor(sf::Color(120, 80, 40, 180));
    towerShape.setOutlineThickness(3.f);
    towerShape.setOutlineColor(sf::Color::Black);
    towerShape.setOrigin(towerShape.getSize() / 2.f);
    float towerX = levelRight - 120.f;
    float towerY = 500.f;
    if (!platforms.empty()) {
        float platformTop = platforms[0]->getTop();
        towerY = platformTop - (towerShape.getSize().y / 2.f);
    }
    towerShape.setPosition(sf::Vector2f(towerX, towerY));
    if (backgroundTexture.getSize().x > 0) {
        backgroundTexture.setRepeated(true);
        backgroundShape.setSize(sf::Vector2f(levelWidth, static_cast<float>(window.getSize().y)));
        backgroundShape.setTexture(&backgroundTexture);
        backgroundShape.setOrigin(sf::Vector2f(0.f, 0.f));
        backgroundShape.setPosition(sf::Vector2f(levelLeft, 0.f));
        backgroundShape.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(static_cast<int>(levelWidth), static_cast<int>(window.getSize().y))));
    }
}

Game::Game() : currentState(GameState::MENU), score(0), lives(3), gameTime(0.f) {
    initWindow();
    initResources();
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    initLevel();
}

Game::~Game() {}

void Game::run() {
    sf::Clock clk;
    while (window.isOpen()) {
        float dt = clk.restart().asSeconds();
        handleEvents();
        update(dt);
        render();
    }
}

void Game::handleEvents() {
    // Consume events for window/system events (SFML3 event API)
    while (auto ev = window.pollEvent()) {
        if (ev->is<sf::Event::Closed>()) {
            window.close();
            continue;
        }
        if (ev->is<sf::Event::KeyPressed>()) {
            const auto* kp = ev->getIf<sf::Event::KeyPressed>();
            if (!kp) continue;
            auto code = kp->code;
            if (currentState == GameState::MENU) {
                if (code == sf::Keyboard::Key::Space) {
                    currentState = GameState::PLAYING;
                    score = 0; lives = 3;
                    initLevel();
                }
                continue;
            }
            if (currentState == GameState::GAME_OVER) {
                if (code == sf::Keyboard::Key::Space) {
                    score = 0; lives = 3; goalReached = false;
                    initLevel();
                    currentState = GameState::PLAYING;
                } else if (code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
                continue;
            }
            if (code == sf::Keyboard::Key::Left) player->moveLeft();
            if (code == sf::Keyboard::Key::Right) player->moveRight();
            if (code == sf::Keyboard::Key::Space) { if (!player->isClimbing()) { player->jump(); playSound(jumpBuffer); } else { player->stopClimb(); } }
            if (code == sf::Keyboard::Key::Up) {
                if (!player->isClimbing()) {
                    float towerX = towerShape.getPosition().x;
                    if (std::abs(player->getPosition().x - towerX) < 40.f) {
                        player->startClimb(towerX);
                    }
                }
            }
        }
        if (ev->is<sf::Event::KeyReleased>()) {
            const auto* kr = ev->getIf<sf::Event::KeyReleased>();
            if (!kr) continue;
            auto code = kr->code;
            if (code == sf::Keyboard::Key::Left || code == sf::Keyboard::Key::Right) player->stop();
        }
    }

    // Keyboard-driven input (polling) for portability with SFML3
    if (currentState == GameState::MENU) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            currentState = GameState::PLAYING;
            score = 0; lives = 3;
            initLevel();
        }
        return;
    }
    if (currentState == GameState::GAME_OVER) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            score = 0; lives = 3; goalReached = false;
            initLevel();
            currentState = GameState::PLAYING;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            window.close();
        }
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) player->moveLeft();
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) player->moveRight();
    else player->stop();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        if (player && !player->isClimbing()) { player->jump(); playSound(jumpBuffer); }
        else if (player) { player->stopClimb(); }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        if (player && !player->isClimbing()) {
            float towerX = towerShape.getPosition().x;
            if (std::abs(player->getPosition().x - towerX) < 40.f) {
                player->startClimb(towerX);
            }
        }
    }
}

void Game::update(float dt) {
    // No actualizar nada en el menú ni después de llegar a la meta
    if (currentState != GameState::PLAYING) return;
    static bool subiendoTorre = false;
    static float climbY = 0.f;
    if (goalReached && player && !subiendoTorre) {
        // Inicia animación
        subiendoTorre = true;
        climbY = player->getPosition().y;
    }
    if (subiendoTorre && player) {
        float targetY = towerShape.getPosition().y - towerShape.getSize().y / 2.f;
        if (climbY > targetY) {
            climbY -= 100.f * dt; // Velocidad de subida
            if (climbY < targetY) climbY = targetY;
            player->setY(climbY);
        } else {
            subiendoTorre = false;
        }
        return;
    }
    if (goalReached) return;
    updateGame(dt);
}

void Game::updateGame(float dt) {
    gameTime += dt;
    if (player) {
        player->update(dt);
        if (player->isClimbing()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
                player->climbUp();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                player->climbDown();
            } else {
                player->stopClimb();
            }
        }
        for (auto& plat : platforms) {
            float playerBottom = player->getBottom();
            float platformTop = plat->getTop();
            if (playerBottom >= platformTop && playerBottom <= platformTop + 40.0f) {
                player->landOnPlatform(platformTop);
                break;
            }
        }
    }
    for (auto& ob : obstacles) ob->update(dt);
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](const std::unique_ptr<Obstacle>& o){ return o->isOffScreen(); }), obstacles.end());
    obstacleTimer += dt;
    if (obstacleTimer >= obstacleSpawnInterval) {
        spawnObstacle();
        obstacleTimer = 0.f;
    }
    checkCollisions();
    static float debugTimer = 0.f;
    debugTimer += dt;
    if (debugTimer >= 1.0f) {
        debugTimer = 0.f;
        if (player) std::cerr << "DEBUG: player.y=" << player->getPosition().y << " obstacles=" << obstacles.size() << std::endl;
    }
}

void Game::render() {
    if (currentState == GameState::MENU) {
        renderMenu();
        return;
    }
    if (player) {
        float halfW = static_cast<float>(window.getSize().x) / 2.f;
        float camX = player->getPosition().x;
        if (camX < levelLeft + halfW) camX = levelLeft + halfW;
        if (camX > levelRight - halfW) camX = levelRight - halfW;
        gameView.setCenter(sf::Vector2f(camX, static_cast<float>(window.getSize().y) / 2.f));
        window.setView(gameView);
    }
    window.clear(sf::Color::Black);
    renderGame();
    renderHUD();
    if (currentState == GameState::GAME_OVER) {
        window.setView(window.getDefaultView());
        if (font.getInfo().family != "") {
            if (goalReached) messageText->setString("JUEGO COMPLETADO");
            else messageText->setString("JUEGO TERMINADO");
            float px = static_cast<float>(window.getSize().x)/2.f - 150.f;
            float py = static_cast<float>(window.getSize().y)/2.f - 20.f;
            messageText->setPosition(sf::Vector2f(px, py));
            window.draw(*messageText);
            // Opciones de fin de juego
            sf::Text opcionSalir(font, "Salir (ESC)", 16);
            opcionSalir.setFillColor(sf::Color::White);
            opcionSalir.setPosition(sf::Vector2f(px, py + 60.f));
            window.draw(opcionSalir);
            sf::Text opcionRepetir(font, "Repetir nivel (ESPACIO)", 16);
            opcionRepetir.setFillColor(sf::Color::White);
            opcionRepetir.setPosition(sf::Vector2f(px, py + 100.f));
            window.draw(opcionRepetir);
        }
        window.setView(gameView);
    }
    for (auto& p : platforms) p->render(window);
    if (player) player->render(window);
    for (auto& ob : obstacles) ob->render(window);
    window.display();
}

void Game::spawnObstacle() {
    if (!obstacles.empty()) return;
    float spawnX = player ? (player->getPosition().x + static_cast<float>(window.getSize().x) * 0.9f) : (static_cast<float>(window.getSize().x) + 200.f);
    float minY = 320.f;
    float maxY = 420.f;
    float y = minY + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (maxY - minY));
    auto ob = std::make_unique<Obstacle>(spawnX, y);
    if (std::rand() % 2 == 0) {
        ob->setTexture(&hoopTexture);
        float r = 80.f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (60.f));
        ob->setRadius(r);
        ob->setTypeHoop();
    } else {
        ob->setTexture(&fireTexture);
        float r = 40.f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (50.f));
        ob->setRadius(r);
        ob->setTypeFire();
    }
    obstacles.emplace_back(std::move(ob));
}

void Game::initResources() {
    try {
        std::cerr << "Current path: " << std::filesystem::current_path() << std::endl;
    } catch (...) {
    }
    {
        const std::vector<std::string> bgCandidates = {"assets/background.png", "assets/fondo_nivel1.png", "assets/fondo_menu.png"};
        bool loaded = false;
        for (auto &p : bgCandidates) {
            if (backgroundTexture.loadFromFile(p)) {
                backgroundTexture.setRepeated(true);
                loaded = true;
                break;
            }
        }
        if (!loaded) {
            std::cerr << "Failed to load background image (tried background.png, fondo_nivel1.png, fondo_menu.png)" << std::endl;
        }
    }
    {
        const std::vector<std::string> playerCandidates = {"assets/charlie.png", "assets/mono.png"};
        bool ok = false;
        for (auto &p : playerCandidates) if (charlieTexture.loadFromFile(p)) { ok = true; break; }
        if (!ok) std::cerr << "Failed to load player texture (tried charlie.png, mono.png)" << std::endl;
    }
    {
        if (hoopTexture.getSize().x == 0) {
            if (hoopTexture.loadFromFile("assets/hoop.png")) {}
            else if (hoopTexture.loadFromFile("assets/aro_fuego.png")) {}
        }
        if (fireTexture.getSize().x == 0) {
            if (fireTexture.loadFromFile("assets/fire.png")) {}
            else if (fireTexture.loadFromFile("assets/aro_fuego.png")) {}
            else if (fireTexture.loadFromFile("assets/mono.png")) {}
        }
        if (hoopTexture.getSize().x == 0) std::cerr << "Hoop texture not found (tried hoop.png, aro_fuego.png)" << std::endl;
        if (fireTexture.getSize().x == 0) std::cerr << "Fire texture not found (tried fire.png, aro_fuego.png, mono.png)" << std::endl;
    }
    {
        const std::vector<std::string> musicCandidates = {"assets/music.ogg", "assets/music.wav"};
        bool ok = false;
        for (auto &mfile : musicCandidates) {
            if (music.openFromFile(mfile)) { music.setLooping(true); music.play(); ok = true; break; }
        }
        if (!ok) std::cerr << "No background music found (tried music.ogg, music.wav)" << std::endl;
    }
    if (!jumpBuffer.loadFromFile("assets/jump.wav")) std::cerr << "Jump sound not found (assets/jump.wav)" << std::endl;
    if (!hitBuffer.loadFromFile("assets/hit.wav")) std::cerr << "Hit sound not found (assets/hit.wav)" << std::endl;
    if (!goalTexture.loadFromFile("assets/meta.png")) {
        std::cerr << "Goal image assets/meta.png not found" << std::endl;
    }
    auto ensureDir = [&](const std::string &p){ try { std::filesystem::create_directories(std::filesystem::path(p).parent_path()); } catch(...) {} };
    auto writeLE32 = [&](std::ofstream &ofs, uint32_t v){ ofs.put(static_cast<char>(v & 0xFF)); ofs.put(static_cast<char>((v>>8)&0xFF)); ofs.put(static_cast<char>((v>>16)&0xFF)); ofs.put(static_cast<char>((v>>24)&0xFF)); };
    auto writeLE16 = [&](std::ofstream &ofs, uint16_t v){ ofs.put(static_cast<char>(v & 0xFF)); ofs.put(static_cast<char>((v>>8)&0xFF)); };
    auto ensureWavFile = [&](const std::string &path, int seconds){
        if (std::filesystem::exists(path)) return;
        ensureDir(path);
        const uint32_t sampleRate = 44100;
        const uint16_t bitsPerSample = 16;
        const uint16_t channels = 1;
        const uint32_t byteRate = sampleRate * channels * (bitsPerSample/8);
        const uint32_t blockAlign = channels * (bitsPerSample/8);
        const uint32_t dataSize = sampleRate * channels * (bitsPerSample/8) * static_cast<uint32_t>(seconds);
        try {
            std::ofstream ofs(path, std::ios::binary);
            if (!ofs) { std::cerr << "Failed to create WAV placeholder: " << path << std::endl; return; }
            ofs.write("RIFF",4);
            writeLE32(ofs, 36 + dataSize);
            ofs.write("WAVE",4);
            ofs.write("fmt ",4);
            writeLE32(ofs, 16);
            writeLE16(ofs, 1);
            writeLE16(ofs, channels);
            writeLE32(ofs, sampleRate);
            writeLE32(ofs, byteRate);
            writeLE16(ofs, blockAlign);
            writeLE16(ofs, bitsPerSample);
            ofs.write("data",4);
            writeLE32(ofs, dataSize);
            std::vector<char> zeros(dataSize, 0);
            ofs.write(zeros.data(), zeros.size());
            ofs.close();
            std::cerr << "Wrote placeholder WAV: " << path << std::endl;
        } catch(...) {
            std::cerr << "Exception writing WAV placeholder: " << path << std::endl;
        }
    };
    ensureWavFile("assets/music.wav", 3);
    ensureWavFile("assets/jump.wav", 1);
    ensureWavFile("assets/hit.wav", 1);
    if (towerTexture.getSize().x == 0) {
        if (towerTexture.loadFromFile("assets/tower.png")) {}
        else if (towerTexture.loadFromFile("assets/meta.png")) {}
        else if (towerTexture.loadFromFile("assets/mono.png")) {}
        else std::cerr << "Tower image not found (tried tower.png, meta.png, mono.png)" << std::endl;
    }
    sf::Texture groundTexture;
    bool groundHasTexture = false;
    if (groundTexture.loadFromFile("assets/ground.png")) {
        groundTexture.setRepeated(true);
        groundHasTexture = true;
    }
    bool hasFont = false;
    if (font.openFromFile("assets/PressStart2P-Regular.ttf")) {
        hasFont = true;
        pixelFont = font;
        std::cerr << "[DEBUG] Fuente pixel cargada correctamente: PressStart2P-Regular.ttf" << std::endl;
    } else {
        std::cerr << "[ERROR] No se pudo cargar la fuente pixel (PressStart2P-Regular.ttf). El juego se cerrará." << std::endl;
        std::exit(1);
    }
    // Forzar uso de la fuente pixel en todos los textos
    titleText = std::make_unique<sf::Text>(font, "Circus Charlie x Clash Royale", 22); // tamaño más chico
    creditsText = std::make_unique<sf::Text>(font, "By Ceti 2025\nPor: Josue Emir Gonzalez Plascencia y Andre Reyes Perez", 8);
    optionPlayText = std::make_unique<sf::Text>(font, "Play (1 Player)", 12);
    titleText->setFillColor(sf::Color::Yellow);
    creditsText->setFillColor(sf::Color::White);
    optionPlayText->setFillColor(sf::Color::White);
    hudLivesText = std::make_unique<sf::Text>(font, "", 10);
    hudLivesText->setFillColor(sf::Color::White);
    hudScoreText = std::make_unique<sf::Text>(font, "", 10);
    hudScoreText->setFillColor(sf::Color::White);
    messageText = std::make_unique<sf::Text>(font, "", 14);
    messageText->setFillColor(sf::Color::Yellow);
    if (titleText) {
        auto b = titleText->getLocalBounds();
        titleText->setOrigin(sf::Vector2f(b.position.x + b.size.x/2.f, b.position.y + b.size.y/2.f));
    }
}

void Game::playSound(const sf::SoundBuffer& buf) {
    // SFML3: construct sound with buffer
    sounds.emplace_back(buf);
    sounds.back().play();
    // cleanup stopped sounds (use Status enum)
    sounds.erase(std::remove_if(sounds.begin(), sounds.end(), [](const sf::Sound& s){ return s.getStatus() == sf::Sound::Status::Stopped; }), sounds.end());
}

void Game::loseLife() {
    if (player) player->takeDamage();
    playSound(hitBuffer);
    lives--;
    if (lives <= 0) {
        currentState = GameState::GAME_OVER;
        std::cerr << "GAME OVER: no lives left" << std::endl;
    } else {
        // Reiniciar el nivel pero mantener las vidas restantes
        initLevel();
    }
}

void Game::checkCollisions() {
    if (!player || player->isProtected()) return;
    sf::Vector2f playerPos = player->getPosition();
    if (!goalReached) {
        float pLeft = player->getLeft();
        float pRight = player->getRight();
        float pTop = player->getTop();
        float pBottom = player->getBottom();
        sf::Vector2f gpos = goalShape.getPosition();
        sf::Vector2f gsize = goalShape.getSize();
        float gLeft = gpos.x - (gsize.x / 2.f);
        float gRight = gpos.x + (gsize.x / 2.f);
        float gTop = gpos.y - (gsize.y / 2.f);
        float gBottom = gpos.y + (gsize.y / 2.f);
        bool overlap = (pLeft < gRight) && (pRight > gLeft) && (pTop < gBottom) && (pBottom > gTop);
        if (overlap) {
            goalReached = true;
            currentState = GameState::GAME_OVER;
            std::cerr << "LEVEL COMPLETE! Reach the goal." << std::endl;
            return;
        }
    }
    for (auto& obstacle : obstacles) {
        if (!obstacle->isActive()) continue;
        sf::Vector2f obstaclePos = obstacle->getPosition();
        float dx = playerPos.x - obstaclePos.x;
        float dy = playerPos.y - obstaclePos.y;
        float dist = std::sqrt(dx*dx + dy*dy);
        if (obstacle->isHoop()) {
            if (!obstacle->isPassed() && std::abs(playerPos.x - obstaclePos.x) < 8.0f && std::abs(playerPos.y - obstaclePos.y) < obstacle->getRadius()) {
                obstacle->markPassed();
                addScore(100);
            }
        } else {
            if (dist < obstacle->getRadius() + 25.0f) {
                loseLife();
                break;
            }
        }
    }
}

void Game::renderMenu() {
    window.setView(window.getDefaultView());
    window.clear(sf::Color::Black);
    if ((font.getInfo().family != "" || pixelFont.getInfo().family != "") && titleText && creditsText && optionPlayText) {
        sf::Text &t = *titleText;
        t.setCharacterSize(22); // Título más pequeño
        t.setOrigin(sf::Vector2f(0.f, 0.f));
        t.setPosition(sf::Vector2f(
            (window.getSize().x - t.getCharacterSize() * t.getString().getSize()) / 2.f,
            80.f
        ));
        window.draw(t);
        creditsText->setPosition(sf::Vector2f(40.f, 140.f));
        window.draw(*creditsText);
        optionPlayText->setPosition(sf::Vector2f(static_cast<float>(window.getSize().x)/2.f - 80.f, 320.f));
        window.draw(*optionPlayText);
        sf::Text footer(font, "By Ceti 2025 — Circus Charlie x Clash Royale — Play: Space", 12);
        footer.setFillColor(sf::Color::White);
        footer.setPosition(sf::Vector2f(40.f, static_cast<float>(window.getSize().y) - 40.f));
        window.draw(footer);
    }
    window.display();
}

void Game::renderHUD() {
    auto prev = window.getView();
    window.setView(window.getDefaultView());
    if (pixelFont.getInfo().family != "") {
        sf::Text scoreText(pixelFont, std::string("") , 18);
        scoreText.setString(std::string("Score: ") + std::to_string(score));
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(static_cast<float>(window.getSize().x) - 180.f, 10.f));
        sf::CircleShape ornament(10.f);
        ornament.setFillColor(sf::Color::Transparent);
        ornament.setOutlineThickness(4.f);
        ornament.setOutlineColor(sf::Color(200,100,255));
        ornament.setPosition(sf::Vector2f(scoreText.getPosition().x - 30.f, 6.f));
        window.draw(ornament);
        window.draw(scoreText);
        // Mostrar texto "Lives:"
        if (hudLivesText) {
            hudLivesText->setString("Lives:");
            hudLivesText->setPosition(sf::Vector2f(10.f, 10.f));
            window.draw(*hudLivesText);
        }
        // Dibujar iconos de vidas
        float iconX = 80.f;
        float iconY = 8.f;
        for (int i = 0; i < lives; ++i) {
            sf::Sprite lifeIcon(lifesTexture);
            // Escala automática para que el alto sea 24px
            float scale = 1.f;
            if (lifesTexture.getSize().y > 0) scale = 24.f / lifesTexture.getSize().y;
            lifeIcon.setScale(sf::Vector2f(scale, scale));
            lifeIcon.setPosition(sf::Vector2f(iconX + i * (24.f + 8.f), iconY));
            window.draw(lifeIcon);
        }
    } else if (font.getInfo().family != "") {
        if (hudLivesText) {
            hudLivesText->setString(std::string("Lives: ") + std::to_string(lives));
            hudLivesText->setPosition(sf::Vector2f(10.f, 10.f));
            window.draw(*hudLivesText);
        }
        if (hudScoreText) {
            hudScoreText->setString(std::string("Score: ") + std::to_string(score));
            hudScoreText->setPosition(sf::Vector2f(static_cast<float>(window.getSize().x) - 150.f, 10.f));
            window.draw(*hudScoreText);
        }
    }
    window.setView(prev);
}

void Game::renderGame() {
    // Fondo se repite a lo largo de todo el nivel
    if (backgroundTexture.getSize().x > 0) {
        float fondoWidth = levelWidth;
        backgroundShape.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(static_cast<int>(fondoWidth), static_cast<int>(window.getSize().y))));
        backgroundShape.setSize(sf::Vector2f(fondoWidth, static_cast<float>(window.getSize().y)));
        backgroundShape.setPosition(sf::Vector2f(levelLeft, 0.f));
        window.draw(backgroundShape);
    } else {
        sf::RectangleShape sky(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
        sky.setFillColor(sf::Color(100, 149, 237));
        sky.setPosition(sf::Vector2f(0.f, 0.f));
        window.draw(sky);
    }
    window.draw(towerShape);
    // La meta solo desaparece si se ha llegado
    if (!goalReached) {
        window.draw(goalShape);
    }
}
