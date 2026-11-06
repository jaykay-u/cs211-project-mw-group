#include <iostream>
#include <random>
#include <iomanip>
using namespace std;

//collapse_line slides one line toward index 0 and merges equal neighbors once
int collapse_line(int line[5], int &score)
{
	//save only the non-zero tiles in their current order
	int values[5] = {0};
	int count = 0;

	for(int i = 0; i < 5; i++)
	{
		if(line[i] != 0)
		{
			values[count] = line[i];
			count = count + 1;
		}
	}

	//merge each pair only once and leave the merged tile for this move
	int merged[5] = {0};
	int merged_count = 0;
	for(int i = 0; i < count; i++)
	{
		if(i + 1 < count && values[i] == values[i + 1])
		{
			merged[merged_count] = values[i] * 2;
			score = score + merged[merged_count];
			merged_count = merged_count + 1;
			i = i + 1;
		}
		else
		{
			merged[merged_count] = values[i];
			merged_count = merged_count + 1;
		}
	}

	//write the collapsed line back and fill the remaining spaces with 0
	for(int i = 0; i < 5; i++)
	{
		if(i < merged_count)
		{
			line[i] = merged[i];
		}
		else
		{
			line[i] = 0;
		}
	}

	return merged_count;
}

bool move_left(int board[][5], int numrows = 5, int *score = nullptr)
{
	int original[5][5] = {};
	int move_score = 0;

	//save the original board so we can tell whether this move changed it
	for(int r = 0; r < numrows; r++)
	{
		for(int c = 0; c < 5; c++)
			original[r][c] = board[r][c];
	}

	//collapse each row toward the left side
	for(int r = 0; r < numrows; r++)
		collapse_line(board[r], move_score);

	if(score != nullptr)
		*score = *score + move_score;

	//compare the original board with the board after the move
	for(int r = 0; r < numrows; r++)
	{
		for(int c = 0; c < 5; c++)
		{
			if(original[r][c] != board[r][c])
				return true;
		}
	}
	return false;
}

bool move_right(int board[][5], int numrows = 5, int *score = nullptr)
{
	int original[5][5] = {};
	int move_score = 0;

	for(int r = 0; r < numrows; r++)
	{
		for(int c = 0; c < 5; c++)
		{
			original[r][c] = board[r][c];
		}
	}

	for(int r = 0; r < numrows; r++)
	{
		//reverse the row so the right side becomes the left side
		int line[5] = {0};
		for(int c = 0; c < 5; c++)
			line[c] = board[r][4 - c];

		collapse_line(line, move_score);

		//reverse the result back into the original row positions
		for(int c = 0; c < 5; c++)
			board[r][4 - c] = line[c];
	}

	if(score != nullptr)
		*score = *score + move_score;

	for(int r = 0; r < numrows; r++)
	{
		for(int c = 0; c < 5; c++)
		{
			if(original[r][c] != board[r][c])
				return true;
		}
	}
	return false;
}

bool move_up(int board[][5], int numrows = 5, int *score = nullptr)
{
	int original[5][5] = {};
	int move_score = 0;

	for(int r = 0; r < numrows; r++)
	{
		for(int c = 0; c < 5; c++)
			original[r][c] = board[r][c];
	}

	//move each column upward by collapsing every column toward row 0
	for(int c = 0; c < 5; c++)
	{
		int line[5] = {0};
		for(int r = 0; r < numrows; r++)
		{
			line[r] = board[r][c];
		}

		collapse_line(line, move_score);

		for(int r = 0; r < numrows; r++)
		{
			board[r][c] = line[r];
		}
	}

	if(score != nullptr)
		*score = *score + move_score;

	for(int r = 0; r < numrows; r++)
	{
		for(int c = 0; c < 5; c++)
		{
			if(original[r][c] != board[r][c])
				return true;
		}
	}
	return false;
}

bool move_down(int board[][5], int numrows = 5, int *score = nullptr)
{
	int original[5][5] = {};
	int move_score = 0;

	for(int r = 0; r < numrows; r++)
	{
		for(int c = 0; c < 5; c++)
		{
			original[r][c] = board[r][c];
		}
	}

	for(int c = 0; c < 5; c++)
	{
		//reverse the column so the bottom side becomes index 0
		int line[5] = {0};
		for(int r = 0; r < numrows; r++)
		{
			line[r] = board[4 - r][c];
		}

		collapse_line(line, move_score);

		//reverse the result back into the original column positions
		for(int r = 0; r < numrows; r++)
		{
			board[4 - r][c] = line[r];
		}
	}

	if(score != nullptr)
		*score = *score + move_score;

	for(int r = 0; r < numrows; r++)
	{
		for(int c = 0; c < 5; c++)
		{
			if(original[r][c] != board[r][c])
			{
				return true;
			}
		}
	}
	return false;
}


void print_board(int board[][5], int numrows = 5)
{
	cout << endl << "\t-----------------------------------------" << endl;

	for (int r = 0; r < numrows; r++)
	{
		for (int c = 0; c < 5; c++)
		{
			if(board[r][c] > 0)
			{
				cout << "\t|" << right << setw(6) << setfill(' ') << board[r][c];
			}
			else
			{
				cout << "\t|" << right << setw(6) << setfill(' ') << " ";
			}

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
	//count empty cells in the board
	int empty_count = 0;
	for (int r = 0; r < numrows; r++)
	{
		for (int c = 0; c < 5; c++)
		{
			if (board[r][c] == 0)
				empty_count++;
		}
	}

	//insert exactly one tile only when the caller has a successful move
	if (empty_count > 0)
	{
		while (true)
		{
			//generate random location r, c
			r = rand() % numrows;
			c = rand() % 5;
			//check if empty
			if (board[r][c] == 0)
			{
				//add a new tile with the value 2
				board[r][c] = 2;
				break;
			}
		}

	}
}