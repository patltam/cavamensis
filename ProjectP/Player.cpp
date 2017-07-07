#include "stdafx.h"
#include "Player.h"

Player::Player(int x, int y, int w, int h, std::string path)
{
	position = sf::Vector2f((float)x, (float)y);
	width = w;
	height = h;
	hbtlx = 44;
	hbtly = 12;
	hbWidth = 24;
	hbHeight = 120;
	sheet.loadFromFile(path);
	sprites.setTexture(sheet);
	sprites.setTextureRect(sf::IntRect(0, 0, 102, 146));
	sprites.setPosition(sf::Vector2f(774, 363));
}

Player::~Player()
{

}

void Player::right(std::vector<int>& wm, sf::Sprite& level, int wmWidth)
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
			std::cout << "Not enough head space!\n";
			return;
		}
		else
		{
			this->position = sf::Vector2f(this->position.x + 1, this->position.y - 1);
			level.move(-12, 12);
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
				level.move(-12, -12);
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
				level.move(-12, -12);
				return;
			}
		}
	}
	this->position = sf::Vector2f(this->position.x + 1, this->position.y);
	level.move(-12, 0);
	return;
}

void Player::left(std::vector<int>& wm, sf::Sprite& level, int wmWidth)
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
			level.move(12, 12);
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
				level.move(12, -12);
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
				level.move(12, -12);
				return;
			}
		}
	}
	this->position = sf::Vector2f(this->position.x - 1, this->position.y);
	level.move(12, 0);
	return;
}

void Player::jump(std::vector<int>& wm, sf::Sprite& level, int wmWidth)
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
	level.move(0, 12);
	return;
}

bool Player::down(std::vector<int>& wm, sf::Sprite& level, int wmWidth)
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
	level.move(0, -12);
	return 1;
}

int Player::getWidth()
{
	return this->width;
}

void Player::setPosition(sf::Vector2f pos)
{
	this->position = pos;
}

sf::Vector2f Player::getPosition()
{
	return this->position;
}

int Player::getHbtlx() {
	return this->hbtlx;
}

int Player::getHbtly() {
	return this->hbtly;
}

int Player::getHbWidth() {
	return this->hbWidth;
}

int Player::getHbHeight() {
	return this->hbHeight;
}

sf::Sprite Player::getSprite()
{
	return this->sprites;
}

void Player::setSpriteStage(int value) {
	switch (value) {
	case 0: // standing
		this->sprites.setTextureRect(sf::IntRect(0, 0, 102, 146));
		break;

	case 1: // run right 1
		this->sprites.setTextureRect(sf::IntRect(101, 0, 102, 146));
		break;

	case 2: // run right 2
		this->sprites.setTextureRect(sf::IntRect(203, 0, 102, 146));
		break;

	case 3: // run right 3
		this->sprites.setTextureRect(sf::IntRect(305, 0, 102, 146));
		break;

	case 4: // run right 4
		this->sprites.setTextureRect(sf::IntRect(407, 0, 102, 146));
		break;

	case 5: // run left 1
		this->sprites.setTextureRect(sf::IntRect(203, 0, -102, 146));
		break;

	case 6: // run left 2
		this->sprites.setTextureRect(sf::IntRect(305, 0, -102, 146));
		break;

	case 7: // run left 3
		this->sprites.setTextureRect(sf::IntRect(407, 0, -102, 146));
		break;

	case 8: // run left 4
		this->sprites.setTextureRect(sf::IntRect(509, 0, -102, 146));
		break;

	case 9: // standing left
		this->sprites.setTextureRect(sf::IntRect(101, 0, -102, 146));
		break;

	case 10: // jump right 1
		this->sprites.setTextureRect(sf::IntRect(509, 0, 102, 146));
		break;

	case 11: // jump right 2
		this->sprites.setTextureRect(sf::IntRect(611, 0, 102, 146));
		break;

	case 12: // jump left 1
		this->sprites.setTextureRect(sf::IntRect(611, 0, -102, 146));
		break;

	case 13: // jump left 2
		this->sprites.setTextureRect(sf::IntRect(713, 0, -102, 146));
		break;

	default:
		break;
	}
}