#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#ifndef NUMBER_OF_CARDS_FOR_PLAYER
	#define NUMBER_OF_CARDS_FOR_PLAYER 3
#endif
#ifndef MAX_COINS_FOR_PLAYER
	#define MAX_COINS_FOR_PLAYER 10
#endif
#ifndef MIN_CARD_FACE
	#define MIN_CARD_FACE 7
#endif
/* global variables*/

const char* PLAYER_1 = "Computer"; 
const char* ANONYMOUS = "Anonymous"; 

/*Structure card (self refrential)*/
typedef struct /*__attribute__((packed))*/ Card  {
	 char suit; 
	 int face; 
	 struct Card* next; 
}card; 

/*structure deck*/
typedef struct /*__attribute__((packed))*/{
    card* first;
}deck;

/*structure player*/
typedef struct /*__attribute__((packed))*/
{
	char name[20+1];
	card cards[NUMBER_OF_CARDS_FOR_PLAYER];
	/*char type;*/
	int bet;

}player;

/*function proto type*/
void display_cards(deck);
void distribute_cards_to_player(player*, deck*);
void print_cards_for_player(player);
void remove_card(card, deck*);
int is_in_deck(deck, char, int);
card* get_random_card(deck*);
void swap_card(card*, card*);
int min_card(card*);
int get_number_of_cards_in_deck(deck);
int get_user_choice(char*, char*);
void bet(player*, int, int);
int total_face(player*);
void display_winner(player*, player*);
int get_card_to_swap(player);
void greetings();
void delay(unsigned long);
void loading(unsigned int);
void shuffle(deck*);
int main(int argc, char *argv[])
{
	int i=0, index_of_card_to_swap=-1;
	char quit;
	player player1, player2;
	
	deck deck1={NULL};

	card cards[]=
		{
			{3,1,NULL},{3,2,NULL},{3,3,NULL},{3,4,NULL},{3,5,NULL},{3,6,NULL},{3,7,NULL},{3,8,NULL},{3,9,NULL},{3,10,NULL},{3,11,NULL},{3,12,NULL},{3,13,NULL},
			{4,1,NULL},{4,2,NULL},{4,3,NULL},{4,4,NULL},{4,5,NULL},{4,6,NULL},{4,7,NULL},{4,8,NULL},{4,9,NULL},{4,10,NULL},{4,11,NULL},{4,12,NULL},{4,13,NULL},
			{5,1,NULL},{5,2,NULL},{5,3,NULL},{5,4,NULL},{5,5,NULL},{5,6,NULL},{5,7,NULL},{5,8,NULL},{5,9,NULL},{5,10,NULL},{5,11,NULL},{5,12,NULL},{5,13,NULL},
			{6,1,NULL},{6,2,NULL},{6,3,NULL},{6,4,NULL},{6,5,NULL},{6,6,NULL},{6,7,NULL},{6,8,NULL},{6,9,NULL},{6,10,NULL},{6,11,NULL},{6,12,NULL},{6,13,NULL}

		};

	card* ptr=NULL;
	
	
	/*greetings*/
	greetings();
	printf("\nInitializing deck of cards ");
	loading(1);
	/*creating the deck begin*/
	for (i=51;i>=0;i--)
	{
		if(ptr!=NULL){
			cards[i].next=ptr;
		}
		ptr=&cards[i];
	}
	deck1.first = &cards[0];
	/*creating the deck end*/
	shuffle(&deck1);
	/*display the deck*/
	display_cards(deck1);
	
	
	/*initializing players begin*/
	strcpy(player1.name,PLAYER_1);
	/*player1.type = 'P';*/
	printf("The first player in this game is the '%s'\n", player1.name);
	
	strcpy(player2.name,(argc>1?argv[1]:ANONYMOUS));
	/*player2.type = 'S';*/
	printf("The second player in this game is the '%s'\n", player2.name);
	/*initializing players end*/
	
	/*distributing cards to players begin*/
	srand(time(NULL));	
	
	printf("\nDistributing cards to players ");
	loading(2);
	/*printf("Let's distribute 3 random cards to the %s\n",player1.name);*/
	distribute_cards_to_player(&player1, &deck1);
	/*printf("%s's cards are not to be shown to %s\n",player1.name, player2.name);*/
	
	/*printf("\nLet's distribute 3 random cards to %s\n",player2.name);*/
	distribute_cards_to_player(&player2,&deck1);
	print_cards_for_player(player2);
	/*distributing cards to players end*/

	
	/*does the player want to fold?*/
	if (get_user_choice("\n%s, would you like to fold (y = yes)? ", player2.name))
	{
		printf("\n%s wins!\nBetter luck next time\n",player1.name);
		return 0;
	}
	
	/*player 1 rasies bet begin*/
	bet(&player1, 1, MAX_COINS_FOR_PLAYER);/* lowest and highest*/
	printf("%s placed a bet of %d coins\n",player1.name, player1.bet );
	/*player 1 rasies bet end*/
	
	/*player 2 matches or raises begin*/
	bet(&player2, player1.bet, MAX_COINS_FOR_PLAYER);/* lowest and highest*/
	printf("%s placed a bet of %d coins\n",player2.name, player2.bet);
	
	
	printf("%s decided to %s the bet\n",player2.name, (player2.bet == player1.bet)?"match":"raise" );
	
	/*player 2 matches or raises end*/

	/*swap for player 1 based on a rule*/
	index_of_card_to_swap=get_card_to_swap(player1);
	if(index_of_card_to_swap>-1){
		printf("\nSwapping cards ");
		loading(2);
		swap_card(&(player1.cards[index_of_card_to_swap]), get_random_card(&deck1));
		printf("\n%s swapped a card!\n",  player1.name );
		/*printf("The swapped card is [%c %d]\n",player1.cards[min_index].suit,player1.cards[min_index].face);*/
	}
	
	/*swap for player 2 based on choice*/
	index_of_card_to_swap=get_card_to_swap(player2);
	if(index_of_card_to_swap>-1){
		printf("\nSwapping cards ");
		loading(2);
		printf("Before swapping\n");
		print_cards_for_player(player2);
		/*printf("The card that will be swapped is [%c %d]\n",player2.cards[index_of_card_to_swap].suit,player2.cards[index_of_card_to_swap].face);*/
		

		swap_card(&(player2.cards[index_of_card_to_swap]), get_random_card(&deck1));

		/*printf("The swapped card is [%c %d]\n",player2.cards[index_of_card_to_swap].suit,player2.cards[index_of_card_to_swap].face);*/
		printf("After swapping\n");
		print_cards_for_player(player2);
	}

	/*player 1 rasies bet begin*/
	bet(&player1, player2.bet, MAX_COINS_FOR_PLAYER);/* lowest and highest*/
	printf("%s placed a bet of %d coins\n",player1.name, player1.bet );
	
	/*player 2 matches or raises begin*/
	bet(&player2, player1.bet, MAX_COINS_FOR_PLAYER);/* lowest and highest*/
	printf("%s placed a bet of %d coins\n",player2.name, player2.bet );
	
	display_winner(&player1, &player2);
	printf("\n");
	display_cards(deck1);
	do{
		printf("\nEnter [Q or q] to quit ");
		scanf(" %c", &quit);
	}while(toupper(quit)!='Q');
	
	return 0;
}

