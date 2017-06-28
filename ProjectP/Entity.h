#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#pragma once

class Entity
{
public:
	sf::Texture sheet;
	sf::Sprite sprites;
	sf::Vector2f position;
	std::map<std::string, int> statuses;
	int hp;
	int mp;
	int limit;
	int atk;
	int matk;
	int def;
	int mdef;
	int speed;
	bool frontRow;

	Entity() {}
	virtual ~Entity() {}
	virtual void setOrigin(sf::Vector2f pos) = 0;
	virtual void setPosition(sf::Vector2f pos) = 0;
	virtual sf::Vector2f getPosition() = 0;
	virtual sf::Sprite getSprite() = 0;
	virtual void setSpriteStage(int value) = 0;
	virtual void setStatus(std::string type, int value) = 0;
	virtual int getStatus(std::string type) = 0;
	virtual void setHP(int value) = 0;
	virtual int getHP() = 0;
	virtual void setMP(int value) = 0;
	virtual int getMP() = 0;
	virtual void setLimit(int value) = 0;
	virtual int getLimit() = 0;
	virtual void setAtk(int value) = 0;
	virtual int getAtk() = 0;
	virtual void setMatk(int value) = 0;
	virtual int getMatk() = 0;
	virtual void setDef(int value) = 0;
	virtual int getDef() = 0;
	virtual void setMdef(int value) = 0;
	virtual int getMdef() = 0;
	virtual void setSpeed(int value) = 0;
	virtual int getSpeed() = 0;
	virtual void setFrontRow(int value) = 0;
	virtual bool getFrontRow() = 0;
};

class PlayerEntity : public Entity
{
public:
	PlayerEntity() {
		sheet.loadFromFile("char/standing2.png");
		sprites.setTexture(sheet);
		position.x = 0;
		position.y = 0;
		hp = 100;
		mp = 100;
		limit = 0;
		atk = 10;
		matk = 10;
		def = 10;
		mdef = 10;
		speed = 10;
		frontRow = true;
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

	void setSpriteStage(int value) {

	}

	void setStatus(std::string type, int value) {
		statuses[type] = value;
	}

	int getStatus(std::string type) {
		return statuses[type];
	}

	void setHP(int value) {
		hp = value;
	}

	int getHP() {
		return hp;
	}

	void setMP(int value) {
		mp = value;
	}

	int getMP() {
		return mp;
	}

	void setLimit(int value) {
		limit = value;
	}

	int getLimit() {
		return limit;
	}

	void setAtk(int value) {
		atk = value;
	}

	int getAtk() {
		return atk;
	}

	void setMatk(int value) {
		matk = value;
	}

	int getMatk() {
		return matk;
	}

	void setDef(int value) {
		def = value;
	}

	int getDef() {
		return def;
	}

	void setMdef(int value) {
		mdef = value;
	}

	int getMdef() {
		return mdef;
	}

	void setSpeed(int value) {
		speed = value;
	}

	int getSpeed() {
		return speed;
	}

	void setFrontRow(int value) {
		if (value == 1) {
			frontRow = true;
		}
		else {
			frontRow = false;
		}
	}

	bool getFrontRow() {
		return frontRow;
	}
};

class MobEntity : public Entity
{
public:
	MobEntity() {
		sheet.loadFromFile("sprites/mob0.png");
		sprites.setTexture(sheet);
		position.x = 0;
		position.y = 0;
		hp = 100;
		mp = 100;
		limit = 0;
		atk = 10;
		matk = 10;
		def = 10;
		mdef = 10;
		speed = 10;
		frontRow = true;
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

	void setSpriteStage(int value) {

	}

	void setStatus(std::string type, int value) {
		statuses[type] = value;
	}

	int getStatus(std::string type) {
		return statuses[type];
	}

	void setHP(int value) {
		hp = value;
	}

	int getHP() {
		return hp;
	}

	void setMP(int value) {
		mp = value;
	}

	int getMP() {
		return mp;
	}

	void setLimit(int value) {
		limit = value;
	}

	int getLimit() {
		return limit;
	}

	void setAtk(int value) {
		atk = value;
	}

	int getAtk() {
		return atk;
	}

	void setMatk(int value) {
		matk = value;
	}

	int getMatk() {
		return matk;
	}

	void setDef(int value) {
		def = value;
	}

	int getDef() {
		return def;
	}

	void setMdef(int value) {
		mdef = value;
	}

	int getMdef() {
		return mdef;
	}

	void setSpeed(int value) {
		speed = value;
	}

	int getSpeed() {
		return speed;
	}

	void setFrontRow(int value) {
		if (value == 1) {
			frontRow = true;
		}
		else {
			frontRow = false;
		}
	}

	bool getFrontRow() {
		return frontRow;
	}
};