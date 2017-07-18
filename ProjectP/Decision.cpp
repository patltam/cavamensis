#include "stdafx.h"
#include "Entity.h"
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class Decision {
public:
	Decision() {

	}

	~Decision() {

	}

	Action* getAction(Entity* caster, std::vector<PlayerEntity*> playerList, std::vector<MobEntity*> mobList) {
		return caster->getAction()[0];
	}
};