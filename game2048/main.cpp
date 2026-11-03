#include <iostream>
#include "2048_helper.h"
using namespace std;

int main()
{
	int size = 5;
	int board[5][5] = {
		{0, 4, 2, 4, 0},
		{0, 0, 0, 0, 0},
		{2, 0, 2, 0, 2},
		{0, 0, 2, 0, 0},
		{2, 2, 2, 2, 2},
	};
	//while getch == x loop for up down left right

	cout << "Initial Board : [5x5] :" << endl;
	print_board(board);
	int score = 0;
	int successful_moves = 0;
	cout << "Score: " << score << "\tSuccessful moves: " << successful_moves << endl;

	char choice = 'u';	//initialized
	while (choice != 'e')
	{

		cout << "Please enter choice:{u,d,l,r,e} for UP, DOWN, LEFT, RIGHT, Exit: " << endl;
		cout << "Choice: ";
		cin >> choice;

		bool valid_move = true;
		bool changed = false;

		if (choice == 'u')
			changed = move_up(board, size, &score);
		else if (choice == 'd')
			changed = move_down(board, size, &score);
		else if (choice == 'l')
			changed = move_left(board, size, &score);
		else if (choice == 'r')
			changed = move_right(board, size, &score);
		else if (choice == 'e')
			valid_move = false;
		else
			valid_move = false;

		//only successful moves insert one tile and increase the move count
		if(valid_move && changed)
		{
			successful_moves = successful_moves + 1;
			insert_random(board, size);
		}

		print_board(board);
		cout << "Score: " << score << "\tSuccessful moves: " << successful_moves << endl;

	}


	return 0;
}