int is_in_deck(deck d, char suit, int face){
	card* ptr=d.first;
	while(ptr!=NULL){
		
		if(ptr->suit==suit &&
			ptr->face==face){
			return 1;
		}
		ptr=ptr->next;
	}
	return 0;
}
void display_cards(deck d){
	int counter=0;
	card* ptr=d.first;
	printf("\nDeck now has %d cards:\n\n",get_number_of_cards_in_deck(d));
	while(ptr!=NULL){
		counter++;
		printf("[%c%s%d]%c", ptr->suit, (ptr->face>9?" ":"  "), ptr->face,(counter%13==0?'\n':' '));
		ptr=ptr->next;
	}
	printf("\n");
}
void remove_card(card c, deck* d){
	card *ptr=d->first, *prev=0;
	int counter=0;
	while(1){
		if (ptr->face==c.face && 
				ptr->suit==c.suit)
		{
			/*printf("Removing [%c %d] from the deck\n",ptr->suit, ptr->face);*/
			/*when it is the first in the link*/
			
			if(counter==0){
				/*printf("Updating deck to point to '%p' (address of card [%c %d])\n",ptr->next, ptr->next->suit, ptr->next->face);*/
				d->first=ptr->next;
			}
			else{
				/*printf("Updating card [%c %d] to point to [%c %d])\n",prev->suit, prev->face, ptr->next->suit, ptr->next->face);*/
				prev->next = ptr->next;
			}
			ptr->next = NULL;
			break;
		}
		prev=ptr;
		ptr=ptr->next;
		counter++;
	}
}
void distribute_cards_to_player(player* p, deck* d){
	int i=0;
	card *ptr=NULL;
	/*distribute 3 cards to the player*/
	for (i=0;i<NUMBER_OF_CARDS_FOR_PLAYER;i++)
	{
		
		ptr=get_random_card(d);
		p->cards[i].suit = ptr->suit;
		p->cards[i].face = ptr->face;
		/*printf("[%c,%d]\n",p->cards[i].suit,p->cards[i].face);*/
		remove_card(p->cards[i],d);
	}
	
	return;
}
void print_cards_for_player(player p){
	int i=0;
	printf("\n%s has the following cards\n",p.name);
	for (i=0;i<NUMBER_OF_CARDS_FOR_PLAYER;i++){
		printf("Card %d: [%c %d]\n",i+1, p.cards[i].suit,p.cards[i].face);
	}
}
void swap_card(card* c1, card* c2){
	int  face;
	char suit;
	if(c1!=NULL &&
		c2!=NULL){
		
		face=c1->face;
		suit=c1->suit;

		c1->face=c2->face;
		c1->suit=c2->suit;

		c2->face=face;
		c2->suit=suit;
	}
}
card* get_random_card(deck* d){
	int random_selection=0,j=0 ;
	card *ptr=NULL;
	random_selection=rand() % get_number_of_cards_in_deck(*d);
	ptr=d->first;
	do{
		ptr=ptr->next;
		j++;
	}while(j<random_selection);
	
	return ptr;
}
int get_number_of_cards_in_deck(deck deck1){
	card *ptr=deck1.first;
	int length=0;
	while(ptr!=NULL){
		ptr=ptr->next;
		length++;
	}
	return length;
}
int min_card(card* c)
{
	int min=0,i=1;
	if (c!=NULL)
	{
		min=c->face;
		c++;
		while (i<NUMBER_OF_CARDS_FOR_PLAYER )
		{
			if (c->face<min)
			{
				min=c->face;
				break;
			}
			i++;
			c++;
		}
	}
	/*only return a valid index if the face value of the card is less than the MIN_CARD_FACE*/
	return min<MIN_CARD_FACE?i:-1;
}
void bet(player* player, int lowest, int highest){
	 int bet=0;
	 if(lowest<highest){
		/*printf("\nPlayer:name: %s\n", player->name);*/
		/*primary placing bet*/
		if(strcmp(player->name,PLAYER_1)==0){
			printf("\nPlacing a random bet for %s between %d - %d\n", player->name, lowest, highest);
			/*
			Here's an example of how this works!!!
			lowest=0, highest=10 
			10 - 0 = 10
			rand()/%10 + 1 => [1 - 10]
			[1 - 10] + lowest
			
			*/
			--lowest; 
			bet = lowest + (rand()%(highest-lowest)) + 1;
		}
		else {
			printf("\n");
			do{
				bet = INT_MIN;
				fprintf(stdout, "%s, please choose a bet between %d - %d ",player->name, lowest, highest);
				fscanf(stdin, "%d", &bet);
				if(bet==INT_MIN)
				{
					//someone did not input correct type
					bet = lowest;
				}
				
			}while(bet<lowest || bet>highest);
		}
	 }
	 else{
		bet = highest;
		/*printf("There is no room for placing bet now!\n");*/
		printf("\n");
	 }
	 player->bet=bet;
	 return ;
}
int get_user_choice(char *s1, char *s2){

	char c;
	printf(s1,s2);
	scanf(" %c",&c);
	return toupper(c) == 'Y'; /*1 or 0*/
	
}
int get_card_to_swap(player p){
	int index=0;
	if(strcmp(p.name,PLAYER_1)==0){
		index=min_card(p.cards);
	}
	else{
		if(get_user_choice("\n%s, do want to swap (y = yes)? ", p.name))
		{
			start_over:
			index=INT_MIN;
			printf("Enter the card number that you want to swap (1, 2, 3): ");
			scanf("%d",&index);
			
			if(index==INT_MIN){
				index=-1;
			}
			else{
				if (index <1 || index > NUMBER_OF_CARDS_FOR_PLAYER ){
					goto start_over;
				}
				index--;/*decrement to use it as an index*/
			}
			
		}
		else{
			index=-1;
		}
	}
	return index;
}
int total_face(player* p)
{
	int i=0, total=0;
	for (i=0;i<NUMBER_OF_CARDS_FOR_PLAYER;i++)
	{
		total+=p->cards[i].face;
		
	}
	return total;
}
void display_winner(player* p1, player* p2){

	int total1=total_face(p1);
	int total2=total_face(p2);
	
	printf("\n");
	print_cards_for_player(*p1);
	printf("The total for %s is %d\n",p1->name, total1);
	printf("%s placed a bet for %d coins\n\n",p1->name, p1->bet);
	
	print_cards_for_player(*p2);
	printf("The total for %s is %d\n",p2->name, total2);
	printf("%s placed a bet for %d coins\n\n",p2->name, p2->bet);
	
	
	if(total1>total2){
		printf("\n%s is the winner!",p1->name);
		printf("\n%s gets %d coins\n", p1->name, (p1->bet+p2->bet));
		
	}
	else if(total1<total2){
		printf("\n%s is the winner!",p2->name);
		printf("\n%s gets %d coins\n", p2->name, (p1->bet+p2->bet));
	}	
	else
	{
		printf("\nThere is a tie between %s and %s!",p1->name, p2->name);
	}
	return;
}
void greetings()
{
	
	char suits[] = {3,4,5,6};
	printf("\n\n"); 
	printf("%c %c %c %c POKER %c %c %c %c\n", suits[0],suits[1],suits[2],suits[3],suits[0],suits[1],suits[2],suits[3]);
	printf("%c                   %c\n", suits[0],suits[3]);
	/*printf("%c   NIKHIL DHANJI    %c\n", suits[rand()%4],suits[rand()%4]);*/
	printf("%c                   %c\n", suits[0],suits[3]);
	printf("%c %c %c %c POKER %c %c %c %c\n", suits[0],suits[1],suits[2],suits[3],suits[0],suits[1],suits[2],suits[3]);
	printf("\n\n");
	return;
}

void delay(unsigned long milliseconds)
{

    clock_t now,then;
    now = then = clock();
    while( (now-then) < milliseconds ){
        now = clock();
	}
}
void loading(unsigned int t){
	int i=0;
	for(;t>0;t--){
		for (i=0;i<3;i++)
		{
			 printf(". ");
			 fflush(stdout);
			 delay(500L);
		}
		printf("\b\b\b\b\b\b");
		fflush(stdout);
		delay(100L);
	}

}
void shuffle(deck* d){

	int number_of_cards=0,i=1; 
	
	number_of_cards=rand() % get_number_of_cards_in_deck(*d);
	/*display_cards(*d);*/
	for (;i<=number_of_cards;i++)
	{
		swap_card(get_random_card(d), get_random_card(d));
	}
	/*display_cards(*d);*/
}