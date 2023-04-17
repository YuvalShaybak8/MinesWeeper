#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#define MAXSIZE 22 //constant variable

//function declaration
void InGame(int curRows, int curCols);
void useCheat(int Board[][MAXSIZE], bool IsHidden[][MAXSIZE], int y, int curRows, int curCols);
void printarr(int Board[][MAXSIZE], bool IsHidden[][MAXSIZE], int curRows, int curCols, bool iamFlag[][MAXSIZE]);
bool playerWon(int Board[][MAXSIZE], bool IsHidden[][MAXSIZE], bool iamFlag[][MAXSIZE], int curRows, int curCols);
void resetAlltheboard(int Board[][MAXSIZE], bool IsHidden[][MAXSIZE], bool iamFlag[][MAXSIZE], int curRows, int curCols);
void openAllthecells(bool IsHidden[][MAXSIZE], int curRows, int curCols);
void initBomb(int Board[][MAXSIZE], int curRows, int curCols);
bool cellInBounds(int Board[][MAXSIZE], bool IsHidden[][MAXSIZE], bool iamFlag[][MAXSIZE], int curRows, int curCols, int x, int y);
void openRecursive(int Board[][MAXSIZE], bool IsHidden[][MAXSIZE], bool iamFlag[][MAXSIZE], int curRows, int curCols, int Rows, int Cols);

void main()
{
	int customRows, customCols;
	bool gameRunning = true;
	int choice; // Represent user choice of option.

	while (gameRunning)//while Gamerunning is true , so start the game.
	{
		//print menu for the user to choose
		printf("\nWelcome to Minesweeper!\n\nPlease choose one of the following options and enter it's number:\n\n1 - for size 8X8\n\n2 - for size 12X12\n\n3 - for size 15X15\n\n4 - for custom size\n\n0 - Exit\n\n");
		scanf("%d", &choice);

		switch (choice)
			//// Check user menu choice input.
		{
		case 0:
			// In case user chose to exit game.
			printf("Goodbye hope to see you soon!\n");
			gameRunning = false;
			continue;
		case 1:
			// 8X8 board.
			InGame(8, 8);//Call to function for the whole game.
			gameRunning = false;
			break;
		case 2:
			// 12X12 board.
			InGame(12, 12);//Call to function for the whole game.
			gameRunning = false;
			break;
		case 3:
			// 15X15 board.
			InGame(15, 15);//Call to function for the whole game.
			gameRunning = false;
			break;
		case 4:
			// In case the user chose the custom board
			printf("Please enter the size of the board(Lines <= %d and Cols <= %d) :", MAXSIZE, MAXSIZE);
			scanf("%d %d", &customRows, &customCols);
			if (((customRows > MAXSIZE) || (customRows <= 0)) || ((customCols > MAXSIZE) || (customCols <= 0)))
			{
				printf("\nOut Of Range , Set to maximum Size : %d)\n", MAXSIZE);
				continue;
			}
			InGame(customRows, customCols);//Call to function for the whole game.
			gameRunning = false;
			break;
		default:
			// case input isn't 0-4.
			continue;
		}
	}
}

