#include "stdafx.h"
#include "Button.h"
#include "Player.h"
#include "Action.h"
#include "Entity.h"
#include "Mob.h"
#include <string>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class IState
{
public:
	IState() {}
	virtual ~IState() {}
	virtual int update(float currFrame, sf::Vector2i mousePosition) = 0;
	virtual void resize(double mult) = 0;
	virtual void click(sf::Vector2i mousePosition) = 0;
	virtual void render(sf::RenderWindow* window) = 0;
	virtual void onEnter() = 0;
	virtual void onExit() = 0;
	virtual std::vector<MobEntity*> onExitToBattle() = 0;
};

class EmptyState : public IState
{
public:
	int update(float currFrame, sf::Vector2i mousePosition) {
		return 0;
	}

	void resize(double mult) {
		// Nothing to resize in the empty state
	}

	void click(sf::Vector2i mousePosition) {
		// Nothing to click in the empty state
	}

	void render(sf::RenderWindow* window) {
		// Nothing to render in the empty state
	}

	void onEnter() {
		// No action to take when the state is entered
	}

	void onExit() {
		// No action to take when the state is exited
	}

	std::vector<MobEntity*> onExitToBattle() {
		// No action to take when the state is exited
	}
};

class MainMenuState : public IState
{
private:
	sf::Texture mmbg;
	sf::Sprite mainMenuBG;
	std::vector<Button*> buttonList;
	Button* startButton;
	std::vector<MobEntity*> mobList;

public:
	MainMenuState() {
		mmbg.loadFromFile("ui/mainmenubg.png");
		mainMenuBG.setTexture(mmbg);
		startButton = new Button(0, 600, 525, 400, 42, "ui/startbutton.png");
		startButton->setIsClickable(true);
		buttonList.push_back(startButton); // ID = 0
	}

	int update(float currFrame, sf::Vector2i mousePosition) {
		// check for mouse over button
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			if (buttonList[i]->getIsClickable() && buttonList[i]->testIsClicked(mousePosition.x, mousePosition.y)) {
				buttonList[i]->showGlow(true);
			}
			else {
				buttonList[i]->showGlow(false);
			}
		}
		// check for clicked button
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			if (buttonList[i]->getIsClicked()) {
				int clickedID = buttonList[i]->getID();
				buttonList[i]->setIsClicked(false);
				switch (clickedID) {
				case 0:
					return 1;
					break;

				default:
					break;
				}
			}
		}
		return 0;
	}

	void resize(double mult) {
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			std::cout << "Resizing main menu buttons\n";
			buttonList[i]->resize(mult);
		}
	}

	void click(sf::Vector2i mousePosition) {
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			if (buttonList[i]->getIsClickable() && buttonList[i]->testIsClicked(mousePosition.x, mousePosition.y)) {
				buttonList[i]->setIsClicked(true);
			}
			else {
				buttonList[i]->setIsClicked(false);
			}
		}
	}

	void render(sf::RenderWindow* window) {
		window->draw(mainMenuBG);
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			window->draw(buttonList[i]->getSprite());
		}
	}

	void onEnter() {
		mainMenuBG.setColor(sf::Color(255, 255, 255, 255));
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			buttonList[i]->setIsClicked(false);
		}
	}

	void onExit() {
		mainMenuBG.setColor(sf::Color(255, 255, 255, 0));
	}

	std::vector<MobEntity*> onExitToBattle() {
		mainMenuBG.setColor(sf::Color(255, 255, 255, 0));
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			buttonList[i]->setIsClicked(false);
		}
		return mobList;
	}
};

