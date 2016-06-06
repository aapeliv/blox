#include <SFML/Graphics.hpp>

#include "classes.hpp"

namespace AapeliBlox {
  Move::Move (int sx, int sy, int srotation, bool sjump) {
    x = sx; y = sy; rotation = srotation; jump = sjump; lastMove = 0;
  }
  void Move::restart (void) {
    lastMove = 0;
  }
  Vector::Vector (int sx, int sy) {
    x = sx; y = sy;
  }
  Block::Block (sf::Color scolor, bool visible, bool sisUndoBlock) {
    color = scolor; isBlock = visible; isUndoBlock = sisUndoBlock;
  }
  ShapeBlock::ShapeBlock (sf::Color scolor, int sx, int sy) {
    x = sx; y = sy; color = scolor;
  }
  void Shape::updateExtrema (void) {
    minx = 0; maxx = 0;
    for (int i = 0; i < 4; i++) {
      minx = std::min(minx, blocks[i].x);
      maxx = std::max(maxx, blocks[i].x);
    }
  }
  Shape::Shape (sf::Color scolor,
                int x1, int y1,
                int x2, int y2,
                int x3, int y3,
                int x4, int y4,
                bool sevenWide,
                bool soddThick,
                int soriginx,
                int soriginy) {
    color = scolor;
    blocks[0] = Vector(x1, y1);
    blocks[1] = Vector(x2, y2);
    blocks[2] = Vector(x3, y3);
    blocks[3] = Vector(x4, y4);
    evenWide = sevenWide;
    oddThick = soddThick;
    originx = soriginx;
    originy = soriginy;
    updateExtrema();
  }
  /** Rotate either 0, 90, 180, 270, etc degrees */
  void Shape::rotate (int rotation) {
    while (rotation < 0) {
      rotation += 4;
    }
    int c = 0; int s = 0;
    switch (rotation % 4) {
      case 0:
        c = 1; break;
      case 1:
        s = -1; break;
      case 2:
        c = -1; break;
      case 3:
        s = 1; break;
    }
    for (int i = 0; i < 4; i++) {
      int x = blocks[i].x - originx;
      int y = blocks[i].y - originy;
      blocks[i].x = x * c - y * s;
      blocks[i].y = x * s + y * c;
    }
    updateExtrema();
  }
}
