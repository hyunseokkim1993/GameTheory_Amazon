#include "Amazon.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <list>


Game::Game()
{
	FileName = "StartBoard.txt";
	PlayerColor = 'b';
	BoardSize = 10;
}


/******************************************************
 Game Loop
 ******************************************************/
void Game::RunGame()
{
	Initialize();

	while(true)
	{
		if(PlayerColor == 'b')
		{
			PlayerTurn();
			if (DidEnemyLose())
			{
				std::cout << "You Won\n";
				exit(0);
			}
			EnemyTurn();
			if(DidPlayerLose())
			{
				std::cout <<"You Lost\n";
				exit(0);
			}
		}
		else
		{
			EnemyTurn();
			if (DidPlayerLose())
			{
				std::cout << "You Lost\n";
				exit(0);
			}
			PlayerTurn();
			if (DidEnemyLose())
			{
				std::cout << "You Won\n";
				exit(0);
			}
		}
	}
}

/******************************************************
 Initialize Game
 ******************************************************/
void Game::Initialize()
{
	std::cout << "Amazon_Game_by_Kevin_Kim\n";
	std::cout << "Show_ReamMe : y|n ";
	char c;
	std::cin >> c;

	//Print ReadMe/Instruction
	while(true)
	{
		if (c == 'y')
		{
			std::cout << "   Black (b) goes first.\n   White (w) goes second.\n"
				<< "   IMPORTANT: Game requires 'StartBoard.txt' to run.\n"
				<< "   StartBoard.txt must include numbers 0,1,2, and 3 in 10 x 10 format.\n"
				<< "     0 represents empty tile, 1 represents player, 2 represents AI, 3 represents obstacle\n"
				<< "   IMPORTANT: AI needs few seconds to compute a solution during the beginning of the game.\n"
				<< "   Computation time will greatly decrease as game progresses, so please be patient.\n\n";
			break;
		}
		if (c == 'n')
		{
			break;
		}
	}

	std::cout << "Enter Name of txt file with .txt extension (ex: test.txt): ";
	std::string a;
	std::cin >> a;
	FileName = a;
	
	//Pick Player Color/Turn
	while(true)
	{
		std::cout << "Pick_Color : b|w ";
		char color;
		std::cin >> color;
		if(color == 'b')
		{
			std::cout << "Color You Picked is Black.\n";
			PlayerColor = color;
			break;
		}
		if(color == 'w')
		{
			std::cout << "Color You Picked is White.\n";
			PlayerColor = color;
			break;
		}
	}

	//Initialize Board
	ReadBoard();
}



/******************************************************
 Read board from txt file to initialize board state
 ******************************************************/
void Game::ReadBoard()
{
	std::ifstream mapFile;
	mapFile.open(FileName);
	if(mapFile.is_open())
	{
		std::string input;
		int x = 0;
		int y = 0;
		
		while (mapFile >> input)
		{
			std::stringstream string2int(input);
			int i = 0;
			string2int >> i;

			Board[y][x] = i;

			/*Initialize current positions*/
			if(i == 1)
			{
				std::pair<int, int>pos(y, x);
				PositionPlayer.push_back(pos);
			}
			else if (i == 2)
			{
				std::pair<int, int>pos(y, x);
				PositionsEnemy.push_back(pos);
			}
			/*******************************/
			
			x++;
			if(x == 10)
			{
				y++;
				x = 0;
			}
		}
	}
	else
	{
		std::cout << "ERROR:Unable to Open Map File\n";
		exit(0);
	}
	PrintBoard();
}



/******************************************************
 Print Board
 ******************************************************/
