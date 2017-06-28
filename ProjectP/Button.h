#include "stdafx.h"
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#pragma once

class Button
{
private:
	int ID;
	int tlX;
	int tlY;
	int width;
	int height;
	int origW;
	int origH;
	bool isClicked;
	bool isClickable;
	sf::Texture texture;
	sf::Sprite sprite;

public:
	Button(int num, int x, int y, int w, int h, std::string path);
	~Button();
	int getID();
	void resize(double value);
	bool testIsClicked(int x, int y);
	void setIsClicked(bool value);
	bool getIsClicked();
	void setIsClickable(bool value);
	bool getIsClickable();
	sf::Sprite getSprite();
	void show();
	void showGlow(bool value);
	void hide();
};