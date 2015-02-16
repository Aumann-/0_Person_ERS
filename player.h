#ifndef PLAYER_H
#define PLAYER_H

#include <stack>
#include "card.h"

namespace ERS
{

class player
{
private:
	std::stack<card> hand;
	int morale;
	int fatigue;
	int speed;

public:
	player();

	void setMorale(int);
	void setFatigue();

	int getMorale();
	int getFatigue();

	void addCard(card);

	card getCard();

	int getSize();

	void calcSpeed();

	int getSpeed();
};

}
#endif