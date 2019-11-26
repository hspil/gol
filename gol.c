/***********************************************************************
Conway's Game of Life

 * Toroidal Universe
 * Standard B3/S23 rule by default.
 * Adjustable below.
***********************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Global height and width of board
int boardHeight, boardWidth;

void change(int board[boardHeight][boardWidth], int B[1], int S[2]);
void draw(int* board);

int main(int argc, char* argv[])
{
	// B3/S23 rule
	// https://conwaylife.com/wiki/Rulestring
	int B[1] = {3};
	int S[2] = {2, 3};

	// Check CLI arguments
	if (argc != 4)
	{
		printf("Usage: ./gol <file> <board width> <board height>");
		return 1;
	}

	char* filename = argv[1];
	boardHeight = atoi(argv[3]);
	int board[boardHeight][boardWidth];

	for (int i = 0; i < boardHeight; i++)
	{
		for (int j = 0; j < boardWidth; j++)
		{
			board[i][j] = 0;
		}
	}

	// If file does not exist, create a new one and populate it with periods
	if (access(filename, F_OK) == -1)
	{
		FILE *fp;
		fp = fopen(filename, "w");

		for (int y = 0; y < boardHeight; y++)
		{
			for (int x = 0; x < boardWidth; x++)
			{
				fprintf(fp, ".");
			}
			fprintf(fp, "\n");
		}

		fclose(fp);
		printf("%s created.\n", filename);
		printf("Open the file in a text editor and change full stops to octothorpes before running this program again.");
		return 0;
	}

	char data;

	// Read file and store into board array
	FILE *fp;
	fp = fopen(filename, "r");
	for(int y = 0; y < boardHeight; y++)
	{
		for(int x = 0; x < boardWidth; x++)
		{
			data = fgetc(fp);

			// Don't store newline chars, read off another char
			switch (data) {

			case '\n':
				data = fgetc(fp);

			case '.':
				board[y][x] = 0;
				break;

			case '#':
				board[y][x] = 1;
				break;

			default:
				fclose(fp);
				printf("Error: File malformed.\n");
				return 1;
			}
		}
	}

	fclose(fp);

	// Primary game loop
	while (1 == 1)
	{
		draw(board[boardHeight][boardWidth]);
		change(board, B, S);
		usleep(250000);
		printf("\n\n\n");
	}
}


void change(int *board, int *B, int *S)
{
	int b_size = sizeof(B) / sizeof(int);
	int s_size = sizeof(S) / sizeof(int);

	int changes[boardHeight][boardWidth];
	for (int y  = 0; y < boardHeight; y++)
	{
		for (int x = 0; x < boardWidth; x++)
		{
			changes[y][x] = 0;
		}
	}

	// Find what changes need to be made
	int neighbor;
	for (int y  = 0; y < boardHeight; y++)
	{
		for (int x = 0; x < boardWidth; x++)
		{
			neighbor = 0;

			// Count living neighbors
			for (int offy = -1; offy < 2; offy++)
			{
				for (int offx = -1; offx < 2; offx++)
				{
					// Do not count self as a neighbor
					if (offy != 0 && offx != 0)
					{
						neighbor += board[(y + offy) % boardHeight][(x + offx) % boardWidth];
					}
				}
			}

			// Cell birth
			if (board[y][x] == 0)
			{
				for (int b = 0; b < b_size; b++)
				{
					if (B[b] == neighbor)
					{
						changes[y][x] = 1;
					}
				}
			}
			else
			{
				int found;
				for (int s = 0; s < s_size; s++)
				{
					if (S[s] == neighbor)
					{
						found = 1;
					}
				}

				// If not found in S
				if (found != 1)
				{
					changes[y][x] = 1;
				}
			}
		}
	}

	// XOR change array with board array
	for (int y  = 0; y < boardHeight; y++)
	{
		for (int x = 0; x < boardWidth; x++)
		{
			// Thanks to Matt Ball
			// https://stackoverflow.com/questions/17024355/is-there-a-logical-boolean-xor-function-in-c-or-c-standard-library
			board[y][x] = (board[y][x] != changes[y][x])? 1: 0;
		}
	}
}


void draw(int board[boardHeight][boardWidth])
{
	for (int y  = 0; y < boardHeight; y++)
	{
		for (int x = 0; x < boardWidth; x++)
		{
			printf((board[y][x] == 1)? "#": ".");
		}
		printf("\n");
	}
}
