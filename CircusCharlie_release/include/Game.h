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
    sf::Texture lifesTexture;
        sf::Music menuMusic;
        sf::Music levelMusic;
private:
    sf::RenderWindow window;
    sf::View gameView;
    GameState currentState;
    int score;
    int lives;
    float gameTime;
    const float TIME_STEP = 1.0f / 60.0f;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Platform>> platforms;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    sf::Font font;
    sf::Font pixelFont;
    sf::Texture backgroundTexture;
    sf::RectangleShape backgroundShape;
    sf::Texture charlieTexture;
    sf::Texture hoopTexture;
    sf::Texture fireTexture;
    sf::RectangleShape goalShape;
    bool goalReached;
    sf::Texture goalTexture;
    sf::Texture towerTexture;
    sf::RectangleShape towerShape;
    float levelWidth;
    float levelLeft;
    float levelRight;
    sf::SoundBuffer jumpBuffer;
    sf::SoundBuffer hitBuffer;
    std::unique_ptr<sf::Sound> jumpSound;
    sf::Music music;
    std::vector<sf::Sound> sounds;
    std::vector<sf::Text> menuOptions;
    int selectedOption;
    std::unique_ptr<sf::Text> titleText;
    std::unique_ptr<sf::Text> creditsText;
    std::unique_ptr<sf::Text> optionPlayText;
    std::unique_ptr<sf::Text> hudLivesText;
    std::unique_ptr<sf::Text> hudScoreText;
    std::unique_ptr<sf::Text> messageText;
    sf::Clock clock;
    float obstacleTimer;
    float obstacleSpawnInterval;
    void initWindow();
    void initPhysics();
    void initAudio();
    void initMenu();
    void initLevel();
    void handleEvents();
    void update(float dt);
    void render();
    void updateMenu(const sf::Event& event);
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
    void addScore(int points) { score += points; }
};

#endif
