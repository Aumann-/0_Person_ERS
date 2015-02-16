#include "player.h"

namespace ERS
{

player::player()
{
	morale = 50;
	fatigue = 0;
	speed = morale/5 - fatigue/10;
}

void player::setMorale(int m)
{
	morale += m;
}

void player::setFatigue()
{
	fatigue++;
}

int player::getMorale()
{
	return morale;
}
	
int player::getFatigue()
{
	return fatigue;
}

void player::addCard(card c)
{
	hand.push(c);
}

card player::getCard()
{
	card t = hand.top();

	hand.pop();
	
	return t;
}

int player::getSize()
{
	return hand.size();
}

void player:: calcSpeed()
{
	if (speed >= 1 && speed <= 25)
		speed = morale/5 - fatigue/10;
	else if (speed < 0)
		speed = 0;
	else if (speed > 25)
		speed = 25;
}

int player::getSpeed()
{
	return speed;
}

}