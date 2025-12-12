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

void Game::initWindow() {
    window.create(sf::VideoMode(sf::Vector2u{800, 600}), "Circus Charlie");
    window.setFramerateLimit(60);
}

void Game::initLevel() {
    // world / level size
    levelLeft = 0.f;
    levelWidth = 5000.f; // long level
    levelRight = levelLeft + levelWidth;

    player = std::make_unique<Player>(100.f, 500.f);
    // ground platform spanning the level
    platforms.emplace_back(std::make_unique<Platform>(levelWidth / 2.f, 560.f, levelWidth, 80.f));
    // clear existing obstacles and reset timers when starting level
    obstacles.clear();
    obstacleTimer = 0.f;
    // assign textures if available
    player->setTexture(&charlieTexture);
    obstacleTimer = 0.f;
    obstacleSpawnInterval = 2.0f;
    // goal at the far right of the level
    goalReached = false;
    if (goalTexture.getSize().x > 0) {
        goalShape.setSize(sf::Vector2f(static_cast<float>(goalTexture.getSize().x), static_cast<float>(goalTexture.getSize().y)));
        goalShape.setTexture(&goalTexture);
    } else {
        goalShape.setSize(sf::Vector2f(60.f, 140.f));
        goalShape.setFillColor(sf::Color(50, 200, 50));
    }
    goalShape.setOrigin(goalShape.getSize() / 2.f);
    goalShape.setPosition(sf::Vector2f(levelRight - 100.f, 500.f));

    // tower placement near goal and keep it persistent
    if (towerTexture.getSize().x > 0) {
        towerShape.setSize(sf::Vector2f(static_cast<float>(towerTexture.getSize().x), static_cast<float>(towerTexture.getSize().y)));
        towerShape.setTexture(&towerTexture);
    } else {
        towerShape.setSize(sf::Vector2f(120.f, 300.f));
        towerShape.setFillColor(sf::Color(120, 80, 40));
    }
    towerShape.setOrigin(towerShape.getSize() / 2.f);
    towerShape.setPosition(sf::Vector2f(levelRight - 180.f, 500.f - (towerShape.getSize().y/2.f)));

    // Set up background shape now that levelWidth is known
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
    while (const auto eventOpt = window.pollEvent()) {
        const auto& event = *eventOpt;
        if (event.is<sf::Event::Closed>()) {
            window.close();
            continue;
        }

        if (event.is<sf::Event::KeyPressed>()) {
            const auto* kp = event.getIf<sf::Event::KeyPressed>();
            if (kp) {
                // Menu state handling
                if (currentState == GameState::MENU) {
                    if (kp->code == sf::Keyboard::Key::Space) {
                        currentState = GameState::PLAYING;
                        score = 0; lives = 3;
                        initLevel();
                    }
                    continue;
                }

                // Game over handling
                if (currentState == GameState::GAME_OVER) {
                    if (kp->code == sf::Keyboard::Key::Space) {
                        // restart
                        score = 0; lives = 3; goalReached = false;
                        initLevel();
                        currentState = GameState::PLAYING;
                    } else if (kp->code == sf::Keyboard::Key::Escape) {
                        window.close();
                    }
                    continue;
                }

                // Playing state controls
                if (kp->code == sf::Keyboard::Key::Left) player->moveLeft();
                if (kp->code == sf::Keyboard::Key::Right) player->moveRight();
                if (kp->code == sf::Keyboard::Key::Space) { if (!player->isClimbing()) { player->jump(); playSound(jumpBuffer); } else { player->stopClimb(); } }
                if (kp->code == sf::Keyboard::Key::Up) {
                    // try to start climbing if near tower
                    if (!player->isClimbing()) {
                        float towerX = towerShape.getPosition().x;
                        if (std::abs(player->getPosition().x - towerX) < 40.f) {
                            player->startClimb(towerX);
                        }
                    }
                }
            }
        }

        if (event.is<sf::Event::KeyReleased>()) {
            const auto* kr = event.getIf<sf::Event::KeyReleased>();
            if (kr) {
                if (kr->code == sf::Keyboard::Key::Left || kr->code == sf::Keyboard::Key::Right) player->stop();
            }
        }
    }
}

void Game::update(float dt) {
    if (currentState == GameState::PLAYING) updateGame(dt);
}