class MapState : public IState
{
private:
	std::vector<MobEntity*> mobList; // for setting up battles
	std::vector<Mob*> spawnedMobList; // for mobs roaming the map
	std::vector<Button*> buttonList;
	Button* menuButton;
	sf::Texture level;
	sf::Sprite currLevel;
	std::vector<int> walkmask;
	int currLevelWidth;
	Player* player;
	bool isJump;
	int jumpCD;
	int jumpFrame;
	bool isFall;
	int fallFrame;
	bool lastRight;
	int playerHbLeft;
	int playerHbRight;
	int playerHbTop;
	int playerHbBottom;
	int playerHbWidth;
	int playerHbHeight;
	bool collideX1;
	bool collideX2;
	bool collideX3;
	bool collideX4;
	bool collideY1;
	bool collideY2;
	bool collideY3;
	bool collideY4;

public:
	MapState() {
		isJump = false;
		jumpCD = -1;
		jumpFrame = 0;
		isFall = false;
		fallFrame = 0;
		lastRight = true;
		menuButton = new Button(1, 1521, 0, 79, 31, "ui/menubutton.png");
		menuButton->setIsClickable(true);
		buttonList.push_back(menuButton); // ID = 1
		player = new Player(0, 0, 2, 8, "char/runs.png");

		level.loadFromFile("maps/veterna.png");
		currLevel.setTexture(level);
		currLevel.setScale(sf::Vector2f(12, 12));
		walkmask = readBMP("maps/veternawm.bmp"); // 24-bit BMP
		currLevelWidth = 700;
		spawnIn(175, 200, currLevel, level, walkmask, "maps/veternawm.bmp", player);
		Mob0* dummy = new Mob0();
		dummy->setPosition(sf::Vector2f(175, 200), player->getPosition());
		spawnedMobList.push_back(dummy);
	}

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
		std::vector<int> walkmask(width * height, 0);
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
		currLevel.move((float)(-(x * 12 - 810)), (float)(-(y * 12 - 375)));
		walkmask = readBMP(path);
		player->setPosition(sf::Vector2f((float)x, (float)y));
	}

	int update(float currFrame, sf::Vector2i mousePosition)
	{
		//std::cout << player->getPosition().x << ", " << player->getPosition().y << "\n";
		//std::cout << "Update checker\n";
		if (lastRight) {
			player->setSpriteStage(0);
		}
		else {
			player->setSpriteStage(9);
		}
		// Player movement
		if (!isJump)
		{
			isFall = player->down(walkmask, currLevel, currLevelWidth);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			lastRight = true;
			player->right(walkmask, currLevel, currLevelWidth);
			if (currFrame <= 3)
			{
				player->setSpriteStage(1);
			}
			else if (currFrame <= 7)
			{
				player->setSpriteStage(2);
			}
			else if (currFrame <= 11)
			{
				player->setSpriteStage(3);
			}
			else if (currFrame <= 15)
			{
				player->setSpriteStage(4);
			}
			else if (currFrame <= 18)
			{
				player->setSpriteStage(1);
			}
			else if (currFrame <= 22)
			{
				player->setSpriteStage(2);
			}
			else if (currFrame <= 26)
			{
				player->setSpriteStage(3);
			}
			else if (currFrame <= 30)
			{
				player->setSpriteStage(4);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isFall && (jumpCD == -1))
			{
				if (!isJump)
				{
					isJump = true;
				}
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			lastRight = false;
			player->left(walkmask, currLevel, currLevelWidth);
			if (currFrame <= 3)
			{
				player->setSpriteStage(5);
			}
			else if (currFrame <= 7)
			{
				player->setSpriteStage(6);
			}
			else if (currFrame <= 11)
			{
				player->setSpriteStage(7);
			}
			else if (currFrame <= 15)
			{
				player->setSpriteStage(8);
			}
			else if (currFrame <= 18)
			{
				player->setSpriteStage(5);
			}
			else if (currFrame <= 22)
			{
				player->setSpriteStage(6);
			}
			else if (currFrame <= 26)
			{
				player->setSpriteStage(7);
			}
			else if (currFrame <= 30)
			{
				player->setSpriteStage(8);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isFall && (jumpCD == -1))
			{
				if (!isJump)
				{
					isJump = true;
				}
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isFall && (jumpCD == -1))
		{
			if (!isJump && !isFall)
			{
					isJump = true;
			}
		}

		// Jump
		if (isJump)
		{
			if (lastRight) {
				player->setSpriteStage(10);
			}
			else {
				player->setSpriteStage(12);
			}
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
				jumpCD = 0;
			}
		}
		if (!isFall)
		{
			fallFrame = 0;
			if (jumpCD != -1) {
				++jumpCD;
			}
			if (jumpCD >= 5) {
				jumpCD = -1;
			}
		}
		if (isFall)
		{
			if (lastRight) {
				player->setSpriteStage(11);
			}
			else {
				player->setSpriteStage(13);
			}
			if (fallFrame > 8)
			{
				isFall = player->down(walkmask, currLevel, currLevelWidth);
				if (!isFall) {
					jumpCD = 0;
				}
			}
			else
			{
				++fallFrame;
			}
		}

		// check for mob collision
		playerHbLeft = player->getSprite().getPosition().x + player->getHbtlx();
		playerHbWidth = player->getHbWidth();
		playerHbRight = playerHbLeft + playerHbWidth;
		playerHbTop = player->getSprite().getPosition().y + player->getHbtly();
		playerHbHeight = player->getHbHeight();
		playerHbBottom = playerHbTop + playerHbHeight;
		for (unsigned int i = 0; i < spawnedMobList.size(); ++i) {
			collideX1 = playerHbLeft >
				spawnedMobList[i]->getSprite().getPosition().x + spawnedMobList[i]->getHbtlx() &&
				playerHbLeft <
				spawnedMobList[i]->getSprite().getPosition().x + spawnedMobList[i]->getHbtlx() + spawnedMobList[i]->getHbWidth();
			collideX2 = playerHbRight >
				spawnedMobList[i]->getSprite().getPosition().x + spawnedMobList[i]->getHbtlx() &&
				playerHbRight <
				spawnedMobList[i]->getSprite().getPosition().x + spawnedMobList[i]->getHbtlx() + spawnedMobList[i]->getHbWidth();
			collideX3 = spawnedMobList[i]->getSprite().getPosition().x + spawnedMobList[i]->getHbtlx() >
				playerHbLeft &&
				spawnedMobList[i]->getSprite().getPosition().x + spawnedMobList[i]->getHbtlx() <
				playerHbRight;
			collideX4 = spawnedMobList[i]->getSprite().getPosition().x + spawnedMobList[i]->getHbtlx() + spawnedMobList[i]->getHbWidth() >
				playerHbLeft &&
				spawnedMobList[i]->getSprite().getPosition().x + spawnedMobList[i]->getHbtlx() + spawnedMobList[i]->getHbWidth() <
				playerHbRight;
			collideY1 = playerHbTop >
				spawnedMobList[i]->getSprite().getPosition().y + spawnedMobList[i]->getHbtly() &&
				playerHbTop <
				spawnedMobList[i]->getSprite().getPosition().y + spawnedMobList[i]->getHbtly() + spawnedMobList[i]->getHbHeight();
			collideY2 = playerHbBottom >
				spawnedMobList[i]->getSprite().getPosition().y + spawnedMobList[i]->getHbtly() &&
				playerHbBottom <
				spawnedMobList[i]->getSprite().getPosition().y + spawnedMobList[i]->getHbtly() + spawnedMobList[i]->getHbHeight();
			collideY3 = spawnedMobList[i]->getSprite().getPosition().y + spawnedMobList[i]->getHbtly() >
				playerHbTop &&
				spawnedMobList[i]->getSprite().getPosition().y + spawnedMobList[i]->getHbtly() <
				playerHbBottom;
			collideY4 = spawnedMobList[i]->getSprite().getPosition().y + spawnedMobList[i]->getHbtly() + spawnedMobList[i]->getHbHeight() >
				playerHbTop &&
				spawnedMobList[i]->getSprite().getPosition().y + spawnedMobList[i]->getHbtly() + spawnedMobList[i]->getHbHeight() <
				playerHbBottom;
			if (collideX1 || collideX2 || collideX3 || collideX4) {
				if (collideY1 || collideY2 || collideY3 || collideY4) {
					std::cout << "Collision!\n";
				}
			}
		}

		// check for mouse over button
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			if (buttonList[i]->getIsClickable() && buttonList[i]->testIsClicked(mousePosition.x, mousePosition.y)) {
				buttonList[i]->showGlow(true);
			}
			else {
				buttonList[i]->showGlow(false);
			}
		}
		// check for clicked button
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			if (buttonList[i]->getIsClicked()) {
				int clickedID = buttonList[i]->getID();
				buttonList[i]->setIsClicked(false);
				switch (clickedID) {
				case 1:
					return 2;
					break;

				default:
					break;
				}
			}
		}

		// mob movement
		for (unsigned int i = 0; i < spawnedMobList.size(); ++i) {
			spawnedMobList[i]->move(walkmask, currLevelWidth, player->getPosition());
		}

		return 1;
	}

	void resize(double mult) {
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			std::cout << "Resizing localmap buttons\n";
			buttonList[i]->resize(mult);
		}
	}

	void click(sf::Vector2i mousePosition) {
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			if (buttonList[i]->getIsClickable() && buttonList[i]->testIsClicked(mousePosition.x, mousePosition.y)) {
				buttonList[i]->setIsClicked(true);
			}
			else {
				buttonList[i]->setIsClicked(false);
			}
		}
	}

	void render(sf::RenderWindow* window)
	{
		window->draw(currLevel);
		//std::cout << player->getPosition().x << ", " << player->getPosition().y << "\n";
		for (unsigned int i = 0; i < spawnedMobList.size(); ++i) {
			window->draw(spawnedMobList[i]->getSprite());
		}
		window->draw(player->getSprite());
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			window->draw(buttonList[i]->getSprite());
		}
	}

	void onEnter()
	{
		currLevel.setColor(sf::Color(255, 255, 255, 255));
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			buttonList[i]->setIsClicked(false);
		}
	}

	void onExit()
	{
		currLevel.setColor(sf::Color(255, 255, 255, 0));
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			buttonList[i]->setIsClicked(false);
		}
	}

	std::vector<MobEntity*> onExitToBattle() {
		currLevel.setColor(sf::Color(255, 255, 255, 0));
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			buttonList[i]->setIsClicked(false);
		}
		return mobList;
	}
};

