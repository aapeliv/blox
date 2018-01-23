#ifndef CLASSES_HPP
#define CLASSES_HPP

#include <SFML/Graphics.hpp>

namespace AapeliBlox {
  // Small helper class for moves
  struct Move {
    int x, y, rotation;
    bool jump;
    double lastMove;
    Move(int sx = 0, int sy = 0, int srotation = 0, bool sjump = false);
    void restart(void);
  };
  const int blockSide = 30;
  struct Vector {
    int x, y;
    Vector(int sx = 0, int sy = 0);
  };
  struct Block {
    sf::Color color;
    bool isBlock;
    bool isUndoBlock;
    Block(sf::Color scolor = sf::Color::White, bool visible = false, bool sisUndoBlock = false);
  };
  struct ShapeBlock {
    sf::Color color;
    int x, y;
    ShapeBlock(sf::Color scolor = sf::Color::White, int sx = 0, int sy = 0);
  };
  struct Shape {
    bool evenWide;
    bool oddThick;
    int minx, maxx;
    int originx, originy;
    sf::Color color;
    Vector blocks[4];
    Shape(sf::Color scolor = sf::Color(255,255,255,0),
           int x1 = 0, int y1 = 0,
           int x2 = 0, int y2 = 1,
           int x3 = 0, int y3 = 2,
           int x4 = 0, int y4 = 3,
           bool sevenWide = false,
           bool soddThick = false,
           int soriginx = 0,
           int soriginy = 0);
    void rotate(int rotation);
  private:
    void updateExtrema(void);
  };
}

#endif //CLASSES_HPP
