This is a 0 person version of the ERS card game written in C++. 

The game functions by creating a deck of cards which is divided among each "player". The "players" then take turns playing a card until certain events happen.

1. Face card is played: If a face card is played, the other player must play cards each to the value of the played face card (Ace = 4, King = 3, Queen = 2, Jack = 1). IF the player also plays a face card, it switch the the other player and continues until a face card is not played. If a player is unable to play a face card in this situation, the player to place the previous face card will get the pile.

2. Doubles are played (two card with the same number value):

3. Sandwich (two cards with the same number value with a different card inbetween):

In either of these two events, the "players" will attempt to "slap" the cards, first first to "slap" gets the pile. Slapping is done using a random numerical value, highest value winning. This outcome is influenced by each player's speed statistic, adding their speed as a modifier (ex. P1 rolls 50 with speed of 5, roll totals 55). Speed is determined by two factors, morale and fatigue. Morale changes based on whether the player wins or loses the pile and can shift up or down. Fatigue increases each round of play and can only increase.

The game is over once a single player has all the cards.