class MenuState : public IState
{
private:
	std::vector<Button*> buttonList;
	Button* backButton;
	Button* battleButton;
	sf::Texture menubgt;
	sf::Sprite menubg;
	std::vector<MobEntity*> mobList;

public:
	MenuState() {
		backButton = new Button(2, 1521, 0, 79, 31, "ui/backbutton.png");
		backButton->setIsClickable(true);
		battleButton = new Button(3, 1521, 30, 79, 31, "ui/battlebutton.png");
		battleButton->setIsClickable(true);
		buttonList.push_back(backButton); // ID = 2
		buttonList.push_back(battleButton); // ID = 3
		menubgt.loadFromFile("ui/menubgtest.png");
		menubg.setTexture(menubgt);
	}

	int update(float currFrame, sf::Vector2i mousePosition)
	{
		// check for mouse over button
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			if (buttonList[i]->getIsClickable() && buttonList[i]->testIsClicked(mousePosition.x, mousePosition.y)) {
				buttonList[i]->showGlow(true);
			}
			else {
				buttonList[i]->showGlow(false);
			}
		}
		// check for clicked button
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			if (buttonList[i]->getIsClicked()) {
				int clickedID = buttonList[i]->getID();
				buttonList[i]->setIsClicked(false);
				switch (clickedID) {
				case 2:
					return 1;
					break;

				case 3:
					return 3;
					break;

				default:
					break;
				}
			}
		}
		return 2;
	}

	void resize(double mult) {
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			std::cout << "Resizing menu buttons\n";
			buttonList[i]->resize(mult);
		}
	}

	void click(sf::Vector2i mousePosition) {
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			if (buttonList[i]->getIsClickable() && buttonList[i]->testIsClicked(mousePosition.x, mousePosition.y)) {
				buttonList[i]->setIsClicked(true);
			}
			else {
				buttonList[i]->setIsClicked(false);
			}
		}
	}

	void render(sf::RenderWindow* window)
	{
		window->draw(menubg);
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			window->draw(buttonList[i]->getSprite());
		}
	}

	void onEnter()
	{
		menubg.setColor(sf::Color(255, 255, 255, 255));
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			buttonList[i]->setIsClicked(false);
		}
	}

	void onExit()
	{
		menubg.setColor(sf::Color(255, 255, 255, 0));
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			buttonList[i]->setIsClicked(false);
		}
	}

	std::vector<MobEntity*> onExitToBattle() {
		menubg.setColor(sf::Color(255, 255, 255, 0));
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			buttonList[i]->setIsClicked(false);
		}
		return mobList;
	}
};

