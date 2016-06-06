#include <vector>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "classes.hpp"
#include "random.hpp"
#include "game.hpp"
#include "menu.hpp"
#include "menus.hpp"

#include "resources/Inconsolata.hpp"
#include "resources/Bangers.hpp"
#include "resources/Chewy.hpp"

#include "resources/bloxIcon.hpp"

using namespace AapeliBlox;

int main(void)
{
	// Current state
	states gameState = inMainMenu;

	int gameWidth = 522;
	int gameHeight = 603;
	int padding = 20;

	int width = gameWidth + 2 * padding;
	int height = gameHeight + 2 * padding;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 2;
	sf::RenderWindow window(sf::VideoMode(width, height), "AapeliBlox", sf::Style::Close, settings);

	window.setIcon(bloxIcon.width,  bloxIcon.height,  bloxIcon.pixel_data);

	std::random_device rRandomDevice;

  RandomNumberGenerator myRNG(rRandomDevice(), 10);

	sf::Font scoreFont;
	scoreFont.loadFromMemory(Inconsolata_ttf, Inconsolata_ttf_len);
	sf::Font menuFont;
	menuFont.loadFromMemory(Bangers_ttf, Bangers_ttf_len);
	sf::Font chewy;
	chewy.loadFromMemory(Chewy_ttf, Chewy_ttf_len);

	MenuHandler menus(gameState, menuFont, width, height);

  Game myGame(gameState, myRNG, menuFont, scoreFont, menus.getEndScoreEntry(), menus.getEndTextEntry(), menus.getEndTimeEntry(), 10, 22, 30, 20, 20,
							200, 20, true, 0.5, 200, 0.1, sf::Color(255, 0, 255, 150),
							sf::Color(23, 47, 66, 255));

	gameState = inMainMenu;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type) {
				case sf::Event::Closed:
					window.close(); break;
				case sf::Event::LostFocus:
					if (gameState == inGame) {
						gameState = inContinueMenu;
					}
				case sf::Event::MouseMoved:
					if (gameState != inGame) {
						menus.mouseMoved(event.mouseMove.x, event.mouseMove.y);
					}
					break;
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left && gameState != inGame) {
						menus.mouseClicked(event.mouseButton.x, event.mouseButton.y);
					}
					break;
				case sf::Event::KeyPressed:
					if (gameState != inGame) {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { menus.inputUp(); }
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { menus.inputDown(); }
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) { menus.inputEnter(); }
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { menus.inputEsc(); }
					} else {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {	myGame.inputLeft(); }
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { myGame.inputRight(); }
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { myGame.inputDown(); }
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { myGame.inputRotateRight(); }
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) { myGame.inputRotateRight(); }
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) { myGame.inputRotateLeft(); }
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { myGame.inputJump(); }
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) { gameState = inContinueMenu; }
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { gameState = inContinueMenu; }
					}
				default: break;
			}
		}

		if (gameState != inGame) {
			switch (menus.processSelection()) {
				case goToNewGameMenu:
					gameState = inNewGameMenu; break;
				case goToAboutBox:
					gameState = inAboutBox; break;
				case goToMainMenu:
					gameState = inMainMenu; break;
				case continueGame:
					gameState = inGame; break;
				case doQuit:
					window.close(); break;
				case newGameUndo:
					myRNG.reSeed(rRandomDevice());
					myGame.newGame(undoGame); break;
				case newGameTimeTrial:
					myRNG.reSeed(rRandomDevice());
					myGame.newGame(timeTrial); break;
				case newGameNormal:
					myRNG.reSeed(rRandomDevice());
					myGame.newGame(normal); break;
				case newGameRestart:
					myGame.restart(); break;
				default: break;
			}
		}

		// Clear the buffer
		window.clear(sf::Color(255, 255, 255, 255));

		myGame.gameLogic();

		myGame.draw(window);

		menus.draw(window);

    // Display everything
		window.display();
	}

  // All good. Quit.
	return 0;
}
