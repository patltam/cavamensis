#include "stdafx.h"
#include "IState.h"
#include <map>
#include <list>
#include <string>

class StateStack {
	std::map<std::string, IState*> mStates;
	std::list<IState*> mStack;

public:
	int update(float currFrame, sf::Vector2i mousePosition) {
		IState* top = mStack.back();
		return top->update(currFrame, mousePosition);
	}

	void resize(double mult) {
		mStates["mainmenu"]->resize(mult);
		mStates["localmap"]->resize(mult);
		mStates["menu"]->resize(mult);
		mStates["battle"]->resize(mult);
	}

	void click(sf::Vector2i mousePosition) {
		IState* top = mStack.back();
		top->click(mousePosition);
	}

	void render(sf::RenderWindow* window) {
		IState* top = mStack.back();
		top->render(window);
	}

	void add(std::string name, IState* state) {
		mStates.insert(std::pair<std::string, IState*>(name, state));
	}

	void remove(std::string name) {
		mStates.erase(name);
	}

	void push(std::string name) {
		IState* state = mStates[name];
		mStack.push_back(state);
	}

	IState* pop() {
		IState* top = mStack.back();
		mStack.pop_back();
		return top;
	}

	void onEnter() {
		IState* top = mStack.back();
		top->onEnter();
	}

	void onExit() {
		IState* top = mStack.back();
		top->onExit();
	}

	std::vector<MobEntity*> onExitToBattle() {
		IState* top = mStack.back();
		return top->onExitToBattle();
	}

	int getSize() {
		return mStack.size();
	}
};