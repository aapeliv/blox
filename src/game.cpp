#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "classes.hpp"
#include "random.hpp"

#include "game.hpp"

namespace AapeliBlox {
  Game::Game (states& sgameState,
              RandomNumberGenerator& sRNG,
              sf::Font& sdFont,
              sf::Font& ssFont,
              MenuEntry& sscoreEntry,
              MenuEntry& sendEntry,
              MenuEntry& stimeEntry,
              int sblockTowerWidth,
              int sblockTowerHeight,
              int sblockSide,
              int sminx,
              int sminy,
              int sfeaturewidth,
              int sfeaturepadding,
              bool sdrawGhost,
              double sdropInterval,
              int blockColor,
              double sminMoveInterval,
              sf::Color sempty,
              sf::Color sundo) : gameState(sgameState),
                                 RNG(sRNG),
                                 sFont(ssFont),
                                 dFont(sdFont),
                                 scoreEntry(sscoreEntry),
                                 endEntry(sendEntry),
                                 timeEntry(stimeEntry) {
    blockTowerWidth = sblockTowerWidth;
    blockTowerHeight = sblockTowerHeight;
    blockTowerVisibleHeight = blockTowerHeight - 2;
    blockSide = sblockSide;
    min = Vector(sminx, sminy);
    drawGhost = sdrawGhost;
    dropInterval = sdropInterval;
    max = Vector(min.x + blockTowerWidth * blockSide,
                 min.y + blockTowerVisibleHeight * blockSide);

    gameTimePretty = "00:00";

    // Set the colors
    empty = sempty;
    undo = sundo;

    emptyBlock = Block(empty, false);
    undoBlock = Block(undo, true, true);

    // Create a grid
    gridLineColor = sf::Color(100, 100, 100, 255);
    grid = sf::VertexArray(sf::Lines, 0);
    for (int a = 0; a <= blockTowerWidth; a++) {
      grid.append(sf::Vertex(sf::Vector2f(min.x + a * blockSide, min.y), gridLineColor));
      grid.append(sf::Vertex(sf::Vector2f(min.x + a * blockSide, max.y), gridLineColor));
    }
    for (int a = 0; a <= blockTowerVisibleHeight; a++) {
      grid.append(sf::Vertex(sf::Vector2f(min.x, min.y + a * blockSide), gridLineColor));
      grid.append(sf::Vertex(sf::Vector2f(max.x, min.y + a * blockSide), gridLineColor));
    }

    // Create empty (quad) Vertex Array for blocks
    blockVertexArray = sf::VertexArray(sf::Quads, 0);

    nextVertexArray = sf::VertexArray(sf::Quads, 0);
    nextGrid = sf::VertexArray(sf::Lines, 0);

    featureBoxes = sf::VertexArray(sf::Quads, 0);
    featureOutline = sf::VertexArray(sf::Lines, 0);

    featureBoxColor = sf::Color(41, 79, 109, 255);
    featureOutlineColor = sf::Color(63, 113, 152, 255);

    // Block color intensity
    colorIntensity = blockColor;

    // Create the seven shapes
    shapes.clear();
    // I
    shapes.push_back(Shape(sf::Color(colorIntensity, 0, 0), -2, 0, -1, 0, 0, 0, 1, 0, true, true));
    // L
    shapes.push_back(Shape(sf::Color(0, 0, colorIntensity), -1, 0, -1, -1, 0, 0, 1, 0));
    // Backwards L
    shapes.push_back(Shape(sf::Color(colorIntensity, colorIntensity / 2, 0), -1, 0, 1, -1, 0, 0, 1, 0));
    // T
    shapes.push_back(Shape(sf::Color(colorIntensity, 0, colorIntensity), 0, 0, 0, -1, -1, 0, 1, 0));
    // N
    shapes.push_back(Shape(sf::Color(0, colorIntensity, colorIntensity), 0, 0, -1, 0, 0, -1, 1, -1));
    // Backwards N
    shapes.push_back(Shape(sf::Color(0, colorIntensity, 0), 0, 0, 1, 0, 0, -1, -1, -1));
    // Block
    shapes.push_back(Shape(sf::Color(colorIntensity, colorIntensity, 0), 0, 0, 0, -1, -1, 0, -1, -1, true, false, -1, 0));

    // Creat the moves
    left = Move(-1, 0, 0, false);
    right = Move(1, 0, 0, false);
    down = Move(0, -1, 0, false);
    jump = Move(0, 0, 0, true);
    rotateLeft = Move(0, 0, 3, false);
    rotateRight = Move(0, 0, 1, false);

    minMoveInterval = sminMoveInterval;

    featureWidth = sfeaturewidth;
    featurePadding = sfeaturepadding;
    features = Vector(max.x + 20, min.y - 1);
    featuresYourScoreDimensions = Vector(featureWidth, 100);
    featuresNextBlockDimensions = Vector(featureWidth, 5 * blockSide);
    featuresTimerDimensions = Vector(featureWidth, 100);
    featuresYourScore = Vector(features.x, features.y);
    featuresNextBlock = Vector(features.x, features.y
              + featuresYourScoreDimensions.y + featurePadding);
    featuresTimer = Vector(features.x, features.y
              + featuresYourScoreDimensions.y + featurePadding
              + featuresNextBlockDimensions.y + featurePadding);
    nextBlockHeight = 50;
    undoHeight = 12;
    gameTimeMins = 5;
    secsInMin = 60;
    gameTime = secsInMin * gameTimeMins; // 4 minutes

    // Fonts/text
    scoreText.setFont(dFont);
    scoreTextNumbers.setFont(sFont);
    nextShapeText.setFont(dFont);
    timerText.setFont(dFont);
    timerTextNumbers.setFont(sFont);

    scoreText.setCharacterSize(30);
    scoreTextNumbers.setCharacterSize(40);
    nextShapeText.setCharacterSize(30);
    timerText.setCharacterSize(30);
    timerTextNumbers.setCharacterSize(40);

    textColor = sf::Color(255, 255, 255, 255);

    scoreText.setColor(textColor);
    scoreTextNumbers.setColor(textColor);
    nextShapeText.setColor(textColor);
    timerText.setColor(textColor);
    timerTextNumbers.setColor(textColor);

    scoreText.setString("Score");
    nextShapeText.setString("Next Block");
    timerText.setString("Time");

    textPaddingx = 10;
    textPaddingy = 5;

    scoreText.setPosition(featuresYourScore.x + textPaddingx, featuresYourScore.y + textPaddingy);
    nextShapeText.setPosition(featuresNextBlock.x + textPaddingx, featuresNextBlock.y + textPaddingy);
    timerText.setPosition(featuresTimer.x + textPaddingx, featuresTimer.y + textPaddingy);

    // Game
    drawFeatureBox(min.x - 1, min.y - 1, max.x - min.x + 2, max.y - min.y + 2);

    // Your score
    drawFeatureBox(featuresYourScore.x, featuresYourScore.y, featuresYourScoreDimensions.x, featuresYourScoreDimensions.y);

    // Next shape
    drawFeatureBox(featuresNextBlock.x, featuresNextBlock.y, featuresNextBlockDimensions.x, featuresNextBlockDimensions.y);

    // Timer
    drawFeatureBox(featuresTimer.x, featuresTimer.y, featuresTimerDimensions.x, featuresTimerDimensions.y);

    newGame (normal);

    drawPoints();
    drawTimer();
  }
  void Game::newGame (gameType newGameType) {
    curGameType = newGameType;

    gameState = inGame;

    left.restart();
    right.restart();
    down.restart();
    jump.restart();
    rotateLeft.restart();
    rotateRight.restart();

    // Set points:
    totalPoints = 0;
    points.clear();
    weights.clear();
    points.resize(5, 0);
    weights.push_back(5);
    weights.push_back(50);
    weights.push_back(100);
    weights.push_back(200);
    weights.push_back(500);

    blocks.clear();
    // Resize blocks to the right size
    blocks.resize(blockTowerWidth, std::vector<Block>(blockTowerHeight + 1, emptyBlock));

    if (curGameType == undoGame) {
      createUndoMap();
    }

    // Choose a random shape
    nextShape = shapes[RNG.block()];

    // Eye candy
    effectsPoints = 0;
    effectInterval = 0.05;

    moveTimer.restart();
    gameTimer.restart();
    drop.restart();
    effects.restart();

    spawnNewBlock();
  }
  void Game::restart (void) {
    newGame(curGameType);
  }
  void Game::doEffects (void) {
    if (effects.getElapsedTime().asSeconds() > effectInterval) {
      effects.restart();
      if (effectsPoints < totalPoints) {
        effectsPoints += int ((totalPoints - effectsPoints + 1) / 2);
      }
    }
  }
  void Game::centerText (sf::Text& text, int x, int y) {
    text.setPosition(x - text.getLocalBounds().width / 2, y);
  }
  void Game::drawFeatureBox (int x, int y, int width, int height) {
    featureOutline.append(sf::Vertex(sf::Vector2f(x, y), featureOutlineColor));
    featureOutline.append(sf::Vertex(sf::Vector2f(x + width, y), featureOutlineColor));
    featureOutline.append(sf::Vertex(sf::Vector2f(x + width, y + height), featureOutlineColor));
    featureOutline.append(sf::Vertex(sf::Vector2f(x, y + height), featureOutlineColor));
    featureOutline.append(sf::Vertex(sf::Vector2f(x, y + height), featureOutlineColor));
    featureOutline.append(sf::Vertex(sf::Vector2f(x, y), featureOutlineColor));
    featureOutline.append(sf::Vertex(sf::Vector2f(x + width, y), featureOutlineColor));
    featureOutline.append(sf::Vertex(sf::Vector2f(x + width, y + height), featureOutlineColor));

    featureBoxes.append(sf::Vertex(sf::Vector2f(x, y), featureBoxColor));
    featureBoxes.append(sf::Vertex(sf::Vector2f(x + width, y), featureBoxColor));
    featureBoxes.append(sf::Vertex(sf::Vector2f(x + width, y + height), featureBoxColor));
    featureBoxes.append(sf::Vertex(sf::Vector2f(x, y + height), featureBoxColor));
  }
  void Game::calculatePoints (void) {
    totalPoints = points[0] * weights[0] +
                  points[1] * weights[1] +
                  points[2] * weights[2] +
                  points[3] * weights[3] +
                  points[4] * weights[4];
  }
  void Game::drawPoints (void) {
    calculatePoints();
    std::stringstream pointText;
    pointText << std::setw(7) << std::setfill('0') << effectsPoints;
    scoreTextNumbers.setString(pointText.str());
    centerText (scoreTextNumbers, featuresYourScore.x + featureWidth / 2, featuresYourScore.y + 40);
  }
  void Game::drawTimer (void) {
    if (gameState == inGame) {
      int secondsPlayed = std::floor(gameTimer.getElapsedTime().asSeconds());
      int minutesPlayed = std::floor(secondsPlayed / secsInMin);
      int seconds = std::floor(secondsPlayed % secsInMin);
      int displayMins = minutesPlayed;
      int displaySecs = seconds;
      if (curGameType != normal) {
        displayMins = gameTimeMins - minutesPlayed - 1;
        displaySecs = secsInMin - seconds - 1;
      }
      std::stringstream timerTextMins;
      timerTextMins << std::setw(2) << std::setfill('0') << displayMins;
      std::stringstream timerTextSecs;
      timerTextSecs << std::setw(2) << std::setfill('0') << displaySecs;

      gameTimePretty = timerTextMins.str().append(":").append(timerTextSecs.str());

      timerTextNumbers.setString(gameTimePretty);
      centerText (timerTextNumbers, featuresTimer.x + featureWidth / 2, featuresTimer.y + 40);

      if (curGameType != normal && gameTimer.getElapsedTime().asSeconds() > gameTime - 0.1) {
        if (curGameType == undoGame) {
          endGameLogic(undoTimeOver);
        } else if (curGameType == timeTrial) {
          endGameLogic(timeTrialOver);
        }
      }
    }
  }
  // Checks whether the block can go to a certain position
  // a.k.a. whether there are any blocks at those places
  bool Game::canBlockGo (Shape shape, Vector location) {
    bool cannotGo = false;
    for (int i = 0; i < 4; i++) {
      if (location.x + shape.blocks[i].x < 0 ||
          location.y + shape.blocks[i].y < 0 ||
          location.x + shape.blocks[i].x > blockTowerWidth - 1 ||
          blocks[location.x + shape.blocks[i].x][location.y + shape.blocks[i].y].isBlock) {
        cannotGo = true;
      }
    }
    return !cannotGo;
  }
  // Helper function that does the same to a rotated block
  bool Game::canBlockRotate (Shape shape, Vector location, int rotation) {
    shape.rotate(rotation);
    return canBlockGo (shape, location);
  }
  void Game::move (Move& doMove) {
    if (moveTimer.getElapsedTime().asSeconds() - doMove.lastMove > minMoveInterval) {
      doMove.lastMove = moveTimer.getElapsedTime().asSeconds();
      if (doMove.x != 0 || doMove.y != 0) {
        if (canBlockGo(currentShape, Vector(currentLocation.x + doMove.x, currentLocation.y + doMove.y))) {
          currentLocation.x += doMove.x;
          currentLocation.y += doMove.y;
        }
      }
      if (doMove.rotation != 0) {
        if (canBlockRotate(currentShape, Vector(currentLocation.x, currentLocation.y), doMove.rotation)) {
          currentShape.rotate(doMove.rotation); currentShapeGhost.rotate(doMove.rotation);
        }
      }
      if (doMove.jump) {
        placeGhost();
        currentLocation = currentLocationGhost;
        placeBlock();
      }
    }
  }
  void Game::inputLeft (void) { move(left); }
  void Game::inputRight (void) { move(right); }
  void Game::inputDown (void) { move(down); }
  void Game::inputRotateLeft (void) { move(rotateLeft); }
  void Game::inputRotateRight (void) { move(rotateRight); }
  void Game::inputJump (void) { move(jump); }
  void Game::createUndoMap (void) {
    // Create some random blocks
    for (int level = 0; level < undoHeight; level++) {
      int blocksInRow = 0;
      for (int x = 0; x < blockTowerWidth; x++) {
        if (RNG.createBlock()) {
          blocks[x][level] = undoBlock;
          blocksInRow++;
        }
      }
      if (blocksInRow == blockTowerWidth) {
        blocks[RNG.blockDestroy()][level].isBlock = false;
      }
    }
  }
  void Game::placeGhost (void) {
    int y = 0;
    bool done = false;
    // Check how far the block can move and move ghost there
    while (!done && y < currentLocation.y + 1) {
      currentLocationGhost = Vector(currentLocation.x, y);
      if (canBlockGo(currentShapeGhost, currentLocationGhost)) {
        done = true;
        for (int cy = y; cy < currentLocation.y + 1; cy++) {
          for (int i = 0; i < 4; i++) {
            if (blocks[currentLocation.x + currentShape.blocks[i].x][cy + currentShape.blocks[i].y].isBlock) {
              done = false;
            }
          }
        }
      }
      y++;
    }
  }
  void Game::spawnNewBlock (void) {
    if (!canBlockGo(nextShape, Vector(blockTowerWidth / 2, blockTowerVisibleHeight - 1))) {
      endGameLogic(tooManyBlocks);
    } else {
      currentShape = nextShape;
      nextShape = shapes[RNG.block()];

      // The ghost is the same, but with reduced opacity
      currentShapeGhost = currentShape;
      currentShapeGhost.color.a = 80;

      // Place it at the top
      currentLocation = Vector(blockTowerWidth / 2, blockTowerVisibleHeight - 1);

      placeGhost();
    }
  }
  void Game::drawBlock (int x, int y, sf::Color& color) {
    if (y < blockTowerVisibleHeight) {
      blockVertexArray.append(sf::Vertex(sf::Vector2f(min.x + x * blockSide, max.y - y * blockSide), color));
      blockVertexArray.append(sf::Vertex(sf::Vector2f(min.x + (x + 1) * blockSide, max.y - y * blockSide), color));
      blockVertexArray.append(sf::Vertex(sf::Vector2f(min.x + (x + 1) * blockSide, max.y - (y + 1) * blockSide), color));
      blockVertexArray.append(sf::Vertex(sf::Vector2f(min.x + x * blockSide, max.y - (y + 1) * blockSide), color));
    }
  }
  void Game::drawShape (Shape& sh, Vector& loc) {
    for (int i = 0; i < 4; i++) {
      drawBlock (sh.blocks[i].x + loc.x, sh.blocks[i].y + loc.y, sh.color);
    }
  }
  void Game::drawNext (void) {
    nextVertexArray.clear();
    nextGrid.clear();

    // Calculate some coordinates
    Vector center(featuresNextBlock.x + featuresNextBlockDimensions.x / 2, featuresNextBlock.y + featuresNextBlockDimensions.y / 2 + nextBlockHeight / 2);

    // Ugly HAXX
    double addx = 0;
    double addy = 0;

    if (nextShape.evenWide) {
      addx = 0.5;
    }
    if (nextShape.oddThick) {
      addy = -0.5;
    }

    for (int i = 0; i < 4; i++) {
      nextVertexArray.append(sf::Vertex(sf::Vector2f(center.x + (nextShape.blocks[i].x - 0.5 + addx) * blockSide, center.y - (nextShape.blocks[i].y + addy) * blockSide), nextShape.color));
      nextVertexArray.append(sf::Vertex(sf::Vector2f(center.x + (nextShape.blocks[i].x + 0.5 + addx) * blockSide, center.y - (nextShape.blocks[i].y + addy) * blockSide), nextShape.color));
      nextVertexArray.append(sf::Vertex(sf::Vector2f(center.x + (nextShape.blocks[i].x + 0.5 + addx) * blockSide, center.y - (nextShape.blocks[i].y + 1 + addy) * blockSide), nextShape.color));
      nextVertexArray.append(sf::Vertex(sf::Vector2f(center.x + (nextShape.blocks[i].x - 0.5 + addx) * blockSide, center.y - (nextShape.blocks[i].y + 1 + addy) * blockSide), nextShape.color));

      nextGrid.append(sf::Vertex(sf::Vector2f(center.x + (nextShape.blocks[i].x - 0.5 + addx) * blockSide, center.y - (nextShape.blocks[i].y + addy) * blockSide), gridLineColor));
      nextGrid.append(sf::Vertex(sf::Vector2f(center.x + (nextShape.blocks[i].x + 0.5 + addx) * blockSide, center.y - (nextShape.blocks[i].y + addy) * blockSide), gridLineColor));
      nextGrid.append(sf::Vertex(sf::Vector2f(center.x + (nextShape.blocks[i].x + 0.5 + addx) * blockSide, center.y - (nextShape.blocks[i].y + 1 + addy) * blockSide), gridLineColor));
      nextGrid.append(sf::Vertex(sf::Vector2f(center.x + (nextShape.blocks[i].x - 0.5 + addx) * blockSide, center.y - (nextShape.blocks[i].y + 1 + addy) * blockSide), gridLineColor));

      nextGrid.append(sf::Vertex(sf::Vector2f(center.x + (nextShape.blocks[i].x + 0.5 + addx) * blockSide, center.y - (nextShape.blocks[i].y + addy) * blockSide), gridLineColor));
      nextGrid.append(sf::Vertex(sf::Vector2f(center.x + (nextShape.blocks[i].x + 0.5 + addx) * blockSide, center.y - (nextShape.blocks[i].y + 1 + addy) * blockSide), gridLineColor));
      nextGrid.append(sf::Vertex(sf::Vector2f(center.x + (nextShape.blocks[i].x - 0.5 + addx) * blockSide, center.y - (nextShape.blocks[i].y + 1 + addy) * blockSide), gridLineColor));
      nextGrid.append(sf::Vertex(sf::Vector2f(center.x + (nextShape.blocks[i].x - 0.5 + addx) * blockSide, center.y - (nextShape.blocks[i].y + addy) * blockSide), gridLineColor));
    }
  }
  void Game::checkRows (void) {
    int rows = 0;
    // Recursively loop until got rid of all rows
    int y = 0;
    while (y < blockTowerHeight) {
      int blocksInRow = 0;
      for (int x = 0; x < blockTowerWidth; x++) {
        if (blocks[x][y].isBlock) {
          blocksInRow++;
        }
      }
      // If the row is full
      if (blocksInRow == blockTowerWidth) {
        rows++;

        // Move every block above down
        for (int cy = y; cy < blockTowerHeight; cy++) {
          for (int x = 0; x < blockTowerWidth; x++) {
            blocks[x][cy] = blocks[x][cy + 1];
          }
        }
        // Make the last row empty blocks
        for (int x = 0; x < blockTowerWidth; x++) {
          blocks[x][blockTowerHeight] = emptyBlock;
        }
        // Restart counter
        y = 0;
      } else {
        y++;
      }
    }
    points[rows]++;
  }
  void Game::dissolveCurrentBlock (void) {
    // Dissolve the block into the field
    for (int i = 0; i < 4; i++) {
      blocks[currentLocation.x + currentShape.blocks[i].x][currentLocation.y + currentShape.blocks[i].y] = Block(currentShape.color, true);
    }
  }
  void Game::placeBlock (void) {
    dissolveCurrentBlock();
    checkRows();
    spawnNewBlock();
    if (curGameType == undoGame) {
      int undoBlocks = 0;
      for (int level = 0; level < undoHeight; level++) {
        for (int x = 0; x < blockTowerWidth; x++) {
          if (blocks[x][level].isUndoBlock) {
            undoBlocks++;
          }
        }
      }
      if (undoBlocks == 0) {
        endGameLogic(undoNoBlocks);
      }
    }
  }
  void Game::endGameLogic (endType type) {
    calculatePoints();
    switch (curGameType) {
      case normal:
        timeEntry.setText(std::string("Time: ").append(gameTimePretty));
        break;
      case undoGame:
      case timeTrial:
        timeEntry.setText(std::string(""));
        break;
    }
    switch (type) {
      case undoNoBlocks:
        totalPoints += (gameTime - gameTimer.restart().asSeconds()) * 25;
        endEntry.setText(std::string("You won!"));
        break;
      case undoTimeOver:
        endEntry.setText(std::string("You lost!"));
        break;
      case timeTrialOver:
        endEntry.setText(std::string("Good job!"));
        break;
      case tooManyBlocks:
        endEntry.setText(std::string("You lost!"));
        break;
    }
    scoreEntry.setText(std::string("Score: ").append(std::to_string(totalPoints)));
    gameState = inEndBox;
  }
  void Game::gameLogic (void) {
    if (gameState == inGame) {
      if (drop.getElapsedTime().asSeconds() > dropInterval) {
        drop.restart();
        // Blocks drop
        if (!canBlockGo(currentShape, Vector(currentLocation.x, currentLocation.y - 1))) {
          placeBlock();
        } else {
          currentLocation.y--;
        }
      }
      placeGhost();
    }

    doEffects();

    // Create a single drawable object from the block data
    blockVertexArray.clear();

    // Iterate through each block and draw a block where it's supposed to go
    for (int x = 0; x < blockTowerWidth; x++) {
      for (int y = 0; y < blockTowerHeight; y++) {
        if (blocks[x][y].isBlock) {
          drawBlock (x, y, blocks[x][y].color);
        }
      }
    }

    // Draw the current block
    drawShape (currentShape, currentLocation);

    // Draw ghost block
    if (drawGhost) {
      drawShape (currentShapeGhost, currentLocationGhost);
    }

    drawNext();

    drawPoints();

    drawTimer();
  }
  void Game::draw (sf::RenderWindow& window) {
    window.draw(featureBoxes);
    window.draw(featureOutline);

    // Draw the blocks
    window.draw(blockVertexArray);

    // Overlay the grid
    window.draw(grid);

    window.draw(nextVertexArray);
    window.draw(nextGrid);

    window.draw(scoreText);
    window.draw(scoreTextNumbers);
    window.draw(nextShapeText);
    window.draw(timerText);
    window.draw(timerTextNumbers);
  }
  int Game::getPoints (void) {
    return totalPoints;
  }
}