void Game::PrintBoard()
{
	for(int i=0; i<11; ++i)
	{
		for(int j=0; j<11; ++j)
		{
			/*Debugging Purpose*/
			/*******************/
			if(i==10)
			{
				for(int k=0; k<10;++k)
				{
					std::cout << k << " ";
				}
				std::cout << "\n";
				break;
			}
			if(j == 10)
			{
				std::cout << i;
				continue;
			}
			/*******************/
			
			if(Board[i][j] == 0)
			{
				std::cout << "-";
			}
			else if (Board[i][j] == 1)
			{
				std::cout << "P";
			}
			else if (Board[i][j] == 2)
			{
				std::cout << "A";
			}
			else if (Board[i][j] == 3)
			{
				std::cout << "X";
			}
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}



/******************************************************
 Player's move
 ******************************************************/
void Game::PlayerTurn()
{
	int selectedX;	int selectedY;
	int newPosX;	int newPosY;
	int obstacleX;	int obstacleY;

	//Select Player Piece
	while (true)
	{
		std::cout << "Select Your Piece:\n";
		std::cout << "X-Coordinate: ";
		std::cin >> selectedX;
		std::cout << "Y-Coordinate: ";
		std::cin >> selectedY;

		if(Board[selectedY][selectedX] == 1)
		{
			break;
		}
		else
		{
			std::cout << "Invalid Input\n";
		}
	}
	//Move Selected Piece
	while(true)
	{
		std::cout << "Move Your Piece:\n";
		std::cout << "X-Coordinate: ";
		std::cin >> newPosX;
		std::cout << "Y-Coordinate: ";
		std::cin >> newPosY;

		if (IsMoveValid(newPosY, newPosX, selectedY, selectedX))
		{
			//Update Board
			Board[newPosY][newPosX] = 1;
			Board[selectedY][selectedX] = 0;
			//Update Position list
			std::pair<int, int> target(selectedY, selectedX);
			PositionPlayer.erase(std::remove(PositionPlayer.begin(), PositionPlayer.end(), target), PositionPlayer.end());
			PositionPlayer.emplace_back(std::pair<int, int>(newPosY, newPosX));

			PrintBoard();
			break;
		}
	}
	//Place Obstacle
	while (true)
	{
		std::cout << "Place Obstacle:\n";
		std::cout << "X-Coordinate: ";
		std::cin >> obstacleX;
		std::cout << "Y-Coordinate: ";
		std::cin >> obstacleY;

		if (IsMoveValid(obstacleY, obstacleX, newPosY, newPosX))
		{
			Board[obstacleY][obstacleX] = 3;
			break;
		}
	}
	PrintBoard();
}



/******************************************************
 Check if move was valid
 ******************************************************/
bool Game::IsMoveValid(int newRow, int newCol, int oldRow, int oldCol, bool dontPrint)
{
	//Out of Bound
	if (newRow > 9 || newCol > 9 || newRow < 0 || newCol < 0)
	{
		if(!dontPrint)
			std::cout << "Invalid Input\n";
		return false;
	}

	//Check Empty
	if (Board[newRow][newCol] != 0)
	{
		if (!dontPrint)
			std::cout << "Invalid Input\n";
		return false;
	}
	
	//Check Straight Path
	if(newRow - oldRow == 0 && newCol - oldCol != 0)
	{
		int high = std::max(newCol, oldCol);
		int low = std::min(newCol, oldCol);
		
		for(int i = low+1; i<high; ++i)
		{
			if(Board[newRow][i] != 0)
			{
				if (!dontPrint)
					std::cout << "Invalid Input: Something in the way\n";
				return false;
			}
		}
		return true;
	}
	if (newRow - oldRow != 0 && newCol - oldCol == 0)
	{
		int high = std::max(newRow, oldRow);
		int low = std::min(newRow, oldRow);

		for (int i = low+1; i < high; ++i)
		{
			if (Board[i][newCol] != 0)
			{
				if (!dontPrint)
					std::cout << "Invalid Input: Something in the way\n";
				return false;
			}
		}
		return true;
	}
	
	//Check Diagonal Path
	int diffRow = std::abs(newRow - oldRow);
	int diffCol = std::abs(newCol - oldCol);
	
	if (diffRow == diffCol)
	{
		int i = oldRow;
		int j = oldCol;
		
		if (oldRow > newRow && oldCol < newCol)
		{
			for (i -= 1, j += 1; i > newRow && j < newCol; --i, ++j)
			{
				if (Board[i][j] != 0) return false;
			}
		}
		if (oldRow < newRow && oldCol > newCol)
		{
			for (i += 1, j -= 1; i < newRow && j > newCol; ++i, --j)
			{
				if (Board[i][j] != 0) return false;
			}
		}
		if (oldRow < newRow && oldCol < newCol)
		{
			for (i += 1, j += 1; i < newRow && j < newCol; i++, j++)
			{
				if (Board[i][j] != 0) return false;
			}
		}
		if (oldRow > newRow && oldCol > newCol)
		{
			for (i -= 1, j -= 1; i > newRow && j > newCol; --i, --j)
			{
				if (Board[i][j] != 0) return false;
			}
		}
		return true;
	}
	


	if (!dontPrint)
		std::cout << "Invalid Input\n";
	return false;
}


/******************************************************
 Get list of positions enemy can make from single spot
 ******************************************************/
std::vector<std::pair<int, int>> Game::GetMovablePositionList(int currRow, int currCol)
{
	std::vector<std::pair<int, int>>list;

	for (int row = 0; row < 10; ++row)
	{
		for (int col = 0; col < 10; ++col)
		{
			if (IsMoveValid(row, col, currRow, currCol ))
			{
				list.emplace_back(std::pair<int, int>(row, col));
			}
		}
	}

	return list;
}



/******************************************************
 Get list of positions enemy can make from single spot
 ******************************************************/
void Game::EnemyTurn()
{
	int bestScope = INT_MAX;
	std::pair<int, int>MoveTo(-1, -1);
	std::pair<int, int>MoveFrom(-1, -1);
	std::pair<int, int>ShootTo(-1,-1);

	using Iter = std::vector<std::pair<int, int>>::const_iterator;

	//For Every Enemy Piece...
	for (Iter it = PositionsEnemy.begin(); it != PositionsEnemy.end(); ++it)
	{
		for (int row = 0; row < BoardSize; ++row)
		{
			for (int col = 0; col < BoardSize; ++col)
			{
				//Search every square
				if (IsMoveValid(row, col, it->first, it->second, true))
				{
					Board[row][col] = 2;
					Board[it->first][it->second] = 0;
					std::pair<int, int> shootLocation = GetOptimalShootLocation(row, col, it->first, it->second);
					int saveInfo = Board[shootLocation.first][shootLocation.second];

					if (shootLocation.first != -1)
					{
						Board[shootLocation.first][shootLocation.second] = 3;
					}
					
					int moveScope = MinMaxMove(0, true);

					Board[row][col] = 0;
					Board[shootLocation.first][shootLocation.second] = 0;
					Board[it->first][it->second] = 2;
					
					int newScope = moveScope;
					
					if (newScope < bestScope)
					{
						MoveFrom.first = it->first;
						MoveFrom.second = it->second;
						MoveTo.first = row;
						MoveTo.second = col;
						ShootTo.first = shootLocation.first;
						ShootTo.second = shootLocation.second;
						bestScope = newScope;
					}
				}
			}
		}
	}


	//Make Move
	Board[MoveFrom.first][MoveFrom.second] = 0;
	Board[MoveTo.first][MoveTo.second] = 2;
	Board[ShootTo.first][ShootTo.second] = 3;

	//Update List
	PositionsEnemy.erase(std::remove(PositionsEnemy.begin(), PositionsEnemy.end(), MoveFrom), PositionsEnemy.end());
	PositionsEnemy.emplace_back(MoveTo);

	std::cout << "Enemy Moved a Piece from (" << MoveFrom.second << ", " << MoveFrom.first << ") to ("
		<< MoveTo.second << ", " << MoveTo.first << ").\n";
	std::cout << "Enemy Shoots Obstacle To (" << ShootTo.second << ", " << ShootTo.first << ").\n";
	
	PrintBoard();
}


int Game::MinMaxMove(int depth, bool isPlayerTurn)
{
	int scope = 0;

	if(isPlayerTurn)
	{
		scope = EvaluatePlayerScope();
	}
	else
	{
		scope = EvaluateEnemyScope();
	}


	//Player can no longer move. AI wins.
	if(scope == 0)
	{
		return scope;
	}

	if(depth == 2)
	{
		return scope;
	}

	if(isPlayerTurn)
	{
		int bestScore = INT_MIN;

		using Iter = std::vector<std::pair<int, int>>::const_iterator;

		for (Iter it = PositionPlayer.begin(); it != PositionPlayer.end(); ++it)
		{
			for(int row =0; row < BoardSize; ++row)
			{
				for(int col=0; col <BoardSize; ++col)
				{
					if(IsMoveValid(row, col, it->first, it->second, true))
					{
						Board[row][col] = 1;
						std::pair<int, int> shootLocation = GetOptimalShootLocation(row, col, it->first, it->second);
						Board[shootLocation.first][shootLocation.second] = 3;
						
						bestScore = std::max(bestScore, MinMaxMove(depth+1, false));

						Board[row][col] = 0;
						Board[shootLocation.first][shootLocation.second] = 0;
					}
				}
			}
		}
		return bestScore;
	}
	else
	{
		int bestScore = INT_MIN;


		std::vector<std::pair<int, int>>Positions;
		for(int i=0; i<BoardSize; i++)
		{
			for(int j=0; j<BoardSize; j++)
			{
				if(Board[i][j] == 2)
				{
					Positions.push_back(std::pair<int, int>(i, j));
				}
			}
		}

		bool hasMovesLeft = false;
		
		using Iter = std::vector<std::pair<int, int>>::const_iterator;
		for (Iter it = Positions.begin(); it != Positions.end(); ++it)
		{
			for (int row = 0; row < BoardSize; ++row)
			{
				for (int col = 0; col < BoardSize; ++col)
				{
					if (IsMoveValid(row, col, it->first, it->second, true))
					{
						hasMovesLeft = true;
						
						Board[row][col] = 1;
						std::pair<int, int> shootLocation = GetOptimalShootLocation(row, col, it->first, it->second);
						Board[shootLocation.first][shootLocation.second] = 3;

						bestScore = std::max(bestScore, MinMaxMove(depth + 1, true));

						Board[row][col] = 0;
						Board[shootLocation.first][shootLocation.second] = 0;
					}
				}
			}
		}

		if(!hasMovesLeft)
		{
			if (DidEnemyLose())
			{
				std::cout << "You Won\n";
				exit(0);
			}
		}
		
		return bestScore;
	}
}

std::pair<int, int> Game::GetOptimalShootLocation(int movedToY, int movedToX, int movedFromY, int movedFromX)
{
	int bestScope = INT_MAX;
	std::pair<int, int>BestLocation(-1,-1);

	//Board[movedFromY][movedFromX] = 0;
	
	for (int row = 0; row < BoardSize; ++row)
	{
		for (int col = 0; col < BoardSize; ++col)
		{
			if (IsMoveValid(row, col, movedToY, movedToX, true))
			{
				Board[row][col] = 3;
				int newScope = EvaluatePlayerScope();
				Board[row][col] = 0;
				if(newScope < bestScope)
				{
					bestScope = newScope;
					BestLocation.first = row;
					BestLocation.second = col;
				}
			}
		}
	}
	//Board[movedFromY][movedFromX] = 2;
	
	
	return BestLocation;
}





void Game::DeleteFromValidList(int row, int col, std::vector<std::pair<int, int>> list)
{
	std::pair<int, int> target(row, col);
	list.erase(std::remove(list.begin(), list.end(), target), list.end());
}



int Game::EvaluatePlayerScope()
{
	using Iter = std::vector<std::pair<int, int>>::const_iterator;

	int ScopeCounter = 0;

	
	for (Iter it = PositionPlayer.begin(); it != PositionPlayer.end(); ++it)
	{
		for (int row = 0; row < BoardSize; ++row)
		{
			for (int col = 0; col < BoardSize; ++col)
			{
				if (IsMoveValid(row, col, it->first, it->second, true))
				{
					ScopeCounter++;
				}
			}
		}
	}

	return ScopeCounter;
}

int Game::EvaluateEnemyScope()
{
	using Iter = std::vector<std::pair<int, int>>::const_iterator;
	
	std::vector<std::pair<int, int>>Positions;
	for (int i = 0; i < BoardSize; i++)
	{
		for (int j = 0; j < BoardSize; j++)
		{
			if (Board[i][j] == 2)
			{
				Positions.push_back(std::pair<int, int>(i, j));
			}
		}
	}
	int ScopeCounter = 0;

	for (Iter it = Positions.begin(); it != Positions.end(); ++it)
	{
		for (int row = 0; row < BoardSize; ++row)
		{
			for (int col = 0; col < BoardSize; ++col)
			{
				if (IsMoveValid(row, col, it->first, it->second, true))
				{
					ScopeCounter++;
				}
			}
		}
	}

	return ScopeCounter;
}


bool Game::DidPlayerLose()
{
	for(int row=0; row<10; ++row)
	{
		for(int col=0; col<10; ++col)
		{
			if (Board[row][col] == 0)
			{
				using Iter = std::vector<std::pair<int, int>>::const_iterator;
				for (Iter it = PositionPlayer.begin(); it != PositionPlayer.end(); ++it)
				{
					if (IsMoveValid(row, col, it->first, it->second, true))
					{
						return false;
					}
				}

			}
		}
	}
	return true;
}


bool Game::DidEnemyLose()
{
	std::vector<std::pair<int, int>>Positions;
	for (int i = 0; i < BoardSize; i++)
	{
		for (int j = 0; j < BoardSize; j++)
		{
			if (Board[i][j] == 2)
			{
				Positions.push_back(std::pair<int, int>(i, j));
			}
		}
	}

	using Iter = std::vector<std::pair<int, int>>::const_iterator;
	for (Iter it = Positions.begin(); it != Positions.end(); ++it)
	{
		for (int row = 0; row < 10; ++row)
		{
			for (int col = 0; col < 10; ++col)
			{
				if (Board[row][col] == 0)
				{
					if (IsMoveValid(row, col, it->first, it->second, true))
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}

//X X X X X - A P - -
//X X X X - X X X - -
//A X - - - X X - - -
//X X X X X X X X X X
//X X X X - - X P - -
//X - X X X X X - - -
//X - - X X X X - - -
//X - - X A X X P - -
//X - - X X A X X - -
//X X P X X X X X - -

//X X X P A X X X X X 0
//X A X X X X X X X X 1
//X X X X X X X X X X 2
//X X X X X X X X X X 3
//X A X X X X X X X X 4
//X X X - X X X X X X 5
//X P X X X - X - X X 6
//X X X X X - ---P 7
//X X A X P X X - --8
//X X - X - X - ---