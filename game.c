/************************************************************************************************************
** Written by..: Storple
** Date Written: 3/3/2024
************************************************************************************************************/

#include "template.h"
#define SIZE 10

// function prototypes
void arrcpy(char arr1[SIZE][SIZE], char arr2[SIZE][SIZE]);
int checkGameOver(int floating[5]);
int checkShipSunk(char arr[SIZE][SIZE], char shipName);
void getCoords(int* coordY, int* coordX, char* i);
void initializeField(char arr[SIZE][SIZE]);
int isValidPlacement(char arr[SIZE][SIZE], int x, int y, int shipSize, int direction);
void loadFromFile(char arr[SIZE][SIZE], char battleship[SIZE][SIZE], char generatedCopy[SIZE][SIZE], int* count, int floating[5]);
void placeShips(char arr[SIZE][SIZE]);
void printField(char arr[SIZE][SIZE]);
void printStatus(int floating[5], int count);
void updatePlayerBoard(char arr[SIZE][SIZE], char battleship[SIZE][SIZE], char generatedCopy[SIZE][SIZE], int x, int y, int* count, int floating[5]);
void updateShipSunk(char arr[SIZE][SIZE], char generatedCopy[SIZE][SIZE], char shipName);
void saveToFile(char arr[SIZE][SIZE], char battleship[SIZE][SIZE], char generatedCopy[SIZE][SIZE], int count, int floating[5]);

int main() {
	char battleship[SIZE][SIZE];
	char playerBoard[SIZE][SIZE];
	char generatedCopy[SIZE][SIZE];
	int floating[] = {1, 1, 1, 1, 1};
	int count = 0;

	int choice = 0;
	getChoice(&choice);
	if (choice == 0) {
		loadFromFile(playerBoard, battleship, generatedCopy, &count, floating);
	}
	else {
		initializeField(battleship);
		initializeField(playerBoard);

		printf("Generating battleship field...\n\n");

		placeShips(battleship);
		arrcpy(generatedCopy, battleship);
	}

	CLS;
	int x, y;

	while (1) {
		printStatus(floating, count);
		printf("\nBattleship Board\n");
		printField(playerBoard);
		getCoords(&x, &y, "Enter coordinates (example: A0): ");
		if (y == -1) // user enters 'QQ' to quit
			break; 
		updatePlayerBoard(playerBoard, battleship, generatedCopy, x, y, &count, floating);
		if (checkGameOver(floating)) { // check if game is over
			printf("\nYou have won!");
			break;
		}
		saveToFile(playerBoard, battleship, generatedCopy, count, floating);
		CLS;
	}

	
}

void arrcpy(char arr1[SIZE][SIZE], char arr2[SIZE][SIZE]) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			arr1[i][j] = arr2[i][j];
		}
	}
}

int checkGameOver(int floating[5]) {
	// check if all ships have been sunken
	for (int i = 0; i < 5; i++) {
		if (floating[i] == 1)
			return 0;
	}
	return 1;
}

int checkShipSunk(char arr[SIZE][SIZE], char shipName) {
	// check if a ship has sunk on the board
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (arr[i][j] == shipName) {
				return 0;
			}
		}
	}
	return 1;
}

void getCoords(int* coordY, int* coordX, char* i){
  int run = 1; //initializes variable to get proper input
  do {
    printf("%s", &*i); //prompts user
    char coordYLet;
    char coordXLet;
    
    if (scanf("%c %c", &coordYLet, &coordXLet) != 2){
      FLUSH; //removes all extra input
      printf("\nPlease submit a proper value\n"); //reprompts user
      continue;
    }
    if (toupper(coordXLet) == 'Q' && toupper(coordXLet) == 'Q'){
      *coordY = -1;
      *coordX = -1;
      break;
    }
    
    if ((int)toupper(coordXLet)-48 < 0 || (int)toupper(coordXLet)-48 > 9 ||  ((int)toupper(coordYLet)-65) < 0 || ((int)toupper(coordYLet)-65) > 9){ //checks if input is okay
      FLUSH; //removes all extra input
      printf("\nPlease submit a proper (A-Z)(0-9) value\n\n"); //reprompts user
      continue;
    }
    *coordY = ((int)toupper(coordYLet)-65); //edits coordY
    *coordX = (int)toupper(coordXLet)-48;
    run = 0; //ends loop
  } while (run);
  FLUSH; //removes all extra input
}

void initializeField(char arr[SIZE][SIZE]) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			arr[i][j] = '.'; // '.' represents empty spot
		}
	}
}

