// Jason Weinberg

#include<stdio.h>

int rollDie() {
	char dice1 = 0, dice2 = 0, total = 0;
	FILE *fp;

	// Generate two dice rolls via dice driver.
	fp = fopen("/dev/dice", "r");
	fread(&dice1, 1, 1, fp);
	fread(&dice2, 1, 1, fp);
	fclose(fp);

	// Increment values of dice so they are in the interval [1, 6].
	dice1++;
	dice2++;

	// Add values of dice rolls to total.
	total = dice1 + dice2;

	// Output dice values.
	printf("You have rolled %d + %d = %d\n", dice1, dice2, total);

	// Return total value of dice rolls.
	return total;
}

int playRound() {
	char total = 0, point = 0, result = 0;

	// Roll two die and add to total.
	total = rollDie();

	// If total is 2, 3, or 12, player loses.
	if(total == 2 || total == 3 || total == 12) {
		result = 1;
	// If total is 7 or 11, user wins.
	} else if(total != 7 && total != 11) {
		point = total;
		for( ; ; ) {
			// Roll die again.
			total = rollDie();
			// If total is 7, user loses. If total equals point, user wins.
			if(total == 7 || total == point) {
				if(total == 7) {
					result = 1;
				} break;
			}
		}
	}

	// Return result of round. 0 = win, 1 = loss.
	return result;
}

int main() {
	char name[255];
	char input[255];
	char result = 0;

	// Welcome and ask for name
	printf("Welcome to Jason's Casino!\n");
	printf("Please enter your name: ");
	scanf("%s", &name);
	printf("%s, would you like to Play or Quit? ", name);
	scanf("%s", &input);

	// Does user want to play?
	if(strcmp(input, "play") != 0) {
		return 0;
	}

	// Infinite loop to keep game running until player wants to quit
	for( ; ; ) {
		printf("\n");

		// Play round.
		result = playRound();

		// Check result of round. 1 = win, 2 = loss.
		if(result != 0) {
			printf("You Lost.\n\n");
		} else {
			printf("You Win!\n\n");
		}

		printf("Would you like to play again? ");
		scanf("%s", &input);

		// Play again?
		if(strcmp(input, "yes") != 0) {
			break;
		}
	}

	// Say goodbye.
	printf("\nGoodbye, %s!\n\n", name);

	return 0;
}