class BattleState : public IState
{
private:
	std::vector<Action> mActions;
	std::vector<Entity*> mEntities;
	PlayerEntity* playerEnt1;
	PlayerEntity* playerEnt2;
	PlayerEntity* playerEnt3;
	PlayerEntity* playerEnt4;
	MobEntity* mobEnt1;
	MobEntity* mobEnt2;
	MobEntity* mobEnt3;
	MobEntity* mobEnt4;
	MobEntity* mobEnt5;
	std::vector<Button*> buttonList;
	Button* backButton;
	sf::Texture bgt;
	sf::Sprite bg;
	sf::Texture battleMenut;
	sf::Sprite battleMenu;
	std::vector<MobEntity*> mobList;

public:
	BattleState() {
		mActions = std::vector<Action>();
		mEntities = std::vector<Entity*>();

		playerEnt1 = new PlayerEntity();
		playerEnt1->setOrigin(sf::Vector2f(0, 148));
		playerEnt1->setPosition(sf::Vector2f(450, 298));
		mEntities.push_back(playerEnt1);
		playerEnt2 = new PlayerEntity();
		playerEnt2->setOrigin(sf::Vector2f(0, 148));
		playerEnt2->setPosition(sf::Vector2f(430, 398));
		mEntities.push_back(playerEnt2);
		playerEnt3 = new PlayerEntity();
		playerEnt3->setOrigin(sf::Vector2f(0, 148));
		playerEnt3->setPosition(sf::Vector2f(410, 498));
		mEntities.push_back(playerEnt3);
		playerEnt4 = new PlayerEntity();
		playerEnt4->setOrigin(sf::Vector2f(0, 148));
		playerEnt4->setPosition(sf::Vector2f(390, 598));
		mEntities.push_back(playerEnt4);

		mobEnt1 = new MobEntity();
		mobEnt1->setOrigin(sf::Vector2f(0, 100));
		mobEnt1->setPosition(sf::Vector2f(1100, 248));
		mEntities.push_back(mobEnt1);
		mobEnt2 = new MobEntity();
		mobEnt2->setOrigin(sf::Vector2f(0, 100));
		mobEnt2->setPosition(sf::Vector2f(1120, 338));
		mEntities.push_back(mobEnt2);
		mobEnt3 = new MobEntity();
		mobEnt3->setOrigin(sf::Vector2f(0, 100));
		mobEnt3->setPosition(sf::Vector2f(1140, 428));
		mEntities.push_back(mobEnt3);
		mobEnt4 = new MobEntity();
		mobEnt4->setOrigin(sf::Vector2f(0, 100));
		mobEnt4->setPosition(sf::Vector2f(1160, 518));
		mEntities.push_back(mobEnt4);
		mobEnt5 = new MobEntity();
		mobEnt5->setOrigin(sf::Vector2f(0, 100));
		mobEnt5->setPosition(sf::Vector2f(1180, 608));
		mEntities.push_back(mobEnt5);

		backButton = new Button(2, 1521, 0, 79, 31, "ui/backbutton.png");
		backButton->setIsClickable(true);
		buttonList.push_back(backButton); // ID = 2
		bgt.loadFromFile("battlebg/bg0.png");
		bg.setTexture(bgt);
		battleMenut.loadFromFile("ui/battlemenu.png");
		battleMenu.setTexture(battleMenut);
		battleMenu.setPosition(sf::Vector2f(0, 600));
	}

