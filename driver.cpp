/*
ERS card game for two players. Uses an array of card object inside a deck object.
Each player has a stack of cards.

Current Status:
	Compiles: Y
	Runs: Y
Features:
	Game currently plays until either player is out of cards.
	Game will properly check how many cards a player should play.
	Game does add any won cards to player's hands
	Game checks for doubles and sandwiches, players will always attempt to slap.

Completion Status:
	FINISHED

Bugs:
	Program sometimes crashes with vector subscript out of range error. *Fixed*
		-Changed from object with a vector to using a stack

	Players do not always play correct amount of cards. *Fixed*
		-Rewrote calculation for amount of cards to play

	Remaining player continues to play if one player runs out of cards *Fixed*
		-Created gameOver function that is called as soon as a player runs out of cards

	Program was looping infinitely *Fixed*
		-Due to NUL cards being added to player hand

	Program was not adding correct amount of cards to player's hand *Fixed*
		-Due to looping based on stack.size(), which changed as loop progressed. Sotring size before looped fixed issue

	Program was crashing when attempting to check if doubles were played *Fixed*
		-Due to not checking if at least two cards were played, causing the read to go out of bounds

To-Do:
	Needed:
		Add stack for all cards played *Done*
		Add played stack to winner's hands when appropriate *Done*
		Move contents of play loop to a function *Done*
	Extra:
		Need to change each player into a player object, containing a stack of cards, a morale value, and a fatigue value *Done*


		1. Add player speed stats (needed for later features) *Done*
			Speed stat will determine how quickly a player can slap.
			Speed will change based on morale (how often they win) and fatigue (how long the game has been going).
			Speed will start at the same value or both players.
			Slap chance will be a random roll using the differences in speed as an offset (player with higher speed get the difference added to their roll).
		2. Add slapping for doubles (based on speed stat) *Done*
		3. Add slapping of sandwiches (based on speed stat) *Done*


*/



#include <iostream>
#include <ctime>
#include <stack>
#include "deck.h"
#include "player.h"

#define count(a,b) for(int i = a; i < b; i++)


using namespace std;
using namespace ERS;

//function to deal cards to each player for game start
//void deal(deck &main_deck, stack<card> &p1, stack<card> &p2);
void deal(deck &main_deck, player &p1, player &p2);

//debug function to print all cards in deck, p1 hand, and p2 hand
//void deal_all(deck &main_deck, stack<card> &p1, stack<card> &p2);
void deal_all(deck &main_deck, player &p1, player &p2);

//function to check the played card to determine if it is a face card and how many cards need to be played
bool checkCard(card &c);

//function for player to take their turn
//void player_turn(stack<card> &p, bool &face, int &amount_plays, bool &winner, stack<card> &played);
void player_turn(player &p, bool &face, int &amount_plays, bool &winner, stack<card> &played);

//function to play top card from player's hand *Deprecated*
//card play(stack<card> &p);

//function to determine how many cards the next player should play if the previous card was a face card
int plays(card &c);

//function to display game over message
void gameOver();

//function to give all played cards to the winner
void giveWon(player &p, stack<card> &w);

//function to check if last two cards played are doubles (same value)
bool checkDouble(stack<card> &played);

//function to check if last three cards were a sandwich
bool checkSandwich(stack<card> &played);

//function to determine which player wins the slap
int Slap(player &p1, player &p2);

//value for the current turn
int turn = 1;

//holds how many turns were played
int p1Plays = 0, p2Plays = 0;

//holds how many doubles, how many sandwiches
int doubles = 0, sandwiches = 0;

//debug to hold which player won slap
int p1Slap = 0, p2Slap = 0;

