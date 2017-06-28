#include "stdafx.h"
#include "Entity.h"
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#pragma once

class Action
{
public:
	PlayerEntity* playerEnt1;
	PlayerEntity* playerEnt2;
	PlayerEntity* playerEnt3;
	PlayerEntity* playerEnt4;
	MobEntity* mobEnt1;
	MobEntity* mobEnt2;
	MobEntity* mobEnt3;
	MobEntity* mobEnt4;
	MobEntity* mobEnt5;

	Action() {}
	virtual ~Action() {}
	virtual void execute() = 0;
};

class PlayerAttack : public Action
{
public:
	PlayerAttack() {

	}

	void execute() {

	}
};