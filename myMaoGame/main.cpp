// George Strauch
// mao game with help from github mao game and wikipedia
// 05/05/2018


/* rules of Mao according to wikipedia
 *
 * - players are each dealt 7 cards
 * - a card is puled out of the deck and players go around
 * matching the face or suite with previous card
 * - if an ace is put down, next player is skipped
 * - if a 7 is put down, next player must draw a penalty card, if he puts down another 7,
 * the player after must draw 2 and so on
 * - jacks are wild, causing the next player to play any card
 * - an 8 will reverse the order of play
 * - players play until their money is gone
 *
 * --MY RULE-- if a player plays an ace, that player chooses a random player to draw a card*/


#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
using namespace std;


// variables that need to be global
enum cardsuite { CLUBS, DIAMONDS, HEARTS, SPADES };
int glbIntNumOf7s = 0; // number of penalty cards to draw
bool glbBolNormalDirection = true; // true for normal, false if 8 is played and direction is reversed
int glbIntPlayerTurn = 0;   // refers to position in vector, not identifier


struct card{
    int value;
    cardsuite cardSuite;
};


struct player{
    int identifier; // to keep track of which player is which after the player vector changes
    vector<card> hand;
    int money;
    int currentBet;
};


/// globally known player is the ultimate winner
player ULTIMATE_PLAYER;


int getNumPlayers(){

    int intNumPlayers = 0;
    cout << "How many players? [2-6] " << endl;
    cin >> intNumPlayers;

    if(intNumPlayers < 2){
        cout << "2 Players..." << endl;
        intNumPlayers = 2;
    }
    else if(intNumPlayers > 6){
        cout << "6 players..." << endl;
        intNumPlayers = 6;
    }
    return intNumPlayers;
}


void printCard(card &crdTmp){

    string suite;
    switch (crdTmp.cardSuite){
        case CLUBS:
            suite = "Clubs";
            break;
        case HEARTS:
            suite = "Hearts";
            break;
        case DIAMONDS:
            suite = "Diamonds";
            break;
        case SPADES:
            suite = "Spades";
            break;
    }

    switch (crdTmp.value){
        case 1:
            cout << "Ace of " << suite << endl;
            break;
        case 11:
            cout << "Jack of " << suite << endl;
            break;
        case 12:
            cout << "Queen of " << suite << endl;
            break;
        case 13:
            cout << "King of " << suite << endl;
            break;
        default:
            cout << crdTmp.value << " of " << suite << endl;
    }
}


void printDeck(vector<card> &vtrDeck){
    for (int k = 0; k < vtrDeck.size(); ++k) {
        printCard(vtrDeck[k]);
    }
}



void printPlayer(player &player1){
    cout << "___\n";
    cout <<"ID: " << player1.identifier << endl;
    cout << "money: $" << player1.money << endl;
    cout << "current bet: $"  << player1.currentBet << endl;
    cout << " hand:\n";
    printDeck(player1.hand);
    cout << "___\n";

}


void shuffle(vector<card> &vtrCards){
    int random = 0;
    card crdTmp;
    for (int i = 0; i < vtrCards.size(); ++i) {
        random = (rand() % vtrCards.size());
        crdTmp = vtrCards[random];
        vtrCards.erase(vtrCards.begin() + random);
        vtrCards.push_back(crdTmp);
    }

}


void setCards(vector<card> &vtr, int intNumDecks){

    vtr.clear();
    card crdTmp;
    int intTmp;
    for (int i = 0; i < intNumDecks; ++i) {
        for (int j = 0; j < 52; ++j) {

            intTmp = (j / 13) +1;
            switch (intTmp){
                case 1:
                    crdTmp.cardSuite = CLUBS;
                    break;
                case 2:
                    crdTmp.cardSuite = DIAMONDS;
                    break;
                case 3:
                    crdTmp.cardSuite = HEARTS;
                    break;
                case 4:
                    crdTmp.cardSuite = SPADES;
            }
            crdTmp.value = (j%13) +1;

            vtr.push_back(crdTmp);
        }
    }// end last loop

    cout << "now shuffling" << endl;
    shuffle(vtr);   // shuffle 3 times to ensure it is fully shuffled
    shuffle(vtr);
    shuffle(vtr);
}


// making the players a vector because we don't have to include the size when passing into a function
void initPlayers(int intNumOfPlayers, vector<player> &vtrPlayers){
    for (int i = 0; i < intNumOfPlayers; ++i) {
        player tmpPlayer;
        tmpPlayer.identifier = i+1;
        tmpPlayer.money = 1000;
        vtrPlayers.push_back(tmpPlayer);
    }

}


