#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
    
    // Timing / physics placeholders
    const float TIME_STEP = 1.0f / 60.0f;
    // Removed Box2D world member
    
    // Objects
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Platform>> platforms;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    
    // Graphics
    sf::Font font;
    sf::Font pixelFont;
    // Background and textures
    sf::Texture backgroundTexture;
    sf::RectangleShape backgroundShape;
    sf::Texture charlieTexture;
    sf::Texture hoopTexture;
    sf::Texture fireTexture;
    // Level goal
    sf::RectangleShape goalShape;
    bool goalReached;
    sf::Texture goalTexture;
    // Tower
    sf::Texture towerTexture;
    sf::RectangleShape towerShape;

    // Level dimensions (world coordinates)
    float levelWidth;
    float levelLeft;
    float levelRight;

    // Audio buffers and active sounds
    sf::SoundBuffer jumpBuffer;
    sf::SoundBuffer hitBuffer;
    sf::Music music;
    std::vector<sf::Sound> sounds;
    
    // Menu
    std::vector<sf::Text> menuOptions;
    int selectedOption;
    std::unique_ptr<sf::Text> titleText;
    std::unique_ptr<sf::Text> creditsText;
    std::unique_ptr<sf::Text> optionPlayText;

    // HUD
    std::unique_ptr<sf::Text> hudLivesText;
    std::unique_ptr<sf::Text> hudScoreText;
    std::unique_ptr<sf::Text> messageText;
    
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
    void initResources();
    void playSound(const sf::SoundBuffer& buf);
    
public:
    Game();
    ~Game();
    
    void run();
    
    // No physics world (Box2D removed)
    void addScore(int points) { score += points; }
};

#endif