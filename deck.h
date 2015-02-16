#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <string>


namespace ERS
{
	class deck
	{
	private:
		card *myCards;
		int deck_counter; //counter used to initialize deck
	
	
	public:
		deck();
		card draw();
		card draw(int);
		void shuffle();


	};//end deck class



}//end namespace



#endif