#include "hand.h"

namespace ERS
{
	//constructor to set th eplayer's hand to an empty vector
	hand::hand()
	{
		p_hand.clear();
	}

	//method that returns the top card from the player's hand
	card hand::play()
	{
		card t = p_hand[0]; //get the top card

		if (p_hand.size() > 1)
			p_hand.erase(p_hand.begin()); //remove the top card from the hand

		else
			p_hand.clear();

		return t; //return the card
	}

	//method to add a card to the player's hand
	void hand::won(std::string t)
	{
		card k;
		k.setSuitValue(t); //set the suit/value of a card to a temp card

		p_hand.push_back(k); //push the temp card to the player's hand
	}

	//method to return the size of a player's hand
	int hand::size()
	{
		return p_hand.size();
	}

}