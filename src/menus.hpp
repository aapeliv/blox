#ifndef MENUS_HPP
#define MENUS_HPP

#include <vector>

#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "menu.hpp"

namespace AapeliBlox {
  enum states {
    inMainMenu,
    inNewGameMenu,
    inContinueMenu,
    inAboutBox,
    inEndBox,
    inGame
  };
  enum selectionOutcome {
    doQuit,
    continueGame,
    goToNewGameMenu,
    goToAboutBox,
    goToMainMenu,
    newGameUndo,
    newGameNormal,
    newGameTimeTrial,
    newGameRestart,
    doNothing
  };
  class MenuHandler {
    sf::Sprite logoSprite;
    sf::Texture logoTexture;

    MenuEntry* selected;
    states& gameState;
    std::vector<MenuEntry*> newGameMenuEntries, continueMenuEntries;
    std::vector<MenuEntry*> mainMenuEntries, aboutBoxEntries, endBoxEntries;
    MenuScreen newGameMenu, continueMenu, mainMenu, aboutBox, endBox;
    MenuEntry Continue, Restart, MainMenu, QuickGame, Singleplayer, Multiplayer,
      Highscores, About, Quit, ChooseGame, TimeTrial, Sandbox, Undo, EndGameText,
      EndGameTime, EndGameScore;
    MenuEntry About0, About1, About2, About3, About4, EmptyLine, Back;
  public:
    MenuHandler(states& sgameState, sf::Font& menuFont, int width, int height);
    MenuScreen* currentMenu(void);
    void mouseMoved(int x, int y);
    void mouseClicked(int x, int y);
    void inputUp(void);
    void inputDown(void);
    void inputEnter(void);
    void inputEsc(void);
    selectionOutcome processSelection(void);
    void draw(sf::RenderWindow& window);
    MenuEntry& getEndTextEntry(void);
    MenuEntry& getEndScoreEntry(void);
    MenuEntry& getEndTimeEntry(void);
  };
}

#endif //MENU_HPP
