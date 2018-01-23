#ifndef MENU_HPP
#define MENU_HPP

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

namespace AapeliBlox {
  struct MenuEntry {
    std::string text;
    bool clickable;
    MenuEntry(std::string stext = "", bool sclickable = true);
    void setText(std::string stext);
  };
  class MenuScreen {
    struct MenuText {
      sf::Text sfText;
      MenuEntry* entry;
      MenuText(sf::Text ssfText, MenuEntry* sentry);
    };
    int maxx, maxy, centerx, centery;
    int bgHeight, padding;
    int lineHeight;
    int selected;
    sf::Color textColor, textColorHover, bgColor;
    std::vector<MenuText> entries;
    sf::VertexArray bgStrip;
  public:
    MenuScreen(void);
    MenuScreen(sf::Font& sfont,
                std::vector<MenuEntry*> menuEntries,
                int scharSize,
                int smaxx,
                int smaxy);
    bool checkHover(int x, int y);
    void draw(sf::RenderWindow& window);
    void changeSelection(int dir);
    MenuEntry* getSelected(void);
  };
}

#endif //MENU_HPP
