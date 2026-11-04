#include <iostream>
#include <cstdlib>
#include <ctime>
#include "2048_helper.h"
using namespace std;

void clear_board(int board[][5])
{
	for(int r = 0; r < 5; r++)
	{
		for(int c = 0; c < 5; c++)
		{
			board[r][c] = 0;
		}
	}
}

void insert_two_tiles(int board[][5])
{
	//insert the first tile into a random empty cell
	insert_random(board, 5);

	//insert the second tile into a different random empty cell
	insert_random(board, 5);
}

void start_new_game(int board[][5], int &score, int &successful_moves)
{
	clear_board(board);
	score = 0;
	successful_moves = 0;
	insert_two_tiles(board);
}

int highest_tile(int board[][5])
{
	int highest = 0;

	for(int r = 0; r < 5; r++)
	{
		for(int c = 0; c < 5; c++)
		{
			if(board[r][c] > highest)
			{
				highest = board[r][c];
			}
		}
	}
	return highest;
}

bool is_game_over(int board[][5])
{
	//a full board can still be playable if two neighboring tiles match
	for(int r = 0; r < 5; r++)
	{
		for(int c = 0; c < 5; c++)
		{
			if(board[r][c] == 0)
			{
				return false;
			}

			if(c < 4 && board[r][c] == board[r][c + 1])
			{
				return false;
			}

			if(r < 4 && board[r][c] == board[r + 1][c])
			{
				return false;
			}
		}
	}
	return true;
}

void display_game_state(int board[][5], int score, int successful_moves)
{
	cout << endl;
	print_board(board);
	cout << "Score: " << score << endl;
	cout << "Successful moves: " << successful_moves << endl;
	cout << "Highest tile: " << highest_tile(board) << endl;
}

void display_help()
{
	cout << endl;
	cout << "Commands:" << endl;
	cout << "u - move up" << endl;
	cout << "d - move down" << endl;
	cout << "l - move left" << endl;
	cout << "r - move right" << endl;
	cout << "s - save game" << endl;
	cout << "o - open/load game" << endl;
	cout << "n - start a new game" << endl;
	cout << "h - show help" << endl;
	cout << "e - exit" << endl;
	cout << "save and load are not implemented" << endl;
}

int main()
{
	int size = 5;
	int board[5][5] = {};
	int score = 0;
	int successful_moves = 0;
	bool win_announced = false;

	srand(static_cast<unsigned int>(time(nullptr)));
	start_new_game(board, score, successful_moves);

	cout << "Welcome to 2048!" << endl;
	display_game_state(board, score, successful_moves);
	display_help();

	char choice = ' ';
	while (choice != 'e')
	{
		cout << endl << "Please enter a command: " << endl;
		cout << "Choice: ";

		if(!(cin >> choice))
		{
			//stop safely when there is no more keyboard or redirected input
			cout << "exiting" << endl;
			break;
		}

		bool game_over = is_game_over(board);

		//movement commands are blocked when the board is full and has no merges
		if(game_over && (choice == 'u' || choice == 'd' || choice == 'l' || choice == 'r'))
		{
			cout << "game over" << endl;
			continue;
		}

		bool changed = false;
		bool was_movement = false;

		if(choice == 'u')
		{
			was_movement = true;
			changed = move_up(board, size, &score);
		}
		else if(choice == 'd')
		{
			was_movement = true;
			changed = move_down(board, size, &score);
		}
		else if(choice == 'l')
		{
			was_movement = true;
			changed = move_left(board, size, &score);
		}
		else if(choice == 'r')
		{
			was_movement = true;
			changed = move_right(board, size, &score);
		}
		else if(choice == 's')
		{
			cout << "save is not implemented" << endl;
		}
		else if(choice == 'o')
		{
			cout << "load is not implemented" << endl;
		}
		else if(choice == 'n')
		{
			char confirm = 'n';
			cout << "Start a new game and erase the current game? (y/n): ";

			if(cin >> confirm)
			{
				if(confirm == 'y' || confirm == 'Y')
				{
					start_new_game(board, score, successful_moves);
					win_announced = false;
					display_game_state(board, score, successful_moves);
				}
				else
				{
					cout << "new game canceled" << endl;
				}
			}
		}
		else if(choice == 'h')
		{
			display_help();
		}
		else if(choice == 'e')
		{
			cout << "exiting" << endl;
		}
		else
		{
			cout << "Invalid command. Enter h to view the commands." << endl;
		}

		//only a successful move inserts one tile and increases the move count
		if(changed)
		{
			successful_moves = successful_moves+1;
			insert_random(board, size);
			display_game_state(board, score, successful_moves);

			//announce the win once when the highest tile reaches 2048
			if(!win_announced && highest_tile(board) >= 2048)
			{
				cout << "Congratulations! You reached 2048." << endl;
				win_announced = true;
			}

			if(is_game_over(board))
				cout << "Game over!" << endl;
		}
		else if(was_movement)
		{
			//unchanged moves do not insert a tile or change counters, but still display state
			display_game_state(board, score, successful_moves);
		}

	}


	return 0;
}