//A function in which the entire game takes place, which means that if you enter it
//you have probably entered a correct output and selected one of the game boards offered in the menu.
void InGame(int curRows, int curCols)
{
	//definition:
   // value -1 bomb.
	// value 0 no bomb around.
	// value 1, 2, 3... numbers of bomb around this numbers.

	bool IsHidden[MAXSIZE][MAXSIZE];// An array that returns true or false depending on the open or closed cells
	int Board[MAXSIZE][MAXSIZE];// An array that represents the numeric value of the cell in the board in the specific cells
	bool iamFlag[MAXSIZE][MAXSIZE];// An array that returns true or false depending on the cells that as a flag or not

	resetAlltheboard(Board, IsHidden, iamFlag, curRows, curCols);//Call to function

	initBomb(Board, curRows, curCols);//Call to function

	bool looseOrWin = false;//fale- the user didnt lose
	bool PlayerWon = false;
	while (!looseOrWin)
	{
		printarr(Board, IsHidden, curRows, curCols, iamFlag);// print the board from function

		char flagOrOpen;
		int x, y;
		bool validInput = true;
		while (validInput)
		{
			printf("Please enter your move, row and column :");
			scanf("%d %d", &x, &y);
			printf("\n");
			if (y >= 0) {
				if (x == -1)
				{
					fseek(stdin, 0, SEEK_END);//clean all the things after the two frist numbers
					validInput = false;
					continue;
				}
				else
				{
					scanf(" %c", &flagOrOpen);

					// if the input of the chars invalid- print a message and bring the user another chance until he enters a valid choice
					if ((flagOrOpen != 'F') && (flagOrOpen != 'f') && (flagOrOpen != 'o') && (flagOrOpen != 'O'))
					{
						printf("Please enter a valid choice!\n");
						continue;
					}

					// if the input of the rows and cols invalid- print a message and bring the user another chance until he enters a valid choice
					if ((x < -1 || x > curRows) || (y > curCols))
					{
						printf("Please enter a valid choice!\n");
						continue;
					}
					validInput = false;
				}
			}
			else {
				fseek(stdin, 0, SEEK_END);//clean all the things after the two frist numbers
			}
			if(validInput)
				printf("Please enter a valid choice!\n");
		}

		if (x == -1) //if valid input- continue to the function and to open the cells.
		{
			useCheat(Board, IsHidden, y, curRows, curCols);//Call to function.
		} else {

			if (IsHidden[x][y] == false && iamFlag[x][y] == false)// if the entered row and col allready opened print message to choose again.
			{
				printf("Invalid move, please enter valid choice!\n");
				continue;
			}

			if (flagOrOpen == 'F' || flagOrOpen == 'f') {
				iamFlag[x][y] = true;
				continue;
			}
 
			iamFlag[x][y] = false;

			if (Board[x][y] == -1) //if the cell is a bomb- print game over and end game.
			{
				looseOrWin = true;
				continue;
			}

			openRecursive(Board, IsHidden, iamFlag, curRows, curCols, x, y);//Call to function.
			
		}
		PlayerWon = playerWon(Board, IsHidden, iamFlag, curRows, curCols);//Call to function.
		if (PlayerWon)
			looseOrWin = true;
	}

	if (PlayerWon == true)// if the bool palyerwon true make a " You Won " message.
	{
		printf("\nYou won!!congragulations!CHAMPION!\n");
		looseOrWin = true;
	}
	else {
		printf("\nYou've hit a bomb! Game over!\n");
		openAllthecells(IsHidden, curRows, curCols);//Call to function to open all the cells.
	}

	printarr(Board, IsHidden, curRows, curCols, iamFlag);// print the board after open.
}

//print the values of all the cells in the board.
void printarr(int Board[][MAXSIZE], bool IsHidden[][MAXSIZE], int curRows, int curCols, bool iamFlag[][MAXSIZE])
{
	printf("     ");
	for (int z = 0; z < curCols; z++)
	{
		printf("%c[4m%3d %c[0m", 27, z, 27);
	}
	printf("\n");
	for (int i = 0; i < curRows; i++)
	{
		for (int j = 0; j < curCols; j++)
		{
			if (j == 0)
			{
				printf("  %2d |", i);
			}
			if (iamFlag[i][j])
			{
				printf("%2c |", 'F');
			}
			else if (IsHidden[i][j])
			{
				printf("%2c |", 'X');
			}
			else
			{
				if (Board[i][j] == -1)
				{
					printf("%2c |", '*');
				}
				else
				{
					if (Board[i][j] == 0)
					{
						printf("%2c |", ' ');
					}
					else
					{
						printf("%2d |", Board[i][j]);
					}
				}
			}
		}
		printf("\n");
	}
	printf("    ");
	for (int i = 0; i < curCols; i++)
	{
		printf("%c[4m%4c%c[0m", 27, ' ', 27);
	}
	printf("\n");
}

//The cheat function comes to help the tester
//if he wants to see if the game works and expose cells quickly without encountering a mine
//and that way you can also win the game
void useCheat(int Board[][MAXSIZE], bool IsHidden[][MAXSIZE], int y, int curRows, int curCols)
{
	int count = 0;
	if (y <= 0)
	{
		printf("Please enter a valid choice!\n");
		return;
	}
	for (int i = 0; i < curRows; i++)
	{
		for (int j = 0; j < curCols; j++)
		{
			if (Board[i][j] != -1 && IsHidden[i][j] == true)
			{
				IsHidden[i][j] = false;
				count++;
				if (count == y)
					return;
			}
		}
	}
}

