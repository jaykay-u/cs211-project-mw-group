#include <iostream>
#include <random>
#include <iomanip>
using namespace std;

void move_left(int board[][5], int numrows = 5)
{
	for (int j = 0; j < 5; j++) {
		for (int i = 1; i < 5; i++)
		{
			if (board[j][i-1] == 0) {
				board[j][i-1] = board[j][i] + board[j][i-1];
				board[j][i] = 0;
			}
			else {
				if (board[j][i] == board[j][i-1]) 
				{
					board[j][i-1] = board[j][i] + board[j][i-1];
					board[j][i] = 0;
				}
			}
		}
	}
}

void move_right(int board[][5], int numrows = 5)
{
	//CODE HERE
}

void move_up(int board[][5], int numrows = 5)
{
	//CODE HERE
}

void move_down(int board[][5], int numrows = 5)
{
	//move one column at a time
	for(int c = 0; c < 5; c++)
	{
		//array holds only the numbers that are not zero in this column.
		int values[5] = {0};
		int count = 0;

		//read the column from bottom to top and save the non-zero values.
		for(int r = numrows - 1; r >= 0; r--)
		{
			if(board[r][c] != 0)
			{
				values[count] = board[r][c];
				count = count + 1;
			}
		}

		//merge equal numbers together
		for(int i = 0; i < count; i++)
		{
			if(i + 1 < count && values[i] == values[i + 1])
			{
				//double the current value
				values[i] = values[i] * 2;

				//move later values left to fill gap
				for(int j = i + 1; j < count - 1; j++)
				{
					values[j] = values[j + 1];
				}

				//set the last value to 0 because it was pushed out
				values[count - 1] = 0;

				//one value was removed so reduce the count
				count = count - 1;

				//check this same position again
				i = i - 1;
			}
		}

		//write the merged values back into the board from bottom to top
		for(int r = numrows - 1, i = 0; r >= 0; r--)
		{
			if(i < count)
			{
				board[r][c] = values[i];
				i = i + 1;
			}
			else
			{
				board[r][c] = 0;
			}
		}
	}
}


void print_board(int board[][5], int numrows = 5)
{
	cout << endl << "\t-----------------------------------------" << endl;

	for (int r = 0; r < numrows; r++)
	{
		for (int c = 0; c < 5; c++)
		{
			if(board[r][c] > 0)
				cout << "\t|" << right << setw(6) << setfill(' ') << board[r][c];
			else
				cout << "\t|" << right << setw(6) << setfill(' ') << " ";

			//cout << board[r][c] << "\t|\t";
		}
		cout << "\t|" << endl;
		cout << "\t-----------------------------------------" << endl;
	}

	cout << "\t" << endl << endl;
}


void insert_random(int board[][5], int numrows = 5)
{
	int r, c;
	//count empty
	int empty_count = 0;
	for (int r = 0; r < numrows; r++)
	{
		for (int c = 0; c < 5; c++)
		{
			if (board[r][c] == 0)
				empty_count++;
		}
	}

	if (empty_count > 0)
	{
		while (true)
		{
			//generate random location r, c
			r = rand() % numrows;
			c = rand() % numrows;
			//check if empty
			if (board[r][c] == 0)
			{
				//add number randomly {2,4}
				board[r][c] = 2 * (1 + (rand() % 2));
				break;
			}
		}

	}
}