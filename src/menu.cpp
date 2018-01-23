#include <vector>
#include <string>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "menu.hpp"

namespace AapeliBlox {
  MenuEntry::MenuEntry(std::string stext, bool sclickable) {
    text = stext; clickable = sclickable;
  }
  void MenuEntry::setText(std::string stext) {
    text = stext;
  }
  MenuScreen::MenuText::MenuText(sf::Text ssfText, MenuEntry* sentry) {
    sfText = ssfText; entry = sentry;
  }
  MenuScreen::MenuScreen(void) {
    // Do nothing
  }
  MenuScreen::MenuScreen(sf::Font& sfont,
                          std::vector<MenuEntry*> menuEntries,
                          int scharSize,
                          int smaxx,
                          int smaxy) {
    maxx = smaxx;
    maxy = smaxy;
    bgHeight = 350;
    textColor = sf::Color::White;
    textColorHover = sf::Color(200, 200, 255 ,255);
    bgColor = sf::Color(97, 164, 216, 200);
    padding = 10;
    centerx = maxx / 2;
    centery = maxy / 2;

    // Background strip
    bgStrip = sf::VertexArray(sf::Quads, 0);
    bgStrip.append(sf::Vertex(sf::Vector2f(0, centery - bgHeight / 2), bgColor));
    bgStrip.append(sf::Vertex(sf::Vector2f(maxx, centery - bgHeight / 2), bgColor));
    bgStrip.append(sf::Vertex(sf::Vector2f(maxx, centery + bgHeight / 2), bgColor));
    bgStrip.append(sf::Vertex(sf::Vector2f(0, centery + bgHeight / 2), bgColor));

    lineHeight = 0;

    for (int i = 0; i < menuEntries.size(); i++) {
      sf::Text sfText;
      sfText.setFont(sfont);
      sfText.setCharacterSize(scharSize);
      sfText.setFillColor(textColor);
      sfText.setString(menuEntries[i]->text);
      lineHeight = std::max(int(lineHeight), int(sfText.getLocalBounds().height));
      entries.push_back(MenuText(sfText, menuEntries[i]));
    }

    for (int i = menuEntries.size() - 1; i >= 0; i--) {
      if (entries[i].entry->clickable) {
        selected = i;
      }
    }
  }
  bool MenuScreen::checkHover(int x, int y) {
    for (int i = 0; i < entries.size(); i++) {
      if (entries[i].sfText.getGlobalBounds().contains(x, y) && entries[i].entry->clickable) {
        selected = i;
        return true;
      }
    }
    return false;
  }
  void MenuScreen::changeSelection(int dir) {
    if (selected + dir >= 0 && selected + dir < entries.size() && entries[selected + dir].entry->clickable) {
      selected += dir;
    }
  }
  void MenuScreen::draw(sf::RenderWindow& window) {
    // Draw background
    window.draw(bgStrip);

    int drawAt = centery - entries.size() * (lineHeight + padding) / 2 + 4 * padding;
    for (int i = 0; i < entries.size(); i++) {
      entries[i].sfText.setString(entries[i].entry->text);
      entries[i].sfText.setFillColor(textColor);
      if (selected == i) {
        entries[i].sfText.setFillColor(textColorHover);
      }
      entries[i].sfText.setPosition(centerx - entries[i].sfText.getLocalBounds().width / 2, drawAt);
      window.draw(entries[i].sfText);
      drawAt += (lineHeight + padding);
    }
  }
  MenuEntry* MenuScreen::getSelected(void) {
    return entries[selected].entry;
  }
}
