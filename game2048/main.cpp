#include <iostream>
#include <cstdio> //provides standard input and output functions
#include <fstream> //provides ifstream and ofstream for reading and writing files
#include <string> //provides the string class for text data
#include <limits> //provides numeric limits for checking and clearing input values
#include <cstdlib> //provides rand() and srand() for random number generation
#include <ctime> //provides time() for creating a random seed
#include "2048_helper.h"
using namespace std;

const char SAVE_FILE[] = "savegame.txt";
const char SAVE_MARKER[] = "CS211_2048_V1";

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
	cout << "savegame.txt is replaced when saving again" << endl;
}

bool save_game(int board[][5], int score, int successful_moves)
{
	//open the single save slot; opening with trunc replaces an older save
	ofstream output(SAVE_FILE);
	if(!output.is_open())
	{
		cout << "error: could not open savegame.txt for writing" << endl;
		return false;
	}

	output << SAVE_MARKER << endl;
	output << score << " " << successful_moves << endl;

	//write all 25 board values so every empty or occupied cell is preserved
	for(int r = 0; r < 5; r++)
	{
		for(int c = 0; c < 5; c++)
		{
			output << board[r][c];
			if(c < 4)
				output << " ";
		}
		output << endl;
	}

	output.close();
	if(output.fail())
	{
		cout << "error: could not finish writing savegame.txt" << endl;
		return false;
	}

	cout << "game saved successfully to savegame.txt" << endl;
	return true;
}

bool valid_save_tile(long long tile)
{
	//load only 0 or powers of two at least 2
	if(tile == 0)
		return true;

	if(tile < 2 || tile > numeric_limits<int>::max())
		return false;

	return (tile & (tile - 1)) == 0;
}

bool load_game(int board[][5], int &score, int &successful_moves)
{
	ifstream input(SAVE_FILE);
	if(!input.is_open())
	{
		cout << "error: savegame.txt was not found or could not be opened" << endl;
		return false;
	}

	//read all saved data into temporary variables before changing the active game
	string marker;
	long long saved_score = -1;
	long long saved_moves = -1;
	long long saved_board[5][5] = {};

	if(!(input >> marker) || marker != SAVE_MARKER)
	{
		cout << "error: invalid save file marker" << endl;
		return false;
	}

	if(!(input >> saved_score) || !(input >> saved_moves) ||
	   saved_score < 0 || saved_moves < 0 ||
	   saved_score > numeric_limits<int>::max() ||
	   saved_moves > numeric_limits<int>::max())
	{
		cout << "error: score and successful-move count must be nonnegative integers" << endl;
		return false;
	}

	//read exactly 25 board values and validate the type of each value
	for(int r = 0; r < 5; r++)
	{
		for(int c = 0; c < 5; c++)
		{
			if(!(input >> saved_board[r][c]))
			{
				cout << "error: save file is missing board values or contains malformed data" << endl;
				return false;
			}

			if(!valid_save_tile(saved_board[r][c]))
			{
				cout << "error: save file contains an invalid tile value" << endl;
				return false;
			}
		}
	}

	//skip whitespace, then reject extra non-whitespace content
	input >> ws;
	if(input.peek() != EOF)
	{
		cout << "error: save file contains extra content after the 25 board values" << endl;
		return false;
	}

	//replace active game state only after every validation check passes
	score = static_cast<int>(saved_score);
	successful_moves = static_cast<int>(saved_moves);
	for(int r = 0; r < 5; r++)
	{
		for(int c = 0; c < 5; c++)
			board[r][c] = static_cast<int>(saved_board[r][c]);
	}

	cout << "game loaded successfully from savegame.txt" << endl;
	return true;
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
			save_game(board, score, successful_moves);
		}
		else if(choice == 'o')
		{
			if(load_game(board, score, successful_moves))
			{
				win_announced = highest_tile(board) >= 2048;
				display_game_state(board, score, successful_moves);
			}
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