void Game::updateGame(float dt) {
    gameTime += dt;
    if (player) {
        player->update(dt);
        // climbing controls: allow up/down movement while climbing via keyboard state
        if (player->isClimbing()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
                player->climbUp();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                player->climbDown();
            } else {
                // hold position
                player->stopClimb();
            }
        }
        // Simple platform collision: if player's bottom intersects platform top while falling, snap to platform
        for (auto& plat : platforms) {
            float playerBottom = player->getBottom();
            float platformTop = plat->getTop();
            if (playerBottom >= platformTop && playerBottom <= platformTop + 40.0f) {
                player->landOnPlatform(platformTop);
                break;
            }
        }
    }

    // update obstacles
    for (auto& ob : obstacles) ob->update(dt);
    // remove offscreen
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](const std::unique_ptr<Obstacle>& o){ return o->isOffScreen(); }), obstacles.end());

    // spawn
    obstacleTimer += dt;
    if (obstacleTimer >= obstacleSpawnInterval) {
        spawnObstacle();
        obstacleTimer = 0.f;
    }

    checkCollisions();

    // periodic debug: print player Y and obstacle count once per second
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
        window.display();
        return;
    }

    // set view to follow player
    if (player) {
        float halfW = static_cast<float>(window.getSize().x) / 2.f;
        float camX = player->getPosition().x;
        // clamp camera center to level bounds
        if (camX < levelLeft + halfW) camX = levelLeft + halfW;
        if (camX > levelRight - halfW) camX = levelRight - halfW;
        gameView.setCenter(sf::Vector2f(camX, static_cast<float>(window.getSize().y) / 2.f));
        window.setView(gameView);
    }

    window.clear(sf::Color::Black);
    renderGame();
    // HUD in screen space
    renderHUD();

    // messages for game over or level complete
    if (currentState == GameState::GAME_OVER) {
        window.setView(window.getDefaultView());
        if (font.getInfo().family != "") {
            if (goalReached) messageText->setString("LEVEL COMPLETE!"); else messageText->setString("GAME OVER");
            // position roughly centered
            float px = static_cast<float>(window.getSize().x)/2.f - 150.f;
            float py = static_cast<float>(window.getSize().y)/2.f - 20.f;
            messageText->setPosition(sf::Vector2f(px, py));
            window.draw(*messageText);
        }
        window.setView(gameView);
    }

    for (auto& p : platforms) p->render(window);
    if (player) player->render(window);
    for (auto& ob : obstacles) ob->render(window);
}

void Game::spawnObstacle() {
    // Only spawn a new obstacle if none currently active on screen (one-by-one)
    if (!obstacles.empty()) return;

    // spawn ahead of the player, within reachable vertical range so player must jump
    float spawnX = player ? (player->getPosition().x + static_cast<float>(window.getSize().x) * 0.9f) : (static_cast<float>(window.getSize().x) + 200.f);
    float minY = 320.f; // high enough so player must jump
    float maxY = 420.f; // but not too high
    float y = minY + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (maxY - minY));
    auto ob = std::make_unique<Obstacle>(spawnX, y);
    if (std::rand() % 2 == 0) {
        ob->setTexture(&hoopTexture);
        // random hoop size between 80 and 140
        float r = 80.f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (60.f));
        ob->setRadius(r);
        ob->setTypeHoop();
    } else {
        ob->setTexture(&fireTexture);
        // random fire size between 40 and 90
        float r = 40.f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (50.f));
        ob->setRadius(r);
        ob->setTypeFire();
    }
    obstacles.emplace_back(std::move(ob));
}

