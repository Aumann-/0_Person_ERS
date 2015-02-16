#ifndef HAND_H
#define HAND_H
#include "card.h"
#include <vector>


namespace ERS
{
	class hand
	{
	private:
		std::vector<card> p_hand;


	public:
		hand();
		card play();
		void won(std::string);
		int size();
	};


}






#endif