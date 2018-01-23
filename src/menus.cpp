#include <vector>

#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "menu.hpp"
#include "menus.hpp"

#include "resources/bloxLogo.hpp"

namespace AapeliBlox {
  MenuHandler::MenuHandler(states& sgameState, sf::Font& menuFont, int width, int height) : gameState(sgameState) {
    selected = 0;

    // Let's make some menus!
    Continue = MenuEntry("Continue", true);
    Restart = MenuEntry("Restart", true);
    MainMenu = MenuEntry("Main Menu", true);
    QuickGame = MenuEntry("Quick Game", true);
    Singleplayer = MenuEntry("New Game", true);
    Multiplayer = MenuEntry("Multiplayer", true);
    Highscores = MenuEntry("Highscores", true);
    About = MenuEntry("About", true);
    Quit = MenuEntry("Quit", true);

    // New game entries
    ChooseGame = MenuEntry("Choose Game Type:", false);
    TimeTrial = MenuEntry("Time Trial", true);
    Sandbox = MenuEntry("Sandbox", true);
    Undo = MenuEntry("Undo", true);
    Back = MenuEntry("Back", true);

    EmptyLine = MenuEntry("", false);

    About0 = MenuEntry("Copyright (c) 2014-2016 Aapeli Vuorinen", false);
    About1 = MenuEntry("Free download, including source code at www.aapelivuorinen.com", false);
    About2 = MenuEntry("This game uses the following great libraries and fonts:", false);
    About3 = MenuEntry("SFML, Copyright (c) Laurent Gomila; Inconsolata, Copyright (c) 2011 Raph Levien (OFL 1.1);", false);
    About4 = MenuEntry("Bangers, Copyright (c) 2010 Vernon Adams (OFL 1.1); and Chewy (Apache License 2.0)", false);

    EndGameText = MenuEntry("", false);
    EndGameTime = MenuEntry("", false);
    EndGameScore = MenuEntry("", false);

    newGameMenuEntries.clear();
    //newGameMenuEntries.push_back(&ChooseGame);
    newGameMenuEntries.push_back(&TimeTrial);
    newGameMenuEntries.push_back(&Sandbox);
    newGameMenuEntries.push_back(&Undo);
    newGameMenuEntries.push_back(&EmptyLine);
    newGameMenuEntries.push_back(&Back);

    continueMenuEntries.clear();
    continueMenuEntries.push_back(&Continue);
    continueMenuEntries.push_back(&Restart);
    continueMenuEntries.push_back(&MainMenu);
    continueMenuEntries.push_back(&Quit);

    mainMenuEntries.clear();
    //mainMenuEntries.push_back(&QuickGame);
    mainMenuEntries.push_back(&Singleplayer);
    //mainMenuEntries.push_back(&Multiplayer);
    //mainMenuEntries.push_back(&Highscores);
    mainMenuEntries.push_back(&About);
    mainMenuEntries.push_back(&Quit);

    aboutBoxEntries.clear();
    aboutBoxEntries.push_back(&About0);
    aboutBoxEntries.push_back(&About1);
    aboutBoxEntries.push_back(&EmptyLine);
    aboutBoxEntries.push_back(&About2);
    aboutBoxEntries.push_back(&About3);
    aboutBoxEntries.push_back(&About4);
    aboutBoxEntries.push_back(&EmptyLine);
    aboutBoxEntries.push_back(&Back);

    endBoxEntries.clear();
    endBoxEntries.push_back(&EndGameText);
    endBoxEntries.push_back(&EndGameScore);
    endBoxEntries.push_back(&EndGameTime);
    endBoxEntries.push_back(&Restart);
    endBoxEntries.push_back(&MainMenu);


    newGameMenu = MenuScreen(menuFont, newGameMenuEntries, 45, width, height);
    continueMenu = MenuScreen(menuFont, continueMenuEntries, 45, width, height);
    mainMenu = MenuScreen(menuFont, mainMenuEntries, 45, width, height);
    endBox = MenuScreen(menuFont, endBoxEntries, 45, width, height);
    aboutBox = MenuScreen(menuFont, aboutBoxEntries, 16, width, height);

    // Load the logo:
    logoTexture.create(bloxLogo.width, bloxLogo.height);
    logoTexture.update(bloxLogo.pixel_data);

    logoSprite.setTexture(logoTexture);
    logoSprite.setPosition(width / 2 - bloxLogo.width / 2, width / 2 - 100);
  }
  MenuScreen* MenuHandler::currentMenu(void) {
    switch (gameState) {
      case inMainMenu:
        return &mainMenu;
      case inNewGameMenu:
        return &newGameMenu;
      case inContinueMenu:
        return &continueMenu;
      case inAboutBox:
        return &aboutBox;
      case inEndBox:
        return &endBox;
      default:
        return 0;
    }
  }
  void MenuHandler::mouseMoved(int x, int y) {
    currentMenu()->checkHover(x, y);
  }
  void MenuHandler::mouseClicked(int x, int y) {
    currentMenu()->checkHover(x, y);
    selected = currentMenu()->getSelected();
  }
  void MenuHandler::inputUp(void) {
    currentMenu()->changeSelection(-1);
  }
  void MenuHandler::inputDown(void) {
    currentMenu()->changeSelection(1);
  }
  void MenuHandler::inputEnter(void) {
    selected = currentMenu()->getSelected();
  }
  void MenuHandler::inputEsc(void) {
    switch (gameState) {
      case inContinueMenu:
        selected = &Continue; break;
      case inMainMenu:
        selected = &Quit; break;
      default:
        selected = &Back; break;
    }
  }
  selectionOutcome MenuHandler::processSelection(void) {
    MenuEntry* temp = selected;
    selected = 0;
    if (temp == &Singleplayer) { return goToNewGameMenu; }
    else if (temp == &Quit) { return doQuit; }
    else if (temp == &Continue) { return continueGame; }
    else if (temp == &About) { return goToAboutBox; }
    else if (temp == &TimeTrial) { return newGameTimeTrial; }
    else if (temp == &Sandbox) { return newGameNormal; }
    else if (temp == &Undo) { return newGameUndo; }
    else if (temp == &MainMenu) { return goToMainMenu; }
    else if (temp == &Restart) { return newGameRestart; }
    else if (temp == &QuickGame) { return newGameTimeTrial; }
    else if (temp == &Back) { return goToMainMenu; }
    else { return doNothing; }
  }
  void MenuHandler::draw(sf::RenderWindow& window) {
    if (gameState != inGame) {
      currentMenu()->draw(window);
      window.draw(logoSprite);
    }
  }
  MenuEntry& MenuHandler::getEndTextEntry(void) {
    return EndGameText;
  }
  MenuEntry& MenuHandler::getEndScoreEntry(void) {
    return EndGameScore;
  }
  MenuEntry& MenuHandler::getEndTimeEntry(void) {
    return EndGameTime;
  }
}

#include "menus.hpp"
