#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#pragma once

class Action
{
public:
	int currCounter;
	int finalCounter;

	Action() {}
	virtual ~Action() {}
	virtual void execute() = 0;
	virtual void setCounter(int value) = 0;
	virtual int getCounter() = 0;
	virtual int getFinalCounter() = 0;
};

class MobAttack : public Action
{
public:
	MobAttack() {
		currCounter = 0;
		finalCounter = 31;
	}

	void execute() {

	}

	void setCounter(int value) {
		currCounter = value;
	}

	int getCounter() {
		return currCounter;
	}

	int getFinalCounter() {
		return finalCounter;
	}
};