void getBets(vector<player> &playerVec, int intnumplayers){

    int intBet;
    //to get human player input, commenting out to make process automated
    for (int i = 0; i < playerVec.size(); ++i) {
        cout << "\nplayer " << playerVec[i].identifier<< " what is your bet? money available is $" << playerVec[i].money;
        cin >> intBet;
        if(intBet > playerVec[i].money){
            cout << "nice try..." << endl;
            playerVec[i].currentBet = playerVec[i].money / intnumplayers;
            playerVec[i].money -= playerVec[i].currentBet;
        }
        else{
            playerVec[i].currentBet = intBet;
            playerVec[i].money -= playerVec[i].currentBet;
        }
    }

//    // automated bets for speed
//    for (int i = 0; i < playerVec.size(); ++i) {
//        intBet = (1000 / intnumplayers);
//        if(intBet > playerVec[i].money){
//            playerVec[i].currentBet = playerVec[i].money;
//            playerVec[i].money = 0;
//        }
//        else{
//            playerVec[i].currentBet = intBet;
//            playerVec[i].money -= playerVec[i].currentBet;
//        }
//    }
}


void dealCards(vector<player> &playerVec, vector<card> &cardPileVec, vector<card> &disCardVec){
    card tmpCard;

    for (int j = 0; j < 7; ++j) {   //controls the number of cards each player gets

        for (int i = 0; i < playerVec.size(); ++i) {    // gives a player a card from the card pile
            tmpCard = cardPileVec[cardPileVec.size() - 1];
            cardPileVec.pop_back();
            playerVec[i].hand.push_back(tmpCard);
        }

    }
    //adds the first card to the discard pile
    tmpCard = cardPileVec[cardPileVec.size() - 1];
    cardPileVec.pop_back();
    disCardVec.push_back(tmpCard);
}


bool winner(vector<player> &playerVec){
    bool boltmp = false;
    for (int i = 0; i < playerVec.size(); ++i) {
        if(playerVec[i].hand.empty()){      // does not kill loop if true in case multiple winners
            cout << "\nWINNER IS PLAYER " << playerVec[i].identifier << endl;
            playerVec[i].money += playerVec[i].currentBet*2;  //doubles his money if he wins
            ULTIMATE_PLAYER = playerVec[i]; //this is temporary and will be over written by all winners except the winner of the last game
            boltmp = true;
        }
    }
    return boltmp;
}


//need a way to increment the player in accordance with direction
void increment(int intNumOfPlayers){
    cout << "changing the player -- players: " << intNumOfPlayers << " direction: " << glbBolNormalDirection << " player that went last: " << glbIntPlayerTurn << endl;
    if(glbBolNormalDirection){
        glbIntPlayerTurn = (glbIntPlayerTurn + 1) % intNumOfPlayers;
    }
    else if(glbIntPlayerTurn == 0){
        glbIntPlayerTurn = intNumOfPlayers -1;
    }
    else{
        --glbIntPlayerTurn;
    }
}


