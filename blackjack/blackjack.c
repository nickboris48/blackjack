#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM_CARDS 52
typedef struct card{
	int val;
	char suit[10];
	char name[10];
} Card;

char suits[4][10] = {"Hearts","Diamonds","Clubs","Spades"};
char names[13][10] = {"Ace","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack","Queen","King"};

void play();
int bet(int *p);
Card *initdeck();
Card *shuffle(Card *deck);
void turn(Card *sdeck, int *pot);
void quit();
Card deal(Card *sdeck,int *inc);
void dd(int *ptotal, int *inc,Card *sdeck);
void hit(int *ptotal, int *inc, Card *sdeck, int *isA);
void dhit(int *dtotal, int *inc, Card *sdeck, int *isDA);

int main(){	
	printf("Welcome to my BlackJack Simulator! Below are the basic rules:\n- Beat the dealer's  hand without going over 21.\n-Dealer must play to 16.\n- Face cards are worth 10, Aces are worth 1 or 11, whichever makes a better hand.\n- Each player starts with two cards, and one of the dealer's cards is hidden until the end.\n- Type 'hit' to ask for another card. Type 'stand' to hold your total and end your turn.\n- If you go over 21 you bust, and the dealer wins regardless of his hand.\n- If you are dealt 21 from the start (Ace & 10), you got a blackjack. If you get a blackjack, you win 2 times the amount of your bet automatically.\n- Remember: Type 'hit' to get another card, and 'stand' to hold. At the beginning of the round, type 'bet' followed by the quantity you want to bet (i.e. 'bet 50').\nType 'play' to begin. At any time, you may type 'help' to get a list of valid commands.\n");
	char input[6];
	scanf("%5s", input);
	while(strcmp(input, "quit") != 0){
		if(strcmp(input, "play") == 0)
			play();
		else if(strcmp(input, "help")==0)
			printf("Type 'play' to start.");
		else
			printf("Invalid command. Type 'help' to be shown valid commands.");
		
		scanf("%s",input);
	}
	printf("Thanks for playing!\n");
}

void play(){
	printf("Lets start you off with 500 credits.\n");
	int balance = 500;
	while(balance>0){
		int pot = bet(&balance);
		Card *deck = initdeck();
		Card *sdeck = shuffle(deck);
		free(deck);
		turn(sdeck, &pot);
		balance += pot;
		printf("Your new balance is %d.\n", balance);
	}
	printf("Sorry, it looks like you lost! Type 'play' to play another game.\n");
}

