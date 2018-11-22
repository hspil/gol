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

//global height and width of board, board

int h, w;

//prototypes
void change(int board[h][w], int B[1], int S[2]);
void draw(int* board);


int main(int argc, char* argv[])
{
	//B3/S23 rule
	int B[1] = {3};
    int S[2] = {2, 3};
	
	//Check CLI arguments
	if (argc != 4)
	{
		printf("Usage: ./gol <file> <board width> <board height>");
        return 1;
	}
	
	//pull filename from CLI argument
	char* filename = argv[1];
	
	//pull width and height for board from command line argument and store into global width and height
	//w = atoi(argv[2]);
    h = atoi(argv[3]);
    
    int board[h][w];
	
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			board[i][j] = 0;
		}
	}
	
	//if file does not exist
	if (access(filename, F_OK) == -1)
	{
		//create file
		FILE *fp;
		fp = fopen(filename, "w");
		//fill with periods
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				fprintf(fp, ".");
			}
			fprintf(fp, "\n");
		}
		//close file
		fclose(fp);
		printf("%s", filename);
		printf(" created.");
		printf("Open the file in a text editor and change full stops to octothorpes before running this program again.");
		return 0;
	}
	else
	{
		char data;
		//read file and store into board array
		FILE *fp;
		fp = fopen(filename, "r");
		for(int y = 0; y < h; y++)
		{
			for(int x = 0; x < w; x++)
			{
				data = fgetc(fp);
				//Don't store newline chars, read off another char
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
					return 2;
				}
			}
		}
		//close file
		fclose(fp);
		
		//primary game loop
		while (1 == 1)
		{
			draw(board[h][w]);
			change(board, B, S);
			usleep(250000);
			printf("\n\n\n");
		}
	}
}



void change(int *board, int *B, int *S)
{
	int b_size = sizeof(B) / sizeof(int);
	int s_size = sizeof(S) / sizeof(int);
	
	int changes[h][w];
	for (int y  = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			changes[y][x] = 0;
		}
	}
	
	//find what changes need to be made
	int neigh;
	for (int y  = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			neigh = 0;
			//count live neighbors
			for (int offy = -1; offy < 2; offy++)
			{
				for (int offx = -1; offx < 2; offx++)
				{
					//do not count self as a neighbor
					if (offy != 0 && offx != 0)
					{
						neigh += board[(y + offy) % h][(x + offx) % w];
					}
				}
			}
			//birth
			if (board[y][x] == 0)
			{
				for (int b = 0; b < b_size; b++)
				{
					if (B[b] == neigh)
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
					if (S[s] == neigh)
					{
						found = 1;
					}
				}
					
				//if not found in S
				if (found != 1)
				{
					changes[y][x] = 1;
				}
			}
		}
	}
	
	//xor array of changes needed with board
	for (int y  = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			//thanks to Matt Ball on StackOverflow: https://stackoverflow.com/questions/17024355/is-there-a-logical-boolean-xor-function-in-c-or-c-standard-library
			board[y][x] = (board[y][x] != changes[y][x])? 1: 0;
		}
	}
}


void draw(int board[h][w])
{
	
	for (int y  = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			printf((board[y][x] == 1)? "#": ".");
		}
		printf("\n");
	}
}