	BattleState(std::vector<PlayerEntity*> playerList, std::vector<MobEntity*> mobList) {
		mActions = std::vector<Action>();
		mEntities = std::vector<Entity*>();

		playerEnt1 = playerList[0];
		playerEnt1->setOrigin(sf::Vector2f(0, 148));
		playerEnt1->setPosition(sf::Vector2f(450, 298));
		mEntities.push_back(playerEnt1);
		playerEnt2 = playerList[1];
		playerEnt2->setOrigin(sf::Vector2f(0, 148));
		playerEnt2->setPosition(sf::Vector2f(430, 398));
		mEntities.push_back(playerEnt2);
		playerEnt3 = playerList[2];
		playerEnt3->setOrigin(sf::Vector2f(0, 148));
		playerEnt3->setPosition(sf::Vector2f(410, 498));
		mEntities.push_back(playerEnt3);
		playerEnt4 = playerList[3];
		playerEnt4->setOrigin(sf::Vector2f(0, 148));
		playerEnt4->setPosition(sf::Vector2f(390, 598));
		mEntities.push_back(playerEnt4);

		mobEnt1 = mobList[0];
		mobEnt1->setOrigin(sf::Vector2f(0, 100));
		mobEnt1->setPosition(sf::Vector2f(1100, 248));
		mEntities.push_back(mobEnt1);
		mobEnt2 = mobList[1];
		mobEnt2->setOrigin(sf::Vector2f(0, 100));
		mobEnt2->setPosition(sf::Vector2f(1120, 338));
		mEntities.push_back(mobEnt2);
		mobEnt3 = mobList[2];
		mobEnt3->setOrigin(sf::Vector2f(0, 100));
		mobEnt3->setPosition(sf::Vector2f(1140, 428));
		mEntities.push_back(mobEnt3);
		mobEnt4 = mobList[3];
		mobEnt4->setOrigin(sf::Vector2f(0, 100));
		mobEnt4->setPosition(sf::Vector2f(1160, 518));
		mEntities.push_back(mobEnt4);
		mobEnt5 = mobList[4];
		mobEnt5->setOrigin(sf::Vector2f(0, 100));
		mobEnt5->setPosition(sf::Vector2f(1180, 608));
		mEntities.push_back(mobEnt5);

		backButton = new Button(2, 1521, 0, 79, 31, "ui/backbutton.png");
		backButton->setIsClickable(true);
		buttonList.push_back(backButton); // ID = 2
		bgt.loadFromFile("battlebg/bg0.png");
		bg.setTexture(bgt);
		battleMenut.loadFromFile("ui/battlemenu.png");
		battleMenu.setTexture(battleMenut);
		battleMenu.setPosition(sf::Vector2f(0, 600));
	}