void Game::initResources() {
    // Print current working directory to help debugging
    try {
        std::cerr << "Current path: " << std::filesystem::current_path() << std::endl;
    } catch (...) {
    }

    // Background: try several candidate filenames
    {
        const std::vector<std::string> bgCandidates = {"assets/background.png", "assets/fondo_nivel1.png", "assets/fondo_menu.png"};
        bool loaded = false;
        for (auto &p : bgCandidates) {
            if (backgroundTexture.loadFromFile(p)) {
                // make texture repeat across the level; actual shape sizing is done in initLevel
                backgroundTexture.setRepeated(true);
                loaded = true;
                break;
            }
        }
        if (!loaded) {
            std::cerr << "Failed to load background image (tried background.png, fondo_nivel1.png, fondo_menu.png)" << std::endl;
        }
    }

    // Player texture (preferred name: charlie.png)
    {
        const std::vector<std::string> playerCandidates = {"assets/charlie.png", "assets/mono.png"};
        bool ok = false;
        for (auto &p : playerCandidates) if (charlieTexture.loadFromFile(p)) { ok = true; break; }
        if (!ok) std::cerr << "Failed to load player texture (tried charlie.png, mono.png)" << std::endl;
    }

    // Hoop / fire textures: try multiple names found in user assets
    {
        // Try to load hoop texture from common names
        if (hoopTexture.getSize().x == 0) {
            if (hoopTexture.loadFromFile("assets/hoop.png")) {}
            else if (hoopTexture.loadFromFile("assets/aro_fuego.png")) {}
        }
        // Try to load fire texture from common names; fallback to mono.png if no dedicated fire image
        if (fireTexture.getSize().x == 0) {
            if (fireTexture.loadFromFile("assets/fire.png")) {}
            else if (fireTexture.loadFromFile("assets/aro_fuego.png")) {}
            else if (fireTexture.loadFromFile("assets/mono.png")) {}
        }
        if (hoopTexture.getSize().x == 0) std::cerr << "Hoop texture not found (tried hoop.png, aro_fuego.png)" << std::endl;
        if (fireTexture.getSize().x == 0) std::cerr << "Fire texture not found (tried fire.png, aro_fuego.png, mono.png)" << std::endl;
    }

    // Music: optional (try .ogg then .wav)
    {
        const std::vector<std::string> musicCandidates = {"assets/music.ogg", "assets/music.wav"};
        bool ok = false;
        for (auto &mfile : musicCandidates) {
            if (music.openFromFile(mfile)) { music.setLooping(true); music.play(); ok = true; break; }
        }
        if (!ok) std::cerr << "No background music found (tried music.ogg, music.wav)" << std::endl;
    }

    // sound buffers (non-fatal)
    if (!jumpBuffer.loadFromFile("assets/jump.wav")) std::cerr << "Jump sound not found (assets/jump.wav)" << std::endl;
    if (!hitBuffer.loadFromFile("assets/hit.wav")) std::cerr << "Hit sound not found (assets/hit.wav)" << std::endl;

    // load goal image if present
    if (!goalTexture.loadFromFile("assets/meta.png")) {
        std::cerr << "Goal image assets/meta.png not found" << std::endl;
    }

    // If some assets are missing, create simple placeholders on disk so the game can load them.
    auto ensureDir = [&](const std::string &p){ try { std::filesystem::create_directories(std::filesystem::path(p).parent_path()); } catch(...) {} };

    // Image placeholders are not created here (SFML image creation differs across versions).
    // The code will draw colored shapes at runtime when textures are missing.

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
            writeLE16(ofs, 1); // PCM
            writeLE16(ofs, channels);
            writeLE32(ofs, sampleRate);
            writeLE32(ofs, byteRate);
            writeLE16(ofs, blockAlign);
            writeLE16(ofs, bitsPerSample);
            ofs.write("data",4);
            writeLE32(ofs, dataSize);
            // write silence (zeros)
            std::vector<char> zeros(dataSize, 0);
            ofs.write(zeros.data(), zeros.size());
            ofs.close();
            std::cerr << "Wrote placeholder WAV: " << path << std::endl;
        } catch(...) {
            std::cerr << "Exception writing WAV placeholder: " << path << std::endl;
        }
    };

    // do not write image files; runtime will draw colored shapes if textures are missing

    // create simple silent WAV placeholders for music and SFX
    ensureWavFile("assets/music.wav", 3);
    ensureWavFile("assets/jump.wav", 1);
    ensureWavFile("assets/hit.wav", 1);

    // load tower image if present (fallback to meta or mono.png)
    if (towerTexture.getSize().x == 0) {
        if (towerTexture.loadFromFile("assets/tower.png")) {}
        else if (towerTexture.loadFromFile("assets/meta.png")) {}
        else if (towerTexture.loadFromFile("assets/mono.png")) {}
        else std::cerr << "Tower image not found (tried tower.png, meta.png, mono.png)" << std::endl;
    }

    // ground texture (optional pixelated) - try assets/ground.png
    sf::Texture groundTexture;
    bool groundHasTexture = false;
    if (groundTexture.loadFromFile("assets/ground.png")) {
        groundTexture.setRepeated(true);
        groundHasTexture = true;
    }
    // if platforms exist and ground texture available, apply to all platforms
    if (groundHasTexture) {
        for (auto &plat : platforms) {
            // platforms are created later in initLevel; this is best-effort
        }
    }

    // load font: try assets/font.ttf then system Arial
    bool hasFont = false;
    if (font.openFromFile("assets/font.ttf")) hasFont = true;
    else if (font.openFromFile("C:/Windows/Fonts/arial.ttf")) hasFont = true;
    else std::cerr << "Warning: no font found (assets/font.ttf or C:/Windows/Fonts/arial.ttf)" << std::endl;

    bool hasPixel = false;
    if (pixelFont.openFromFile("assets/pixel_font.ttf")) {
        hasPixel = true;
    }

    // prepare menu and HUD text (if font loaded)
    if (hasFont) {
        // title uses pixel font if available
        if (hasPixel) titleText = std::make_unique<sf::Text>(pixelFont, "Circus Charlie x Clash Royale", 72);
        else titleText = std::make_unique<sf::Text>(font, "Circus Charlie x Clash Royale", 72);
        titleText->setFillColor(sf::Color::Yellow);

        creditsText = std::make_unique<sf::Text>(font, "By Ceti 2025\nPor: Josue Emir Gonzalez Plascencia y Andre Reyes Perez", 14);
        creditsText->setFillColor(sf::Color::White);

        optionPlayText = std::make_unique<sf::Text>(font, "Play (1 Player)", 24);
        optionPlayText->setFillColor(sf::Color::White);

        hudLivesText = std::make_unique<sf::Text>(font, "", 18);
        hudLivesText->setFillColor(sf::Color::White);

        hudScoreText = std::make_unique<sf::Text>(font, "", 18);
        hudScoreText->setFillColor(sf::Color::White);

        messageText = std::make_unique<sf::Text>(font, "", 28);
        messageText->setFillColor(sf::Color::Yellow);
    }
}

