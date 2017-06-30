#include "stdafx.h"
#include "Button.h"
#include "Player.h"
#include "StateStack.cpp"
#include <string>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#pragma comment(lib, "Winmm.lib")

std::vector<int> readBMP(char* filename)
{
	FILE* f;
	fopen_s(&f, filename, "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

											   // extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	int size = 3 * width * height;
	int row_padded = (width * 3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded]; // allocate 3 bytes per pixel
	unsigned char tmp;
	//std::vector<int> walkmask(width * height * 144, 0);
	std::vector<int> walkmask(width * height, 0);
	//int currY = (height - 1) * 12;
	int currY = height - 1;
	int xCount = 0;
	for (int i = 0; i < height; ++i)
	{
		fread(data, sizeof(unsigned char), row_padded, f);
		for (int j = 0; j < width * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)
			tmp = data[j];
			data[j] = data[j + 2];
			data[j + 2] = tmp;

			//std::cout << "R: " << (int)data[j] << " G: " << (int)data[j + 1] << " B: " << (int)data[j + 2] << "\n";

			/*if (xCount >= width)
			{
				currY = currY - 12;
				xCount = 0;
			}
			if (data[j] == 0 && data[j + 1] == 0 && data[j + 2] == 0)
			{
				for (int k = currY; k < currY + 12; ++k)
				{
					for (int l = xCount * 12; l < ((xCount * 12) + 12); ++l)
					{
						walkmask[k * width * 12 + l] = 1;
					}
				}
			}*/
			if (xCount >= width)
			{
				currY = currY - 1;
				xCount = 0;
			}
			if (data[j] == 0 && data[j + 1] == 0 && data[j + 2] == 0)
			{
				walkmask[currY * width + xCount] = 1;
			}
			++xCount;
		}
	}
	fclose(f);
	return walkmask;
}

