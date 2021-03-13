#pragma once
#include <string>
#include <utility>
#include <vector>

class Game
{
public:
	Game();
	void RunGame();
	void PrintBoard();
	bool DidPlayerLose();
	bool DidEnemyLose();
	void Initialize();
	void PlayerTurn();
	void EnemyTurn();
	bool IsMoveValid(int newRow, int newCol, int oldRow, int oldCol, bool dontPrint = false);
	void ReadBoard();
	void DeleteFromValidList(int row, int col, std::vector<std::pair<int, int>>list);
	std::vector<std::pair<int, int>> GetMovablePositionList(int currRow, int currCol);

	int MinMaxMove(int depth, bool isPlayerTurn);
	std::pair<int, int> GetOptimalShootLocation(int movedToY, int movedToX, int movedFromY, int movedFromX);

	void MoveEfficiently();
	
	int EvaluatePlayerScope();
	int EvaluateEnemyScope();
	
private:
	int Board[10][10] = {0};
	char PlayerColor;
	int BoardSize;
	std::string FileName;

	std::vector<std::pair<int, int>>PositionPlayer;
	std::vector<std::pair<int, int>>PositionsEnemy;
};
