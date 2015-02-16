#include "card.h"

namespace ERS
{
	//constructor that sets the value of a card to a null value
	card::card()
	{
		value_suit = "NUL";
	}

	//method to read the value from  a card
	char card::readValue()
	{
		return value_suit[0];
	}

	//method to read the suit from a card
	char card::readSuit()
	{
		return value_suit[0];
	}

	//method to set the suit of a card
	void card::setSuit(char s)
	{
		value_suit[2] = s;
		value_suit[1] = '-';
	}

	//method to set the value of a card
	void card::setValue(char v)
	{
		value_suit[0] = v;
	}

	//method to set the entire card suit/value string
	void card::setSuitValue(std::string sv)
	{
		value_suit = sv;
	}

	//method to retrun the entire suit/value string
	std::string card::readCard()
	{
		return value_suit;
	}

}//end namespace