#ifndef GAME_HPP
#define GAME_HPP

#include <string>

#include <SFML/Graphics.hpp>

#include "classes.hpp"
#include "random.hpp"
#include "menus.hpp"

namespace AapeliBlox {
  enum gameType {
    undoGame,
    timeTrial,
    normal
  };
  class Game {
    enum endType {
      undoNoBlocks,
      undoTimeOver,
      timeTrialOver,
      tooManyBlocks
    };
    states& gameState;

    // Global game settings
    int blockTowerWidth, blockTowerHeight, blockTowerVisibleHeight, blockSide, totalPoints, featureWidth;
    int nextBlockHeight, textPaddingx, textPaddingy, featurePadding, undoHeight, gameTime, gameTimeMins, secsInMin;
    Vector min, max, features, featuresNextBlock, featuresYourScore, featuresTimer;
    Vector featuresNextBlockDimensions, featuresYourScoreDimensions, featuresTimerDimensions;
    bool drawGhost;

    // Some colors
    sf::Color empty, undo;

    Block emptyBlock, undoBlock;

    // Time between each drop
    double dropInterval, effectInterval;

    // Timers
    sf::Clock drop, effects, gameTimer;

    std::string gameTimePretty;

    int effectsPoints;

    // We need a random number generator
    RandomNumberGenerator& RNG;

    // Vector to store blocks in
    std::vector<std::vector<Block>> blocks;
    sf::VertexArray blockVertexArray;
    int colorIntensity;
    std::vector<Shape> shapes;

    // Grid data
    sf::Color gridLineColor;
    sf::VertexArray grid;

    // Current block data
    Shape currentShape;
    Shape currentShapeGhost;

    Shape nextShape;
    Vector nextShapeLocation;
    sf::VertexArray nextGrid, nextVertexArray;

    Vector currentLocation;
    Vector currentLocationGhost;

    sf::VertexArray featureOutline, featureBoxes;
    sf::Color featureOutlineColor, featureBoxColor;

    Move left, right, down, jump, rotateLeft, rotateRight;
    sf::Clock moveTimer;
    double minMoveInterval;

    // Point calcuation
    std::vector<int> points;
    std::vector<int> weights;

    // Font stuff
    // We need a font to display text with
    // make it a pointer, so it's faster
    sf::Font &sFont, &dFont;
    sf::Text scoreText;
    sf::Text scoreTextNumbers;
    sf::Text nextShapeText;
    sf::Text timerText;
    sf::Text timerTextNumbers;

    sf::Color textColor;

    gameType curGameType;

    MenuEntry &scoreEntry, &endEntry, &timeEntry;

    void drawBlock (int x, int y, sf::Color& color);
    void drawShape (Shape& sh, Vector& loc);
    void drawNext (void);
    void drawPoints (void);
    void drawTimer (void);
    void drawFeatureBox (int x, int y, int width, int height);
    void placeGhost (void);
    void checkRows (void);
    bool canBlockGo (Shape shape, Vector location);
    bool canBlockRotate (Shape shape, Vector location, int rotation);
    void dissolveCurrentBlock (void);
    void placeBlock (void);
    void move (Move& doMove);
    void centerText (sf::Text& text, int x, int y);
    void doEffects (void);
    void createUndoMap (void);
    void calculatePoints (void);
    void endGameLogic (endType type);
  public:
    Game (states& sgameState,
          RandomNumberGenerator& sRNG,
          sf::Font& sdFont,
          sf::Font& ssFont,
          MenuEntry& sscoreEntry,
          MenuEntry& sendEntry,
          MenuEntry& stimeEntry,
          int sblockTowerWidth = 10,
          int sblockTowerHeight = 22,
          int sblockSide = 30,
          int sminx = 50,
          int sminy = 50,
          int sfeaturewidth = 200,
          int sfeaturepadding = 20,
          bool sdrawGhost = true,
          double sdropInterval = 0.5,
          int blockColor = 254,
          double sminMoveInterval = 0.1,
          sf::Color empty = sf::Color(255, 0, 255, 150),
          sf::Color undo = sf::Color(23, 47, 66, 255));
    void newGame (gameType newGameType);
    void spawnNewBlock (void);
    void gameLogic (void);
    void draw (sf::RenderWindow& window);
    void inputLeft (void);
    void inputRight (void);
    void inputDown (void);
    void inputRotateLeft (void);
    void inputRotateRight (void);
    void inputJump (void);
    int getPoints (void);
    void restart (void);
  };
}

#endif //GAME_HPP