int isValidPlacement(char arr[SIZE][SIZE], int x, int y, int shipSize, int direction) {
	if (direction == 0) { // horizontal direction
		if (y + shipSize > SIZE) // out of bounds
			return 0;
		for (int i = y; i < y + shipSize; i++) {
			if (arr[x][i] != '.') // spot is already occupied
				return 0;
		}
	}
	else { // vertical direction
		if (x + shipSize > SIZE) // out of bounds
			return 0;
		for (int i = x; i < x + shipSize; i++) {
			if (arr[i][y] != '.')
				return 0; // spot is already occupied
		}
	}
	return 1;
}

void loadFromFile(char arr[SIZE][SIZE], char battleship[SIZE][SIZE], char generatedCopy[SIZE][SIZE], int* count, int floating[5]) {
	FILE* file = fopen("index.bin", "rb");
	if (file == NULL) { // there was an error opening the file
		perror("There was an error opening the file...");
		exit(EXIT_FAILURE);
	}

	fread(arr, sizeof(char), 100, file);
	fread(battleship, sizeof(char), 100, file);
	fread(generatedCopy, sizeof(char), 100, file);
	fread(count, sizeof(int), 1, file);
	fread(floating, sizeof(int), 5, file);

	fclose(file);
}

void placeShips(char arr[SIZE][SIZE]) {
	int shipSizes[] = {2,3,3,4,5};
	char shipNames[] = {'Z', 'Y', 'X', 'W', 'V'};
	for (int i = 0; i < 5; i++) {
		int shipSize = shipSizes[i];
		int direction = randomNum(0, 1); // 0 represents horizontal direction, 1 represents vertical
		int x, y;
		do {
			x = randomNum(0, SIZE - 1);
			y = randomNum(0, SIZE - 1);
		} while (!isValidPlacement(arr, x, y, shipSize, direction));
		if (direction == 0) { // horizontal direction
			for (int j = 0; j < shipSize; j++) {
				arr[x][y + j] = shipNames[i];
			}
		}
		else { // vertical direction
			for (int j = 0; j < shipSize; j++) {
				arr[x + j][y] = shipNames[i];
			}
		}
	}
}

void printField(char arr[SIZE][SIZE]) {
	// prints field with 0-9 on the horizontal axis and a-j on the vertical axis
	printf("  ");
	int i, j;
	char c;
	for (i = 0; i < SIZE; i++) {
		printf("%d ", i);
	}
	printf("\n");
	for (i = 0, c = 'A'; i < SIZE; i++, c++) {
		printf("%c ", c);
		for (j = 0; j < SIZE; j++) {
			printf("%c ", arr[i][j]);
		}
		printf("\n");
	}
}

void printStatus(int floating[5], int count) {
	char* shipNames[5] = { "Zule Ship", "Yeti Ship", "XMax Ship", "Wiener Ship", "Violet Ship" };

	// prints if each ship is either sunk or floating
	for (int i = 0; i < 5; i++) {
		if (floating[i] == 0)
			printf("%s\tSunk\n", shipNames[i]);
		else
			printf("%s\tFloating\n", shipNames[i]);
	}

	// prints total missiles fired
	printf("\nMissiles Fired: %d\n", count);
}

void updatePlayerBoard(char arr[SIZE][SIZE], char battleship[SIZE][SIZE], char generatedCopy[SIZE][SIZE], int x, int y, int* count, int floating[5]) {
	if (battleship[x][y] != '.') {
		char shipName = battleship[x][y];
		battleship[x][y] = 'H';
		arr[x][y] = 'H';

		// check if the ship was sunk
		if (checkShipSunk(battleship, shipName)) {
			// update player board
			updateShipSunk(arr, generatedCopy, shipName);
			char shipNames[] = { 'Z', 'Y', 'X', 'W', 'V' };
			for (int i = 0; i < 5; i++) {
				if (shipNames[i] == shipName)
					floating[i] = 0;
			}
		}	
		(*count)++;
	}
	else if (battleship[x][y] == '.') {
		arr[x][y] = 'M';
		(*count)++;
	}
}

void updateShipSunk(char arr[SIZE][SIZE], char generatedCopy[SIZE][SIZE], char shipName) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (generatedCopy[i][j] == shipName)
				arr[i][j] = shipName;
		}
	}
}

void saveToFile(char arr[SIZE][SIZE], char battleship[SIZE][SIZE], char generatedCopy[SIZE][SIZE], int count, int floating[5]) {
	FILE* file = fopen("index.bin", "wb");
	if (file == NULL) { // there was an error opening the file
		perror("There was an error opening the file...");
		exit(EXIT_FAILURE);
	}

	fwrite(arr, sizeof(char), 100, file);
	fwrite(battleship, sizeof(char), 100, file);
	fwrite(generatedCopy, sizeof(char), 100, file);
	fwrite(&count, sizeof(int), 1, file);
	fwrite(floating, sizeof(int), 5, file);

	fclose(file);
}