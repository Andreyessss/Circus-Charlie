#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <box2d/box2d.h>
#include <memory>
#include <vector>

class Player;
class Platform;
class Obstacle;

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game {
private:
    // Window
    sf::RenderWindow window;
    sf::View gameView;
    
    // State
    GameState currentState;
    int score;
    int lives;
    float gameTime;
    
    // Physics
    std::unique_ptr<b2World> world;
    const float TIME_STEP = 1.0f / 60.0f;
    const int VELOCITY_ITERATIONS = 8;
    const int POSITION_ITERATIONS = 3;
    
    // Objects
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Platform>> platforms;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    
    // Graphics
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    // Audio
    sf::Music music;
    sf::SoundBuffer jumpBuffer;
    sf::SoundBuffer hitBuffer;
    sf::Sound jumpSound;
    sf::Sound hitSound;
    
    // Menu
    std::vector<sf::Text> menuOptions;
    int selectedOption;
    
    // Timing
    sf::Clock clock;
    float obstacleTimer;
    float obstacleSpawnInterval;
    
    // Methods
    void initWindow();
    void initPhysics();
    void initAudio();
    void initMenu();
    void initLevel();
    
    void handleEvents();
    void update(float dt);
    void render();
    
    void updateMenu(sf::Event& event);
    void updateGame(float dt);
    
    void renderMenu();
    void renderGame();
    void renderHUD();
    void renderGameOver();
    
    void spawnObstacle();
    void checkCollisions();
    void loseLife();
    
public:
    Game();
    ~Game();
    
    void run();
    
    b2World* getWorld() { return world.get(); }
    void addScore(int points) { score += points; }
};

#endif