// this will decide what the player needs to do.
// how many cards to draw, what card to play, which direction to go next, draws random player card if needed,
// changes global variables as needed
/// this function it the real meat and potatoes of the program, it decides all
void action(vector<player> &playerVec, vector<card> &cardPileVec, vector<card> &disCardVec){

    card tmpCard;
    bool bolTmp = false;

    for (int i = 0; i < glbIntNumOf7s; ++i) {
        tmpCard = cardPileVec[cardPileVec.size() - 1];
        cardPileVec.pop_back();
        playerVec[glbIntPlayerTurn].hand.push_back(tmpCard);
    }

    if(disCardVec[disCardVec.size() - 1].value == 11){      //if a joker was played, the next player may play any card
        tmpCard = playerVec[glbIntPlayerTurn].hand[0];
        playerVec[glbIntPlayerTurn].hand.erase(playerVec[glbIntPlayerTurn].hand.begin() + 0);
        disCardVec.push_back(tmpCard);
        bolTmp = true;
    }

    if(!bolTmp){

        //loop to find a card to deal
        for (int j = 0; j < playerVec[glbIntPlayerTurn].hand.size(); ++j) {

            if(playerVec[glbIntPlayerTurn].hand[j].value == disCardVec[disCardVec.size() - 1].value){       //if the value of the card is = to top of discard stack
                tmpCard = playerVec[glbIntPlayerTurn].hand[j];
                playerVec[glbIntPlayerTurn].hand.erase(playerVec[glbIntPlayerTurn].hand.begin() + j);
                disCardVec.push_back(tmpCard);
                bolTmp = true;
                break;

            } else if(playerVec[glbIntPlayerTurn].hand[j].cardSuite == disCardVec[disCardVec.size() - 1].cardSuite){   //if the suite of the card is = to top of discard stack
                tmpCard = playerVec[glbIntPlayerTurn].hand[j];
                playerVec[glbIntPlayerTurn].hand.erase(playerVec[glbIntPlayerTurn].hand.begin() + j);
                disCardVec.push_back(tmpCard);
                bolTmp = true;
                break;

            }
        }

    }

    if(!bolTmp){   //draw a card if couldn't put any cards down
        tmpCard = cardPileVec[cardPileVec.size() -1];
        cardPileVec.pop_back();
        playerVec[glbIntPlayerTurn].hand.push_back(tmpCard);
    }

    //tests if we need to reverse the order
    ///the following if statement happens if the player did place down a card
    if(bolTmp){

        cout << "cardPlayed: ";
        printCard(tmpCard);

        if(tmpCard.value == 8){     //reverse direction
            glbBolNormalDirection = !glbBolNormalDirection;
        }

        else if(tmpCard.value == 1){
            //my rule
            tmpCard = cardPileVec[cardPileVec.size() -1];
            cardPileVec.pop_back();
            playerVec[rand() % playerVec.size()].hand.push_back(tmpCard);    //////

            //game rule
            increment(playerVec.size());
        }

        else if(tmpCard.value == 7){    //7's rule
            ++glbIntNumOf7s;
        }

        else{
            glbIntNumOf7s = 0;
        }

    }

    increment(playerVec.size());

    if(winner(playerVec)){
        return;
    } else {
        action(playerVec, cardPileVec, disCardVec);    //recurse until there is a winner
    }

}


// removes broke players, clears hands, determines new card pile size, clears all cards
void cleanUp(vector<player> &playerVec, vector<card> &cardPileVec, vector<card> &disCardVec, int &intNumOfDeks){

    cardPileVec.clear();
    disCardVec.clear();
    glbIntPlayerTurn = 0;
    glbBolNormalDirection = true;

    for (int i = 0; i < playerVec.size(); ++i) {

        // clears hands and broke players
        if(playerVec[i].money == 0){
            cout << "removing player ID: " << playerVec[i].identifier << endl;
            playerVec.erase(playerVec.begin() + i);
        } else{
            playerVec[i].currentBet = 0;
            playerVec[i].hand.clear();
        }
    }

    intNumOfDeks = playerVec.size() / 2;    //changes the number of decks in play based on number of players left
}


void playMao(){

    srand(5);
    int intNumPlayers = getNumPlayers();
    int intNumDecks = intNumPlayers / 2;

    vector<player> vtrPlayers;
    vector<card> vtrCardPile;
    vector<card> vtrDisPile;

    cout << "Number of Players: " << intNumPlayers << endl
         << "Number of decks: " << intNumDecks << endl;

    initPlayers(intNumPlayers, vtrPlayers);

    while(vtrPlayers.size() > 1){       // the winner is the last player standing
        cout << "---- next round --- \n";

        setCards(vtrCardPile, intNumDecks);
        cout << "decks: " << intNumDecks << "\n deck: \n";
        printDeck(vtrCardPile);
        getBets(vtrPlayers, intNumPlayers);
        dealCards(vtrPlayers, vtrCardPile, vtrDisPile);
        action(vtrPlayers, vtrCardPile, vtrDisPile);
        cleanUp(vtrPlayers, vtrCardPile, vtrDisPile, intNumDecks);

        cout << "Number of players left: " << vtrPlayers.size() << endl;
        cout << "*****************************\n";
        cout << "Players left: \n";
        for (int i = 0; i < vtrPlayers.size(); ++i) {
            printPlayer(vtrPlayers[i]);
        }
        cout << "*****************************\n";
    }

    cout << "\n\nTHE GRAND WINNER IS: Player " << ULTIMATE_PLAYER.identifier << endl;
    cout << "Player " << ULTIMATE_PLAYER.identifier << " is left With: $" << ULTIMATE_PLAYER.money;

}


int main() {
    playMao();

    return 0;
}