int main()
{
	//creates the main deck, initializing it with 52 cards in order.
	deck main_deck;
	
	//creates a stack for each player's hand
	//stack<card> p1;
	//stack<card> p2;

	//creates player objects
	player p1, p2;

	//creates a stack to act as the pile of played cards
	stack<card> played;

	//holds each card that is play to be passed to checkCard
	card temp;

	//holds the value returned from checkCard for next player
	bool face = false;

	//holds the value of whether to give played stack to previous player
	bool winner = false;

	//variable to hold the number of cards the next player must play
	int amount_plays = 1;

	//holds check for if cards are doubles
	bool dbls = false;

	//holds check for sandwich
	bool sand = false;

	//holds results from slap function
	int slapWin = 0;

	//shuffles the main deck using a random swap alorithm
	main_deck.shuffle();

	//deals out all cards in the main deck to each player.
	deal(main_deck, p1, p2);

	//deal_all(main_deck, p1, p2);

	//loop to play until either player runs out of cards *needs moved to function*
	while (p1.getSize() > 0 || p2.getSize() > 0)
	{
		if (turn == 1) //if it is player 1 turn
		{
			player_turn(p1, face, amount_plays, winner, played);

			//check for doubles only if at least two cards are played
			if (played.size() > 1)
				dbls = checkDouble(played);
			else
				dbls = false;

			//check for sandwiches only if at least three cards are played
			if (played.size() > 2)
				sand = checkSandwich(played);
			else
				sand = false;

			p1Plays++;
			p1.setFatigue();

			//if cards are doubles
			if (dbls)
			{
				winner = false; //set winner to false to avoid later check

				slapWin = Slap(p1, p2); //check which player wins the slap

				//if p1 wins, give p1 the pot
				if (slapWin == 1)
				{
					cout << "P1 Won the Doubles" << endl;
					giveWon(p1, played);
					
					//increase p1 morale, decrease p2
					p1.setMorale(5);
					p2.setMorale(-5);
				}
				//if p2 wins, give p2 the pot
				else if (slapWin == 2)
				{
					cout << "P2 Won the Doubles" << endl;
					giveWon(p2, played);

					//decrease p1 morale, increase p2
					p1.setMorale(-5);
					p2.setMorale(5);
				}
				//if a tie
				else
				{
					cout << "Tie on Doubles" << endl;
				}
				//cin.ignore();
			}
			//f not doubles, check for sandwich
			else if (sand)
			{
				winner = false;

				slapWin = Slap(p1, p2);

				//if p1 wins, give p1 the pot
				if (slapWin == 1)
				{
					cout << "P1 Won the Doubles" << endl;
					giveWon(p1, played);

					//increase p1 morale, decrease p2
					p1.setMorale(5);
					p2.setMorale(-5);
				}
				//if p2 wins, give p2 the pot
				else if (slapWin == 2)
				{
					cout << "P2 Won the Doubles" << endl;
					giveWon(p2, played);

					//decrease p1 morale, increase p2
					p1.setMorale(-5);
					p2.setMorale(5);
				}
				//if a tie
				else
				{
					cout << "Tie on Doubles" << endl;
				}
				//cin.ignore();
			}

			//if p2 played a face && p1 did not, p2 gets all played cards
			if (winner)
			{
				giveWon(p2, played);
				cout << "P2 Wins the Pot" << endl;

				p1.setMorale(-3);

				p2.setMorale(3);	
			}

			p1.calcSpeed();

			p2.calcSpeed();

			//reset winner flag
			winner = false;
			
			//prints cards remaining for both players
			cout << endl << "P1 Cards Remaining: " << p1.getSize() << endl;
			cout << "P1 Speed: " << p1.getSpeed() << endl;
			cout << "P2 Cards Remaining: " << p2.getSize() << endl;
			cout << "P2 Speed: " << p2.getSpeed() << endl;

			//cin.ignore();
		}
		else //if it is player 2 turn
		{
			player_turn(p2, face, amount_plays, winner, played);

			p2Plays++;
			p2.setFatigue();

			//check for doubles only if at least two cards are played
			if (played.size() > 1)
				dbls = checkDouble(played);
			else
				dbls = false;

			//check for sandwiches only if at least three cards are played
			if (played.size() > 2)
				sand = checkSandwich(played);
			else
				sand = false;

			//if doubles
			if (dbls)
			{
				winner = false; //set winner to false to avod later check

				slapWin = Slap(p1, p2); //check which player wins

				//if p1 wins, give p1 the pot
				if (slapWin == 1)
				{
					cout << "P1 Won the Doubles" << endl;
					giveWon(p1, played);

					//increase p1 morale, decrease p2
					p1.setMorale(5);
					p2.setMorale(-5);
				}
				//if p2 wins, give p2 the pot
				else if (slapWin == 2)
				{
					cout << "P2 Won the Doubles" << endl;
					giveWon(p2, played);

					//decrease p1 morale, increase p2
					p1.setMorale(-5);
					p2.setMorale(5);
				}
				//if a tie
				else
				{
					cout << "Tie on Doubles" << endl;
				}
				//cin.ignore();
			}
			//if not doubles, check for sandwich
			else if (sand)
			{
				winner = false;

				slapWin = Slap(p1, p2);

				//if p1 wins, give p1 the pot
				if (slapWin == 1)
				{
					cout << "P1 Won the Doubles" << endl;
					giveWon(p1, played);

					//increase p1 morale, decrease p2
					p1.setMorale(5);
					p2.setMorale(-5);
				}
				//if p2 wins, give p2 the pot
				else if (slapWin == 2)
				{
					cout << "P2 Won the Doubles" << endl;
					giveWon(p2, played);

					//decrease p1 morale, increase p2
					p1.setMorale(-5);
					p2.setMorale(5);
				}
				//if a tie
				else
				{
					cout << "Tie on Doubles" << endl;
				}
				//cin.ignore();
			}

			//if p1 played a face && p2 did not, p1 gets all played cards
			if (winner)
			{
				giveWon(p1,played);
				cout << "P1 Wins the Pot" << endl;

				p1.setMorale(3);

				p2.setMorale(-3);
			}

			p1.calcSpeed();

			p2.calcSpeed();

			//reset winner flag
			winner = false;

			//prints cards remaing for both players
			cout << endl << "P1 Cards Remaining: " << p1.getSize() << endl;
			cout << "P1 Speed: " << p1.getSpeed() << endl;
			cout << "P2 Cards Remaining: " << p2.getSize() << endl;
			cout << "P2 Speed: " << p2.getSpeed() << endl;

			//cin.ignore();
		}

		
	}//end while 

	//game is over
	gameOver();

	cin.ignore();
	return 0;
}