void turn(Card *sdeck, int *pot){
	int inc = 0;
	Card pc1 = deal(sdeck,&inc);
	Card pc2 = deal(sdeck,&inc);
	Card dc1 = deal(sdeck,&inc);
	Card dc2 = deal(sdeck,&inc);
	int isA = 0;
	if(strcmp(pc1.name, "Ace") == 0 || strcmp(pc2.name, "Ace") == 0){
		if(strcmp(pc1.name, "Ace") == 0)
			pc1.val = 11;
		else
			pc2.val = 11;
		isA = 1;
	}
	int ptotal = pc1.val + pc2.val;
	int isDA = 0;
	if(strcmp(dc1.name, "Ace") == 0 || strcmp(dc2.name, "Ace") == 0){
		if(strcmp(dc1.name, "Ace") == 0)
			dc1.val = 11;
		else
			dc2.val = 11;
		isDA = 1;
	}
	int dtotal = dc1.val + dc2.val;
	printf("You've been dealt a %s of %s and a %s of %s for a total of %d.\n", pc1.name, pc1.suit, pc2.name, pc2.suit, ptotal);
	if(ptotal == 21){
		printf("Congrats! You got a blackjack! 2-1 payout.\n");
		*pot = (*pot*2);
		return;
	}
	printf("The dealer's face-up card is a %s of %s worth %d.\n", dc1.name, dc1.suit, dc1.val);
	char input[11];
	scanf("%10s", input);	
	while(1){
		if(strcmp(input,"double")==0){
			dd(&ptotal, &inc, sdeck);
			break;
		}else if(strcmp(input, "hit")==0){
			hit(&ptotal, &inc, sdeck, &isA);
			break;
		}else if(strcmp(input, "stand")==0){
			printf("You stand with %d\n", ptotal);
			break;
		}else if(strcmp(input, "help")==0)
			printf("You can choose to double down, stand, hit, or quit.\n");
		else if(strcmp(input, "quit")==0)
			quit();
		else
			printf("Invalid command. Try again or type 'help'.\n");
		scanf("%19s",input);
	}
	if(ptotal == 21){
		if(strcmp(input,"double")==0){
			printf("Congrats! You got a blackjack on a double! 4-1 payout.\n");
			*pot = (*pot*4);
		}else{
			printf("Congrats! You got a blackjack! 2-1 payout.\n");
			*pot = (*pot*2);
		}
		return;
	}else if(ptotal > 21){
		printf("Oh no! You busted. Try again!\n");
		if(strcmp(input, "double")==0)
			*pot = *pot*-2;
		else
			*pot = *pot*-1;
		return;
	}
	printf("The dealer flips his facedown card to reveal a %s of %s, making his new total %d.\n", dc2.name,dc2.suit,dtotal);
	if(dtotal < 16)
		dhit(&dtotal, &inc, sdeck, &isDA);
	if(ptotal > dtotal || dtotal > 21){
		if(strcmp(input,"double")==0){
			printf("Congrats! You beat the dealer on a double! Pay out 3-1.\n");
			*pot = *pot*3;
		}else{
			printf("Congrats! You beat the dealer! Pay out 1.5-1.\n");
			*pot = *pot*1.5;
		}
	}else if(ptotal < dtotal){
		printf("Oh no! The dealer won with a total of %d.\n", dtotal);
		if(strcmp(input, "double")==0)
			*pot = *pot*-2;
		else
			*pot = *pot*-1;
	}else{
		printf("Looks like you and the dealer tied! Push pot.\n");
		*pot = 0;
	}		
}

void dd(int *ptotal, int *inc, Card *sdeck){
	Card d = deal(sdeck, inc);
	if(d.name == "Ace"){
		if(*ptotal + 11 <= 21){
			*ptotal = *ptotal + 11;
			d.val = 11;
			printf("You were dealt an Ace of %s, worth 11. Your new total is %d.\n", d.suit, *ptotal);
		}else{
			*ptotal = *ptotal + 1;
			printf("You were dealt an Ace of %s, worth 1. Your new total is %d.\n", d.suit, *ptotal);
		}
	}else{
		*ptotal = *ptotal + d.val;
		printf("You were dealt a %s of %s, worth %d. Your new total is %d.\n", d.name, d.suit, d.val,*ptotal);
	}
}

void hit(int *ptotal, int *inc, Card *sdeck, int *isA){
	char input[11];
	strcpy(input, "hit"); 
	while(1){
		if(strcmp(input, "hit")==0){
			Card d = deal(sdeck,inc);
			if(d.name == "Ace"){
				if(*ptotal + 11 <= 21){
					*ptotal = *ptotal + 11;
					d.val = 11;
					*isA = 1;
					printf("You were dealt an Ace of %s, worth 11. Your new total is %d.\n", d.suit, *ptotal);
				}else{
					*ptotal = *ptotal + 1;
					printf("You were dealt an Ace of %s, worth 1. Your new total is %d.\n", d.suit, *ptotal);
				}
			}else{
				*ptotal = *ptotal + d.val;
				printf("You were dealt a %s of %s. Your new total is %d.\n", d.name, d.suit, *ptotal);
			}
			if(*ptotal >= 21){
				if(*isA==1){
					*ptotal=*ptotal-10;
					printf("You had an Ace worth 11, which was converted to a 1. Your new total is %d.\n", *ptotal);
					*isA = 0;
				}else
					break;
			}
		}else if(strcmp(input, "stand")==0){
			printf("You stand with %d.\n", *ptotal);
			break;
		}else if(strcmp(input, "help")==0)
			printf("You can choose to stand, hit, or quit.\n");
		else if(strcmp(input, "quit")==0)
			quit();
		else
			printf("Invalid command. Try again or type 'help'.\n");
		scanf("%10s", input);
	}
}

