#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#pragma once

class Player
{
private:
	int width;
	int height;
	int hbtlx;
	int hbtly;
	int hbWidth;
	int hbHeight;
	sf::Vector2f position; // walkmask top left position
	sf::Texture sheet;
	sf::Sprite sprites;

public:
	Player(int x, int y, int w, int h, std::string path);
	~Player();
	void right(std::vector<int>& wm, sf::Sprite& level, int wmWidth);
	void left(std::vector<int>& wm, sf::Sprite& level, int wmWidth);
	void jump(std::vector<int>& wm, sf::Sprite& level, int wmWidth);
	bool down(std::vector<int>& wm, sf::Sprite& level, int wmWidth);
	int getWidth();
	int getHbtlx();
	int getHbtly();
	int getHbWidth();
	int getHbHeight();
	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition();
	sf::Sprite getSprite();
	void setSpriteStage(int value);
};