//function takes the top card from the main deck and adds it to each players hand in turn, starting with p1
void deal(deck &main, player &p1, player &p2)
{
	for (int i = 0; i < 26; i++)
	{
		p1.addCard(main.draw()); //deal card to p1
		p2.addCard(main.draw()); //deal card to p2
	}
}

/*//Code to test the contents of the deck and each players hand after dealing out all cards
void deal_all(deck &main, stack<card> &p1, stack<card> &p2)
{
	//deal all cards in main deck
	cout << "Main Deck" << endl << endl;
	count(0,52)
		cout << main.draw().readCard() << endl;

	cout << endl;

	//deal all cards in player 1 hand
	cout << "P1:" << p1.size() << endl << endl;
	count(0, 26)
		cout << play(p1).readCard() << endl;

	cout << endl;

	//deal all cards in player 2 hand
	cout << "P2:" << p2.size() << endl << endl;
	count(0, 26)
		cout << play(p2).readCard() << endl;

	cout << endl;
}*/

//function to check if card is a face card
bool checkCard(card &t)
{
	if (t.readValue() == 'A') //if an Ace
		return true;
	else if (t.readValue() == 'K') //if a King
		return true;
	else if (t.readValue() == 'Q') //if a Queen
		return true;
	else if (t.readValue() == 'J') //if a Jack
		return true;
	else //not a face card
		return false;
}

//function to determine how many cards the next player must play
int plays(card &t)
{
	if (t.readValue() == 'A') //if an Ace, worth 4 cards
		return 4;
	else if (t.readValue() == 'K') //if a King, worth 3 cards
		return 3;
	else if (t.readValue() == 'Q') //if a Queen, worth 2 cards
		return 2;
	else if (t.readValue() == 'J') //if a Jack, worth 1 card
		return 1;
	else //not a face, worth 1 card
		return 1;
}

/*custom pop function to return the top card of the player's hand and remove it from the stack *Deprecated*
card play(stack<card> &p)
{
	card t = p.top(); //store top card in hand

	p.pop(); //remove top card from hand

	return t; //return played card
}
*/

//game ends
void gameOver()
{
	cout << "Game Over " << turn << endl << endl;;
	cout << "P1 played " << p1Plays << " times." << endl;
	cout << "P2 played " << p2Plays << " times." << endl;
	cout << "Total: " << p1Plays+p2Plays << endl;
	cout << "Doubles: " << doubles << endl;
	cout << "Sandwiches: " << sandwiches << endl;
	cout << endl << "P1 Wins: " << p1Slap << endl;
	cout << "P2 Wins: " << p2Slap << endl;
	cin.ignore();
	exit(0);
}