void dhit(int *dtotal, int *inc, Card *sdeck, int *isDA){
	while(*dtotal < 16){
		Card d = deal(sdeck,inc);
		if(d.name == "Ace"){
			if(*dtotal + 11 <= 21){
				*dtotal = *dtotal + 11;
				d.val = 11;
				*isDA = 1;
				printf("The dealer was dealt an Ace of %s, worth 11. The dealer's new total is %d.\n", d.suit, *dtotal);
			}else{
				*dtotal = *dtotal + 1;
				printf("The dealer was dealt an Ace of %s, worth 1. The dealer's new total is %d.\n", d.suit, *dtotal);
			}
		}else{
			*dtotal = *dtotal + d.val;
			printf("The dealer was dealt a %s of %s. The dealer's new total is %d.\n", d.name, d.suit, *dtotal);
		}
		if(*dtotal >= 21){
			if(*isDA==1){
				*dtotal = *dtotal-10;
				printf("The dealer had an Ace worth 11, which was converted to a 1. The dealer's new total is %d.\n", *dtotal);
				*isDA = 0;
			}else
				break;
		}
	}
	printf("The dealer stands with %d.\n", *dtotal);
}

Card deal(Card *sdeck, int *inc){
	Card c = sdeck[*inc];
	*inc = *inc + 1;
	return c;
}

int bet(int *p){
	char input[6];
	int val;
	printf("Enter an amount you would like to bet.\n");
	scanf("%5s", input);
	while(1){
		if(strcmp(input, "bet")==0){
			scanf("%d", &val);
			if(val>*p)
				printf("You cannot bet more than your current balance. Your balance is: %d", *p);
			else
				break;
		}else if(strcmp(input, "help")==0)
			printf("To make a bet, type in 'bet', followed by an amount you you want to bet. For example, if you want to bet 50, type 'bet 50'.\n");
		else if(strcmp(input, "quit")==0)
			quit();
		else
			printf("Invalid command. Type 'help' if needed.\n");
		scanf("%5s", input);
	}
	printf("You've made a bet of %d. Good luck!\n", val);
	int pot = val;
	return pot;		
}

Card *initdeck(){
	int i=1;
	int j=1;
	int inc = 0;
	Card *deck = malloc(52 * sizeof(Card));
	for(i;i<=4;i++){
		j=1;
		for(j;j<=13;j++){
			Card c;
			if(j<=10)
				c.val = j;
			else
				c.val = 10;
			strcpy(c.suit,suits[i-1]);
			strcpy(c.name,names[j-1]);
			deck[inc] = c;
			inc++;
		}
	}
	return deck;	
}

Card *shuffle(Card *deck){
	srand(time(NULL));
	int i=0;
	Card *sdeck = malloc(52 * sizeof(Card));
	for(i;i<52;i++){
		sdeck[i] = deck[i];
	}
	i=0;
	for(i=NUM_CARDS-1;i>0;i--){
		int j = rand()%(i+1);
		Card n = sdeck[i];
		sdeck[i] = sdeck[j];
		sdeck[j] = n;
	}
	return sdeck;
}

void quit(){
	printf("Are you sure you want to quit? Type 'y' or 'n'.\n");
	char input[6];
	scanf("%s",input);
	if(strcmp(input,"y")==0)
		exit(0);
	else 
		printf("Quit cancelled.\n");
}