void spawnIn(int x, int y, sf::Sprite& currLevel, sf::Texture& level, std::vector<int>& walkmask, char* path, Player* player)
{
	currLevel.setTexture(level);
	currLevel.move((float)(-(x * 12 - (800 - player->getWidth()/2)) - 12), (float)(-(y * 12 - 375)));
	walkmask = readBMP(path);
	player->setPosition(sf::Vector2f((float)x, (float)y));
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow* window;
	window = new sf::RenderWindow(sf::VideoMode(1600, 900), "Project P", sf::Style::Default, settings);
	window->setFramerateLimit(30);
	sf::Font font;
	font.loadFromFile("img seasons.ttf");

	StateStack gGameMode;
	gGameMode.add("mainmenu", new MainMenuState());
	gGameMode.add("localmap", new MapState());
	gGameMode.add("menu", new MenuState());
	//gGameMode.add("battle", new BattleState());
	std::vector<PlayerEntity*> playerList;
	PlayerEntity* player1 = new PlayerEntity();
	playerList.push_back(player1);
	PlayerEntity* player2 = new PlayerEntity();
	playerList.push_back(player2);
	PlayerEntity* player3 = new PlayerEntity();
	playerList.push_back(player3);
	PlayerEntity* player4 = new PlayerEntity();
	playerList.push_back(player4);
	std::vector<MobEntity*> mobList;
	MobEntity* mob1 = new MobEntity();
	mobList.push_back(mob1);
	MobEntity* mob2 = new MobEntity();
	mobList.push_back(mob2);
	MobEntity* mob3 = new MobEntity();
	mobList.push_back(mob3);
	MobEntity* mob4 = new MobEntity();
	mobList.push_back(mob4);
	MobEntity* mob5 = new MobEntity();
	mobList.push_back(mob5);
	gGameMode.add("battle", new BattleState(playerList, mobList));
	gGameMode.push("mainmenu");
	gGameMode.onEnter();

	//sf::Texture mmbg;
	//mmbg.loadFromFile("ui/mainmenubg.png");
	//sf::Sprite mainMenuBG;
	//mainMenuBG.setTexture(mmbg);
	//sf::RectangleShape pauseScreen(sf::Vector2f(1600, 900));
	//pauseScreen.setFillColor(sf::Color(0, 0, 0, 0));

	//Player* player = new Player(0, 0, 2, 8, "char/standing2.png");

	/*sf::Texture level;
	level.loadFromFile("maps/winterdawn.png");
	sf::Sprite currLevel;
	currLevel.setTexture(level);
	currLevel.setScale(sf::Vector2f(12, 12));
	std::vector<int> walkmask = readBMP("maps/winterdawnwm.bmp");
	int currLevelWidth = 663;*/
	/*for (int i = 0; i < walkmask.size(); ++i)
	{
		if (i % 20 == 0)
		{
			std::cout << "\n";
		}
		std::cout << walkmask[i];
	}*/

	int width = 1600;
	int height = 900;
	int newWidth = 1600;
	int newHeight = 900;
	int prevWidth = 1600;
	int prevHeight = 900;
	double mult = 1;

	// status checks
	int prevMenu = 0;
	int menu = 0;
	int currFrame = 1;
	bool isJump = false;
	int jumpFrame = 0;
	bool isFall = false;
	int fallFrame = 0;

	//std::vector<Button*> buttonList;
	//Button* startButton = new Button(0, 600, 525, 400, 42, "ui/startbutton.png");
	//startButton->setIsClickable(true);
	//buttonList.push_back(startButton);
	//Button* menuButton = new Button(1, 1521, 0, 79, 31, "ui/menubutton.png");
	//menuButton->hide();
	//buttonList.push_back(menuButton);
	//Button* backButton = new Button(2, 1521, 0, 79, 31, "ui/backbutton.png");
	//backButton->hide();
	//buttonList.push_back(backButton);

	//spawnIn(300, 50, currLevel, level, walkmask, "maps/winterdawnwm.bmp", player);
	while (window->isOpen())
	{
		sf::Event event;
		sf::Vector2i localPosition = sf::Mouse::getPosition(*window);
		window->clear(sf::Color::Black);
		menu = gGameMode.update(currFrame, localPosition);
		//std::cout << menu << " " << gGameMode.getSize() << "\n";
		if (prevMenu != menu) {
			gGameMode.onExit();
			switch (menu) {
			case 0: // mainmenu
				if (prevMenu == 2) { // from menu
					while (gGameMode.getSize() > 1) {
						gGameMode.onExit();
						gGameMode.pop();
						gGameMode.onEnter();
					}
				}
				break;

			case 1: // localmap
				if (prevMenu == 0) { // from mainmenu
					gGameMode.push("localmap");
					gGameMode.onEnter();
				}
				else if (prevMenu == 2) { // from menu
					gGameMode.onExit();
					gGameMode.pop();
					gGameMode.onEnter();
				}
				break;

			case 2: // menu
				if (prevMenu == 1) { // from localmap
					gGameMode.push("menu");
					gGameMode.onEnter();
				}
				else if (prevMenu == 3) { // from battle
					gGameMode.onExit();
					gGameMode.pop();
					gGameMode.onEnter();
				}
				break;

			case 3: // battle
				if (prevMenu == 1) { // from localmap
					mobList = gGameMode.onExitToBattle();
					gGameMode.add("battle", new BattleState(playerList, mobList));
					gGameMode.push("battle");
					gGameMode.onEnter();
				}
				break;

			default:
				break;
			}
		}
		prevMenu = menu;

		while (window->pollEvent(event))
		{
			//sf::Vector2i localPosition = sf::Mouse::getPosition(*window);
			/*for (unsigned int i = 0; i < buttonList.size(); ++i)
			{
				if (buttonList[i]->getIsClickable() && buttonList[i]->testIsClicked(localPosition.x, localPosition.y))
				{
					buttonList[i]->showGlow(true);
				}
				else
				{
					buttonList[i]->showGlow(false);
				}
			}*/

			switch (event.type)
			{
			case sf::Event::Closed:
				window->close();
				break;

			case sf::Event::Resized:
				prevHeight = newHeight;
				newHeight = event.size.height;
				if (prevHeight != newHeight) {
					mult = (double)newHeight / prevHeight;
					prevWidth = newWidth;
					newWidth = (int)(prevWidth * mult);
				}
				else {
					prevWidth = newWidth;
					newWidth = event.size.width;
					mult = (double)newWidth / prevWidth;
					newHeight = (int)(prevHeight * mult);
				}
				window->setSize(sf::Vector2u(newWidth, newHeight));
				gGameMode.resize(mult);
				/*for (unsigned int i = 0; i < buttonList.size(); ++i)
				{
					buttonList[i]->resize(mult);
				}*/
				break;

			case sf::Event::KeyPressed:
				break;

			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					gGameMode.click(localPosition);
					/*for (unsigned int i = 0; i < buttonList.size(); ++i)
					{
						if (buttonList[i]->getIsClickable() && buttonList[i]->testIsClicked(localPosition.x, localPosition.y))
						{
							buttonList[i]->setIsClicked(true);
						}
						else
						{
							buttonList[i]->setIsClicked(false);
						}
					}*/
				}
				break;

			default:
				break;
			}
		}

		// Check for clicked button
		/*for (unsigned int i = 0; i < buttonList.size(); ++i)
		{
			if (buttonList[i]->getIsClicked())
			{
				int clickedID = buttonList[i]->getID();
				switch (clickedID)
				{
				case 0:
					menu = 1;
					buttonList[i]->setIsClickable(false);
					buttonList[i]->hide();
					buttonList[1]->setIsClickable(true);
					buttonList[1]->show();
					//mainMenuBG.setColor(sf::Color(255, 255, 255, 0));
					break;

				case 1:
					menu = 2;
					buttonList[i]->setIsClickable(false);
					buttonList[2]->setIsClickable(true);
					buttonList[2]->show();
					pauseScreen.setFillColor(sf::Color(0, 0, 0, 127));
					break;

				case 2:
					menu = 1;
					buttonList[i]->setIsClickable(false);
					buttonList[i]->hide();
					buttonList[1]->setIsClickable(true);
					pauseScreen.setFillColor(sf::Color(0, 0, 0, 0));
					break;

				default:
					break;
				}
			}
		}*/

		/*if (menu == 1)
		{
			// Player movement
			if (!isJump)
			{
				isFall = player->down(walkmask, currLevel, currLevelWidth);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				player->right(walkmask, currLevel, currLevelWidth);
				if (currFrame <= 5)
				{

				}
				else if (currFrame <= 10)
				{

				}
				else if (currFrame <= 15)
				{

				}
				else if (currFrame <= 20)
				{

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isFall)
				{
					if (!isJump)
					{
						isJump = true;
					}
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				player->left(walkmask, currLevel, currLevelWidth);
				if (currFrame <= 10)
				{

				}
				else if (currFrame <= 20)
				{

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isFall)
				{
					if (!isJump)
					{
						isJump = true;
					}
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isFall)
			{
				if (!isJump)
				{
					isJump = true;
				}
			}

			// Jump
			if (isJump)
			{
				if (jumpFrame < 2)
				{
					++jumpFrame;
					player->jump(walkmask, currLevel, currLevelWidth);
					player->jump(walkmask, currLevel, currLevelWidth);
					player->jump(walkmask, currLevel, currLevelWidth);
				}
				else if (jumpFrame < 4)
				{
					++jumpFrame;
					player->jump(walkmask, currLevel, currLevelWidth);
				}
				else if (jumpFrame < 10)
				{
					++jumpFrame;
				}
				else
				{
					isJump = false;
					isFall = player->down(walkmask, currLevel, currLevelWidth);
					jumpFrame = 0;
				}
			}
			if (!isFall)
			{
				fallFrame = 0;
			}
			if (isFall)
			{
				if (fallFrame > 8)
				{
					isFall = player->down(walkmask, currLevel, currLevelWidth);
				}
				else
				{
					++fallFrame;
				}
			}
		}*/

		//window->clear(sf::Color::Black);
		gGameMode.render(window);
		//window->draw(currLevel);
		//window->draw(player->getSprite());
		//window->draw(pauseScreen);
		//window->draw(mainMenuBG);
		//for (unsigned int i = 0; i < buttonList.size(); ++i)
		//{
		//	window->draw(buttonList[i]->getSprite());
		//}
		window->display();
		++currFrame;
		if (currFrame > 30)
		{
			currFrame = 1;
		}
	}
    return 0;
}