	/*static bool sortByTime(Action a, Action b)
	{
		return a.TimeRemaining() > b.TimeRemaining()
	}

	void onEnter()
	{
		mBattleStates.Change("tick");

		mEntities = params.entities;

		foreach(Entity e in mEntities)
		{
			if (e.playerControlled)
			{
				PlayerDecide action = new PlayerDecide(e, e.Speed());
				mActions.Add(action);
			}
			else
			{
				AIDecide action = new AIDecide(e, e.Speed());
				mActions.Add(action);
			}
		}

		Sort(mActions, BattleState::SortByTime);
	}*/

	int update(float currFrame, sf::Vector2i mousePosition)
	{
		// check for mouse over button
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			if (buttonList[i]->getIsClickable() && buttonList[i]->testIsClicked(mousePosition.x, mousePosition.y)) {
				buttonList[i]->showGlow(true);
			}
			else {
				buttonList[i]->showGlow(false);
			}
		}
		// check for clicked button
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			if (buttonList[i]->getIsClicked()) {
				int clickedID = buttonList[i]->getID();
				buttonList[i]->setIsClicked(false);
				switch (clickedID) {
				case 2:
					return 2;
					break;

				default:
					break;
				}
			}
		}
		return 3;
	}
	
	void resize(double mult) {
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			std::cout << "Resizing battle buttons\n";
			buttonList[i]->resize(mult);
		}
	}

	void click(sf::Vector2i mousePosition) {
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			if (buttonList[i]->getIsClickable() && buttonList[i]->testIsClicked(mousePosition.x, mousePosition.y)) {
				buttonList[i]->setIsClicked(true);
			}
			else {
				buttonList[i]->setIsClicked(false);
			}
		}
	}

	void render(sf::RenderWindow* window)
	{
		window->draw(bg);
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			window->draw(buttonList[i]->getSprite());
		}
		for (unsigned int i = 0; i < mEntities.size(); ++i) {
			window->draw(mEntities[i]->getSprite());
		}
		window->draw(battleMenu);
	}

	void onEnter()
	{
		bg.setColor(sf::Color(255, 255, 255, 255));
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			buttonList[i]->setIsClicked(false);
		}
	}

	void onExit()
	{
		bg.setColor(sf::Color(255, 255, 255, 0));
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			buttonList[i]->setIsClicked(false);
		}
	}

	std::vector<MobEntity*> onExitToBattle() {
		bg.setColor(sf::Color(255, 255, 255, 0));
		for (unsigned int i = 0; i < buttonList.size(); ++i) {
			buttonList[i]->setIsClicked(false);
		}
		return mobList;
	}

	int getActive() {
		int counter = 0;
		for (unsigned int i = 0; i < mEntities.size(); ++i) {
			if (mEntities[i]->getHP() > 0) {
				++counter;
			}
		}
		return counter;
	}

	std::vector<Entity*> sortBySpeed() {
		int counter = 0;

	}
};