//function for each player to play the top card of their hand
void player_turn(player &p, bool &face, int &amount_plays, bool &winner, stack<card> &played)
{
		//if previous player's card was a face card, set winner flag to true
		if (face)
		{
			winner = true;
		}

		face = false; //set face card flag to false

		//debug to check how many cards should be played
		cout << amount_plays << endl;

		//hold played card
		card temp;

		//if player has insufficient cards to play, play number in hand
		if (p.getSize() < amount_plays)
		{
			amount_plays = p.getSize();
		}

		//loop based on how many should be played
		for (int i = 0; i < amount_plays; i++)
		{
			temp = p.getCard(); //play a card

			cout << "P" << turn << ": " << temp.readCard() << endl; //print played card to screen

			played.push(temp); //push played card onto played stack

			face = checkCard(temp); //check if played card is a face card

			//if card was a face card
			if (face)
			{
				amount_plays = plays(temp); //set next player's amount to play based on previous face card
				winner = false; //set winner flag to false
				break;
			}		
		}

		//if card was not a face card, set amount to play to 1
		if (!face)
		{
			amount_plays = 1;
		}

		//if player runs out of cards, game is over
		if (p.getSize() == 0)
		{
			cout << "P" << turn << " ran out of cards" << endl; 
			gameOver();
		}

		//switch to next player's turn
		if (turn == 1)
			turn = 2;
		else
			turn = 1;

		
}

//function that takes the stack of played cards and gives it to the player who won them
void giveWon(player &p, stack<card> &w)
{
	card t;
	int pSize = p.getSize(); //sotre the current size of the player's hand
	int wSize = w.size(); //store the current size of the played cards stack
	card *ptr = new card[pSize]; //create new array to hold player's hand

	//store all of player's hand into temp array
	for (int i = 0; i < pSize; i++)
	{
		t = p.getCard();
		ptr[i] = t;
	}

	//move all of played cards into player's hand
	for (int i = 0; i < wSize; i++)
	{
		t = w.top();
		w.pop();
		p.addCard(t);
	}

	//move contents of temp array (previous contents of hand) back into player's hand
	for (int i = 0; i < pSize; i++)
	{
		p.addCard(ptr[i]);
	}

	//delete temp array
	delete [] ptr;
}

//function that checks if last two cards played were doubles
bool checkDouble(stack<card> &played)
{
	bool d = false; //create flag

	card t = played.top(); //get top card

	played.pop(); //remove top card

	card s = played.top(); //get new top card

	//if original top card and new top card have the same value, they are doubles
	if (t.readValue() == s.readValue())
	{
		d = true;
		doubles++;
	}

	//put original top card back on stack
	played.push(t);

	//return flag
	return d;
}


bool checkSandwich(stack<card> &played)
{
	bool s = false; //create flag

	card a = played.top(); //get top card

	played.pop(); //remove top card

	card b = played.top(); //get new top card (the filling)

	played.pop(); //remove filling

	card c = played.top(); //get bottom of sandwich

	if (a.readValue() == c.readValue()) //if original top card and bottom card are the same, it is a sandwich
	{
		s = true;
		sandwiches++;
	}

	//put filling and top back on stack
	played.push(b);
	played.push(a);

	//return flag
	return s;
}


int Slap(player &p1, player&p2)
{
	//get random roll for each player, adding their current speed as a handicap
	int p1Roll = (rand() % 100) + p1.getSpeed();

	rand(); rand(); rand(); //this is used to create a buffer between the rolls, withoutthis P2 wins 8-9 times out of 10. With this, wins are about even

	int p2Roll = (rand() % 100) + p2.getSpeed();

	//cout << p1Roll << " " << p2Roll << endl;
	//cin.ignore();

	//if p1 rolled higher
	if (p1Roll > p2Roll)
	{
		p1Slap++;
		return 1;
	}
	//if p2 rolled higher
	else if (p2Roll > p1Roll)
	{
		p2Slap++;
		return 2;
	}
	//if a tie
	else
		return 0;
}