//The function detects 8 bombs in the cells at the beginning of the board printing at random
void initBomb(int Board[][MAXSIZE], int curRows, int curCols)
{
	srand((unsigned)time(NULL));//Using the rand function

	int count = 0;
	int sqr = sqrt((double) curRows * curCols);

	while (count < sqr)//put 8 random Mines in different cells.
	{
		int rows, cols;
		rows = rand() % curRows;
		cols = rand() % curCols;
		if (Board[rows][cols] != -1) // if the cell is not a Mine, put new Mine.
		{
			Board[rows][cols] = -1;
			for (int k = rows - 1; k < rows + 2; k++)
			{
				for (int s = cols - 1; s < cols + 2; s++)
				{
					if ((k >= 0 && k <= curRows - 1) && (s >= 0 && s <= curCols - 1) && (k != rows || s != cols))
						// Consider the boundaries of the board and if the cell is not a mine-promote the value of the cell by plus 1
					{
						if (Board[k][s] != -1)
						{
							Board[k][s]++;
						}
					}
				}
			}
			count++;
		}
	}
}

//A bool function that checks if the user has won or if there are any more hidden cells
//that do not have a bomb coming that he needs to reveal
bool playerWon(int Board[][MAXSIZE], bool IsHidden[][MAXSIZE], bool iamFlag[][MAXSIZE], int curRows, int curCols)
{
	for (int i = 0; i < curRows; i++)// check all the board and if the user still dont see a bomb but still have hidden cell , let him to continue.
	{
		for (int j = 0; j < curCols; j++)
		{
			if ((Board[i][j] != -1) && ((IsHidden[i][j] == true) || iamFlag[i][j] == true))
			{
				return false;
			}
		}
	}
	return true;
}

//A function that if a loss user opens all the cells and shows the values ​​and bombs
void openAllthecells(bool IsHidden[][MAXSIZE], int curRows, int curCols)
{
	for (int i = 0; i < curRows; i++)//open all the cells on the board
	{
		for (int j = 0; j < curCols; j++)
		{
			IsHidden[i][j] = false;
		}
	}
}

//A function that resets the entire board to 0 and hides them
void resetAlltheboard(int Board[][MAXSIZE], bool IsHidden[][MAXSIZE], bool iamFlag[][MAXSIZE], int curRows, int curCols)
{
	for (int i = 0; i < curRows; i++)//To reset the value of each cell
	{
		for (int j = 0; j < curCols; j++)
		{
			iamFlag[i][j] = false;
			Board[i][j] = 0;
			IsHidden[i][j] = true;
		}
	}
}

// cellInBounds - function return true if cell is meets the conditions of valid cell.
bool cellInBounds(int Board[][MAXSIZE], bool IsHidden[][MAXSIZE], bool iamFlag[][MAXSIZE], int curRows, int curCols, int x, int y) {
	if ((x < 0 || x >= curRows) || (y < 0 || y >= curCols) || Board[x][y] == -1 || iamFlag[x][y] == true || IsHidden[x][y] == false)
		return false;
	return true;
}

//The recursive function, opens the empty cells next to the match used to open
//until the moment you meet a cell with a number like 1, 2 and so on.
void openRecursive(int Board[][MAXSIZE], bool IsHidden[][MAXSIZE], bool iamFlag[][MAXSIZE], int curRows, int curCols, int x, int y)
{
	if (!cellInBounds(Board, IsHidden, iamFlag, curRows, curCols, x, y))
		return;

	IsHidden[x][y] = false; //if the input is valid and not a bomb - open the cell

	// if cell's arounding cells contains at least 1 "bomb-cell" do not keep opening arounding cells.
	if (Board[x][y] > 0)
		return;

	for (int rows = x - 1; rows < x + 2; rows++)
	{
		for (int cols = y - 1; cols < y + 2; cols++)
		{
			openRecursive(Board, IsHidden, iamFlag, curRows, curCols, rows, cols);
		}
	}
}
