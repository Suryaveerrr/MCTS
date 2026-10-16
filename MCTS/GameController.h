#pragma once

#include "../AI_Framework/framework.h"
#include <SFML/Graphics.hpp>
#include "GameBoard.h"
#include "GameState.h"
#include "AIActor_TicTacToe.h"
#include "AIConstructor_MCTS.h"
#include "AIBlackboard_GlobalTTT.h"

class GameController
{
public:
	enum TerrainType { Board };
	enum TurnState { Waiting, AITakingTurn,  EndTurn, Setup };
	enum StartingPlayer {Random, AI, Human};


	GameController();
	~GameController();

	//Intialisation
	void Init();
	void LoadGraphicsAssests();

	//Events and Game Loop
	void Update(float dt);
	void HandleKeyPress(const sf::Event::KeyPressed* _keyPress);
	void HandleMousePress(const sf::Event::MouseButtonPressed* _mousePress);
	void MakeMove(int _row, int _col);

	// Rendering
	void DisplayUI(sf::RenderWindow& window);
	void Render(sf::RenderWindow& window);


	// Game Status
	void StartGame();
	void SetupGame();

	bool gameRunning;

private:


	// constants
	static const int mapWidth = 3;
	static const int mapHeight = 3;
	const float mapSectionXY = 256.0f;

	const int maxRuns = 1000;

	// timing
	sf::Clock clock;
	const float turnDelay = 2.0f;


	// creeate the initial board and root node for the tree
	GameState mainGameState;

	// define the markers used by the AI and the player
	const BOARD_SQUARE_STATE aiMarker = BOARD_SQUARE_STATE::CROSS;
	const BOARD_SQUARE_STATE playerMarker = BOARD_SQUARE_STATE::CIRCLE;

	// Status & Tracking
	TurnState turnState;
	BOARD_SQUARE_STATE markerTurn = BOARD_SQUARE_STATE::CIRCLE;

	// AI
	AIActor_TicTacToe aiOpponent;
	AIConstructor_MCTS mctsConstructor;

	// Game Status
	void ProgressNextTurn();
	void NextTurn();
	void AITurn();
	bool isValidMove(int _row, int _col);
	void UpdateBlackboardState();
	void StopGame();

	// Initialisation
	void InitGame();

	// Gets and Checks
	BOARD_SQUARE_STATE GetOppositeMarker(BOARD_SQUARE_STATE _marker);
	char GetCharForState(BOARD_SQUARE_STATE _marke);
	bool CheckWin();


	std::string statusText;
	StartingPlayer startingPlayer = StartingPlayer::Random;
	// AI Initialisation
	void InitAI();
	void CreateBoard();
	void SetBoardSprites();



	// Rendering
	void DrawBoard(sf::RenderWindow& window);
	void DrawMarkers(sf::RenderWindow& window);
	void DisplayUIText(sf::RenderWindow& window, std::string _text, float _xPos, float _yPos);
	void DisplayMCTSNodes(sf::RenderWindow& window);
	void UpdateStatusText(std::string _newText);
	int ConvertScreenPosToGridPos(float screenPos);

	//Graphics
	sf::Font font;

	sf::Texture xMarkerTexture;
	sf::Texture oMarkerTexture;
	sf::Texture boardBackTexture;

	TerrainType board[mapWidth][mapHeight];

	std::vector< sf::Sprite > boardArea;

	//Global Access
	AIRandom_Global* Global_Random;
	AIMath_Global* Global_Math;
	AIBlackboard_GlobalTTT* Global_Blackboard;

};

