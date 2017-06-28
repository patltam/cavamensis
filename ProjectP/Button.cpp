#include "stdafx.h"
#include "Button.h"

Button::Button(int num, int x, int y, int w, int h, std::string path)
{
	ID = num;
	tlX = x;
	tlY = y;
	width = w;
	height = h;
	origW = w;
	origH = h;
	isClicked = false;
	isClickable = false;
	texture.loadFromFile(path);
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, w, h));
	sprite.setPosition(sf::Vector2f((float)x, (float)y));
}

Button::~Button()
{

}

int Button::getID()
{
	return this->ID;
}

void Button::resize(double value)
{
	this->tlX = (int)((double)this->tlX * value);
	this->tlY = (int)((double)this->tlY * value);
	this->width = (int)((double)this->width * value);
	this->height = (int)((double)this->height * value);
}

bool Button::testIsClicked(int x, int y)
{
	if (x >= this->tlX && x <= (this->tlX + this->width) &&
		y >= this->tlY && y <= (this->tlY + this->height))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Button::setIsClicked(bool value)
{
	this->isClicked = value;
}

bool Button::getIsClicked()
{
	return this->isClicked;
}

void Button::setIsClickable(bool value)
{
	this->isClickable = value;
}

bool Button::getIsClickable()
{
	return this->isClickable;
}

sf::Sprite Button::getSprite()
{
	return this->sprite;
}

void Button::show()
{
	this->sprite.setColor(sf::Color(255, 255, 255, 255));
}

void Button::showGlow(bool value)
{
	if (value)
	{
		this->sprite.setTextureRect(sf::IntRect(0, this->origH, this->origW, this->origH));
	}
	else
	{
		this->sprite.setTextureRect(sf::IntRect(0, 0, this->origW, this->origH));
	}
}

void Button::hide()
{
	this->sprite.setColor(sf::Color(255, 255, 255, 0));
}