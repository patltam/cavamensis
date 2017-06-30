#include "stdafx.h"
#include "Entity.h"
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#pragma once

class Mob
{
public:
	sf::Texture sheet;
	sf::Sprite sprites;
	sf::Vector2f position;
	int width;
	int height;
	MobEntity* mob1;
	MobEntity* mob2;
	MobEntity* mob3;
	MobEntity* mob4;
	MobEntity* mob5;
	std::vector<MobEntity*> mobList;

	Mob() {}
	virtual ~Mob() {}
	virtual void setOrigin(sf::Vector2f pos) = 0;
	virtual void setPosition(sf::Vector2f pos) = 0;
	virtual sf::Vector2f getPosition() = 0;
	virtual sf::Sprite getSprite() = 0;
	virtual int getWidth() = 0;
	virtual std::vector<MobEntity*> getMobList() = 0;
	virtual void setSpriteStage(int value) = 0;
	virtual void right(std::vector<int>& wm, int wmWidth) = 0;
	virtual void left(std::vector<int>& wm, int wmWidth) = 0;
	virtual void jump(std::vector<int>& wm, int wmWidth) = 0;
	virtual bool down(std::vector<int>& wm, int wmWidth) = 0;
	virtual void move(std::vector<int>& wm, int wmWidth, sf::Vector2f playerPos) = 0;
};

class Mob0 : public Mob
{
public:
	Mob0() {
		sheet.loadFromFile("sprites/mob0.png");
		sprites.setTexture(sheet);
		position.x = 0;
		position.y = 0;
		width = 1;
		height = 1;
		mob1 = new MobEntity();
		mob2 = new MobEntity();
		mob3 = new MobEntity();
		mob4 = new MobEntity();
		mob5 = new MobEntity();
		mobList.push_back(mob1);
		mobList.push_back(mob2);
		mobList.push_back(mob3);
		mobList.push_back(mob4);
		mobList.push_back(mob5);
	}

	void setOrigin(sf::Vector2f pos) {
		sprites.setOrigin(pos);
	}

	void setPosition(sf::Vector2f pos) {
		position.x = pos.x;
		position.y = pos.y;
		sprites.setPosition(pos);
	}

	sf::Vector2f getPosition() {
		return position;
	}

	sf::Sprite getSprite() {
		return sprites;
	}

	int getWidth() {
		return this->width;
	}

	void setSpriteStage(int value) {

	}

	std::vector<MobEntity*> getMobList() {
		return this->mobList;
	}

	void right(std::vector<int>& wm, int wmWidth)
	{
		int nextX = (int)this->position.x + this->width;
		// check for wall
		for (int i = 0; i < this->height - 1; ++i)
		{
			if (wm[((int)this->position.y + i) * wmWidth + nextX] == 1)
			{
				return;
			}
		}
		// check for incline
		if (wm[((int)this->position.y + this->height - 1) * wmWidth + nextX] == 1)
		{
			// check for adequate head space
			if (wm[((int)this->position.y - 1) * wmWidth + nextX] == 1)
			{
				return;
			}
			else
			{
				this->position = sf::Vector2f(this->position.x + 1, this->position.y - 1);
				return;
			}
		}
		// check for decline
		if (wm[((int)this->position.y + this->height) * wmWidth + (int)this->position.x] == 1)
		{
			if (wm[((int)this->position.y + this->height) * wmWidth + (int)this->position.x + 1] == 0)
			{
				if (wm[((int)this->position.y + this->height + 1) * wmWidth + (int)this->position.x + 1] == 1)
				{
					this->position = sf::Vector2f(this->position.x + 1, this->position.y + 1);
					return;
				}
			}
		}
		// check for ceiling decline
		if (wm[(int)this->position.y * wmWidth + nextX] == 1)
		{
			if (wm[((int)this->position.y + this->height) * wmWidth + nextX] == 0)
			{
				if (wm[((int)this->position.y + this->height) * wmWidth + nextX - 1] == 0)
				{
					this->position = sf::Vector2f(this->position.x + 1, this->position.y + 1);
					return;
				}
			}
		}
		this->position = sf::Vector2f(this->position.x + 1, this->position.y);
		return;
	}

	void left(std::vector<int>& wm, int wmWidth)
	{
		int nextX = (int)this->position.x - 1;
		// check for wall
		for (int i = 0; i < this->height - 1; ++i)
		{
			if (wm[((int)this->position.y + i) * wmWidth + nextX] == 1)
			{
				return;
			}
		}
		// check for incline
		if (wm[((int)this->position.y + this->height - 1) * wmWidth + nextX] == 1)
		{
			// check for adequate head space
			if (wm[((int)this->position.y - 1) * wmWidth + nextX] == 1)
			{
				return;
			}
			else
			{
				this->position = sf::Vector2f(this->position.x - 1, this->position.y - 1);
				return;
			}
		}
		// check for decline
		if (wm[((int)this->position.y + this->height) * wmWidth + (int)this->position.x + 1] == 1)
		{
			if (wm[((int)this->position.y + this->height) * wmWidth + (int)this->position.x] == 0)
			{
				if (wm[((int)this->position.y + this->height + 1) * wmWidth + (int)this->position.x] == 1)
				{
					this->position = sf::Vector2f(this->position.x - 1, this->position.y + 1);
					return;
				}
			}
		}
		// check for ceiling decline
		if (wm[(int)this->position.y * wmWidth + nextX] == 1)
		{
			if (wm[((int)this->position.y + this->height) * wmWidth + nextX] == 0)
			{
				if (wm[((int)this->position.y + this->height) * wmWidth + nextX - 1] == 0)
				{
					this->position = sf::Vector2f(this->position.x - 1, this->position.y + 1);
					return;
				}
			}
		}
		this->position = sf::Vector2f(this->position.x - 1, this->position.y);
		return;
	}

	void jump(std::vector<int>& wm, int wmWidth)
	{
		int nextY = (int)this->position.y - 1;
		for (int i = 0; i < this->width; ++i)
		{
			if (wm[nextY * wmWidth + ((int)this->position.x + i)] == 1)
			{
				return;
			}
		}
		this->position = sf::Vector2f(this->position.x, this->position.y - 1);
		return;
	}

	bool down(std::vector<int>& wm, int wmWidth)
	{
		int nextY = (int)this->position.y + this->height;
		for (int i = 0; i < this->width; ++i)
		{
			if (wm[nextY * wmWidth + ((int)this->position.x + i)] == 1)
			{
				return 0;
			}
		}
		this->position = sf::Vector2f(this->position.x, this->position.y + 1);
		return 1;
	}

	void move(std::vector<int>& wm, int wmWidth, sf::Vector2f playerPos) {
		srand((int)time(NULL));
		int proc = rand() % 3;
		if (proc == 0) {
			// stay still
		}
		else if (proc == 1) {
			right(wm, wmWidth);
		}
		else if (proc == 2) {
			left(wm, wmWidth);
		}
	}
};