void Game::playSound(const sf::SoundBuffer& buf) {
    // construct sound with buffer
    sounds.emplace_back(buf);
    sounds.back().play();
    // cleanup stopped sounds
    sounds.erase(std::remove_if(sounds.begin(), sounds.end(), [](const sf::Sound& s){ return s.getStatus() == sf::Sound::Status::Stopped; }), sounds.end());
}

void Game::loseLife() {
    if (player) player->takeDamage();
    // play hit sound
    playSound(hitBuffer);
    lives--;
    if (lives <= 0) {
        // don't forcibly close the window; switch to GAME_OVER state so user can see message
        currentState = GameState::GAME_OVER;
        std::cerr << "GAME OVER: no lives left" << std::endl;
    }
}

void Game::checkCollisions() {
    if (!player || player->isProtected()) return;

    sf::Vector2f playerPos = player->getPosition();

    // check goal by AABB
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
            // if player passes through hoop horizontally near center and not yet counted
            if (!obstacle->isPassed() && std::abs(playerPos.x - obstaclePos.x) < 8.0f && std::abs(playerPos.y - obstaclePos.y) < obstacle->getRadius()) {
                obstacle->markPassed();
                addScore(100);
                // remove hoop by marking inactive
                // we will remove offscreen ones later; deactivate
                // set a small scale or change color
                // here: deactivate so it no longer collides
                // but keep visual: we'll let it remain and not harm player
                // no life lost
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
    // draw menu in screen coordinates
    window.setView(window.getDefaultView());
    window.clear(sf::Color::Black);
    if ((font.getInfo().family != "" || pixelFont.getInfo().family != "") && titleText && creditsText && optionPlayText) {
        // center title properly to avoid clipping
        sf::Text &t = *titleText;
        auto b = t.getLocalBounds();
        t.setOrigin(sf::Vector2f(b.position.x + b.size.x/2.f, b.position.y + b.size.y/2.f));
        t.setPosition(sf::Vector2f(static_cast<float>(window.getSize().x)/2.f, 80.f));
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
}

void Game::renderHUD() {
    // draw HUD (screen space)
    auto prev = window.getView();
    window.setView(window.getDefaultView());
    if (pixelFont.getInfo().family != "") {
        sf::Text scoreText(pixelFont, std::string("") , 18);
        scoreText.setString(std::string("Score: ") + std::to_string(score));
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(static_cast<float>(window.getSize().x) - 180.f, 10.f));
        // ornament circle
        sf::CircleShape ornament(10.f);
        ornament.setFillColor(sf::Color::Transparent);
        ornament.setOutlineThickness(4.f);
        ornament.setOutlineColor(sf::Color(200,100,255));
        ornament.setPosition(sf::Vector2f(scoreText.getPosition().x - 30.f, 6.f));
        window.draw(ornament);
        window.draw(scoreText);
        if (hudLivesText) {
            hudLivesText->setString(std::string("Lives: ") + std::to_string(lives));
            hudLivesText->setPosition(sf::Vector2f(10.f, 10.f));
            window.draw(*hudLivesText);
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
    // Background (world space)
    if (backgroundTexture.getSize().x > 0) {
        window.draw(backgroundShape);
    } else {
        sf::RectangleShape sky(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
        sky.setFillColor(sf::Color(100, 149, 237));
        sky.setPosition(levelLeft, 0.f);
        window.draw(sky);
    }

    // Draw tower and goal (they are positioned in world coordinates)
    window.draw(towerShape);
    window.draw(goalShape);
}