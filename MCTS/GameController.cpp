#include "GameController.h"
#include <iostream>

#include <sstream>
#include <string>
#include <chrono>
#include <fstream>
#include <ctime>

GameController::GameController()
{
    gameRunning = false;



    // obtain Math and Random singletons
    Global_Random = AIRandom_Global::getInstance();
    Global_Math = AIMath_Global::getInstance();
    Global_Blackboard = AIBlackboard_GlobalTTT::getInstance();
}

GameController::~GameController()
{


}


// ** Intialisation **

/*
* Init should be called prior to starting to game
*/
void GameController::Init()
{

    //initialise the graphics 
    LoadGraphicsAssests();
    CreateBoard();
    SetBoardSprites();

    InitAI();
    InitGame();
}

void GameController::InitGame()
{
    turnState = TurnState::Waiting;
    statusText = "Waiting for Player";
}

/*
* InitAI initalises the AIConstructor. Must be called before game starts.
*/
void GameController::InitAI()
{
    mctsConstructor.Init();
    mctsConstructor.DefineAI();

    aiOpponent.Init("AI", mctsConstructor);
    aiOpponent.SetGameContext(*this);
}



void GameController::CreateBoard()
{
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            board[x][y] = TerrainType::Board;
        }
    }

}
/*
LoadGraphicsAssests will load the graphics assets from file erady for use
*/
void GameController::LoadGraphicsAssests()
{


    bool load = xMarkerTexture.loadFromFile("Assets/X_Marker.png");
    load = oMarkerTexture.loadFromFile("Assets/O_Marker.png");
    load = boardBackTexture.loadFromFile("Assets/BoardTile.png");

    font.openFromFile("Assets/DefaultAriel.ttf");

}


void GameController::SetBoardSprites()
{
    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            if (board[x][y] == TerrainType::Board)
            {
                boardArea.push_back(sf::Sprite(boardBackTexture));
            }


        }
    }
}

//** Events and Game Loop **


/*
* Update is called each frame and should only contain code desined to be called this often
*/
void GameController::Update(float dt)
{

    if (turnState == TurnState::Setup)
    {
        StartGame();
    }

    if (gameRunning)
    {
        if (turnState == TurnState::EndTurn) {
            NextTurn();
        }
        else if (turnState == TurnState::AITakingTurn)
        {
            AITurn();
        }


    }
}



/**
* getCharForState will return a character to represent each BOARD_SQUARE_STATE
*/
char GameController::GetCharForState(BOARD_SQUARE_STATE printState)
{
    // Print characters for specific states
    switch (printState)
    {
    case BOARD_SQUARE_STATE::NONE:
        return '.';
    case BOARD_SQUARE_STATE::CROSS:
        return 'X';
    case BOARD_SQUARE_STATE::CIRCLE:
        return 'O';
    }

    return ' ';
}


bool GameController::CheckWin()
{
    BOARD_SQUARE_STATE _winner = mainGameState.CheckWin();

    // number of moves played so far on the board
    int _turnsTaken = 9 - (int)mainGameState.GetPossibleMoves().size();

    // appends one line per finished game to a CSV log file (Task 11)
    auto _logResult = [&](const std::string& _result)
        {
            std::ofstream _log("mcts_results.csv", std::ios::app);

            // write a header if the file is new/empty
            if (_log.tellp() == 0)
            {
                _log << "time,result,ai_marker,turns_taken,max_runs" << std::endl;
            }

            std::time_t _now = std::time(nullptr);
            std::tm _tm;
            localtime_s(&_tm, &_now);
            char _timeBuf[32];
            std::strftime(_timeBuf, sizeof(_timeBuf), "%Y-%m-%d %H:%M:%S", &_tm);

            _log << _timeBuf << ","
                << _result << ","
                << GetCharForState(aiMarker) << ","
                << _turnsTaken << ","
                << maxRuns << std::endl;
        };

    if (_winner != BOARD_SQUARE_STATE::NONE)
    {
        std::cout << "Winner - " << _winner << std::endl;
        std::string _winnerText = "Winner - ";

        _winnerText.push_back(GetCharForState(_winner));
        UpdateStatusText(_winnerText);

        _logResult(_winner == aiMarker ? "AI Win" : "Player Win");

        StopGame();
        return true;
    }

    // no winner and no empty squares left, so the game is a draw
    if (mainGameState.GetPossibleMoves().size() == 0)
    {
        UpdateStatusText("Draw");

        _logResult("Draw");

        StopGame();
        return true;
    }


    return false;
}


/*
* HandleKeyPress will handle the SFML key presses
*/
void GameController::HandleKeyPress(const sf::Event::KeyPressed* _keyPress)
{
    if (!gameRunning)
        return;

}

/*
* HandleKeyPress will handle the SFML mouse presses
*/
void GameController::HandleMousePress(const sf::Event::MouseButtonPressed* _mousePress)
{

    if (!gameRunning)
        return;

    if (_mousePress->button == sf::Mouse::Button::Right)
    {

    }
    else if (_mousePress->button == sf::Mouse::Button::Left)
    {

        if (turnState == TurnState::Waiting)
        {

            int gridRow = ConvertScreenPosToGridPos(_mousePress->position.y);
            int gridCol = ConvertScreenPosToGridPos(_mousePress->position.x);

            if (gridRow < 0 || gridRow >= mapWidth || gridCol < 0 || gridCol >= mapHeight)
            {
                // click out of board, ignore
            }
            else
            {

                bool validMove = isValidMove(gridRow, gridCol);
                if (validMove)
                {
                    MakeMove(gridRow, gridCol);
                    ProgressNextTurn();
                }
            }
        }
    }
}


int GameController::ConvertScreenPosToGridPos(float screenPos)
{

    return (screenPos / mapSectionXY);
}


/*
* AITurn will tick the active character, causing them to select and execude an action for their turn
*/
void GameController::AITurn()
{

    // implement a slight delay, so the player sees the AI thinking and then acting
    if (clock.getElapsedTime().asSeconds() < turnDelay)
        return;

    aiOpponent.Update(1.f);

    ProgressNextTurn();

}



// ** Rendering **


/*
DisplayUI can be used to display data useful for debugging or tracking on the right panel
*/
void GameController::DisplayUI(sf::RenderWindow& window)
{

    DisplayUIText(window, statusText, 10, 800);

    DisplayUIText(window, "Tic Tac Toe", 800, 50);

    std::string _aiMark = "AI Marker : ";
    _aiMark.push_back(GetCharForState(aiMarker));

    std::string _humanMark = "Human Marker : ";
    _humanMark.push_back(GetCharForState(playerMarker));

    DisplayUIText(window, _aiMark, 800, 150);
    DisplayUIText(window, _humanMark, 800, 200);


    DisplayMCTSNodes(window);


}

void GameController::DisplayMCTSNodes(sf::RenderWindow& window)
{


    if (!Global_Blackboard->HasValue<std::vector<int>>("AINodeVisits"))
        return;

    std::vector <int> _nodeVisits = Global_Blackboard->GetValue< std::vector<int>>("AINodeVisits");

    std::vector <int> _nodeWins = Global_Blackboard->GetValue<std::vector<int>>("AINodeWins");
    std::vector <int> _nodeActionRow = Global_Blackboard->GetValue<std::vector<int>>("AINodeRow");
    std::vector <int> _nodeActionCol = Global_Blackboard->GetValue<std::vector<int>>("AINodeCol");

    AIMath_Global::Vector2f _aiMove = Global_Blackboard->GetValue<AIMath_Global::Vector2f>("AIMove");

    float yPos = 50.f;

    std::string _header = "Row\tCol\tVisits\tWins\tWin %";
    DisplayUIText(window, _header, 800, 300);
    for (int i = 0; i < _nodeActionCol.size(); i++)
    {
        std::ostringstream os;


        os.setf(std::ios::fixed);
        os.precision(2);

        float _winPercent = 0.f;

        if (_nodeVisits[i] != 0)
            _winPercent = (float)(_nodeWins[i]) / _nodeVisits[i];

        std::string wp = std::format("{:.2f}", _winPercent);


        os << std::to_string(_nodeActionRow[i]) << "\t\t" << std::to_string(_nodeActionCol[i]) << "\t\t" << std::to_string(_nodeVisits[i]) << "\t\t" << std::to_string(_nodeWins[i]) << "\t\t" << wp;

        if (_nodeActionRow[i] == _aiMove.y && _nodeActionCol[i] == _aiMove.x)
        {
            os << "\t**SELECT**" << std::endl;
        }
        else
        {
            os << std::endl;
        }


        DisplayUIText(window, os.str(), 800, 300 + (yPos * (i + 1)));
    }

}

void GameController::DisplayUIText(sf::RenderWindow& window, std::string _text, float _xPos, float _yPos)
{

    sf::Text text(font); // a font is required to make a text object


    // set the string to display
    text.setString(_text);

    // set the character size
    text.setCharacterSize(42); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::White);
    text.setPosition({ _xPos,_yPos });

    window.draw(text);


}


void GameController::Render(sf::RenderWindow& window)
{


    DrawBoard(window);
    DrawMarkers(window);
    DisplayUI(window);
}


/*
* DrawMap will draw the background tiles for the map
*/
void GameController::DrawBoard(sf::RenderWindow& window)
{
    for (int col = 0; col < mapWidth; col++)
    {
        for (int row = 0; row < mapHeight; row++)
        {
            int _index = (col * mapHeight) + row;

            boardArea[_index].setPosition({ col * mapSectionXY, row * mapSectionXY });
            window.draw(boardArea[_index]);
        }
    }
}


/*
* DrawMap will draw the background tiles for the map
*/
void GameController::DrawMarkers(sf::RenderWindow& window)
{
    for (int col = 0; col < mapWidth; col++)
    {
        for (int row = 0; row < mapHeight; row++)
        {

            if (mainGameState.gameBoard.GetBoardSquare(row, col) == BOARD_SQUARE_STATE::CIRCLE) {
                sf::Sprite _s(oMarkerTexture);
                _s.setPosition({ col * mapSectionXY, row * mapSectionXY });
                window.draw(_s);
            }
            else
            {
                if (mainGameState.gameBoard.GetBoardSquare(row, col) == BOARD_SQUARE_STATE::CROSS) {
                    sf::Sprite _s(xMarkerTexture);
                    _s.setPosition({ col * mapSectionXY, row * mapSectionXY });
                    window.draw(_s);
                }
            }

        }
    }
}




// ** Game Status **



/*
* UpdateStatusText will set the text that appears below the game play area, as well as log to the console output
*/
void GameController::UpdateStatusText(std::string _newText)
{
    std::cout << "TURN LOG: " << _newText << std::endl;
    statusText = _newText;
}

/*
* StartGame will set the game status to running
*/
void GameController::StartGame()
{

    // implement a small delay to allow the starting player message to be seen
    if (clock.getElapsedTime().asSeconds() < turnDelay)
        return;


    gameRunning = true;

    if (markerTurn == aiMarker)
    {
        turnState = TurnState::AITakingTurn;
    }
    else  if (markerTurn == playerMarker)
    {
        turnState = TurnState::Waiting;
    }




}

/*
* SetupGame will initialise the game, setting the starting player, and updating the global blackboard with this information
* The game state will then be set to game running
*/
void GameController::SetupGame()
{
    Global_Blackboard->AddNewValue<int>("PlayerMarker", playerMarker);
    Global_Blackboard->AddNewValue<int>("AIMarker", aiMarker);
    Global_Blackboard->AddNewValue<int>("MaxRuns", maxRuns);
    UpdateBlackboardState();

    StartingPlayer _starter = startingPlayer;

    if (_starter == StartingPlayer::Random)
    {
        float _turn = Global_Random->GetRandom01();

        if (_turn >= 0.5f)
        {
            _starter = StartingPlayer::AI;
        }
        else
        {
            _starter = StartingPlayer::Human;
        }
    }
    std::string _startMessage = "Starting Player is : ";
    if (_starter == StartingPlayer::AI)
    {
        _startMessage.append("AI");
        markerTurn = aiMarker;

    }
    else  if (_starter == StartingPlayer::Human)
    {
        _startMessage.append("Human");
        markerTurn = playerMarker;

    }

    UpdateStatusText(_startMessage);

    clock.restart();
    turnState = TurnState::Setup;
}


void GameController::StopGame()
{
    gameRunning = false;
}


/*
* ProgressNextTurn shoudl be called to move to the next player's turn
*/
void GameController::ProgressNextTurn()
{
    clock.restart();

    turnState = TurnState::EndTurn;
}


/*
NextTurn will handle the swtich of player at turn end and set the game either waiting for player or AI  decision
*/
void GameController::NextTurn()
{

    // implement a small delay to allow all actions to be seen
    if (clock.getElapsedTime().asSeconds() < turnDelay)
        return;

    UpdateStatusText("Processesing...");

    UpdateBlackboardState();

    bool winner = CheckWin();

    if (!winner)
    {

        markerTurn = GetOppositeMarker(markerTurn);

        if (markerTurn == aiMarker)
        {
            UpdateStatusText("AI Thinking...");
            clock.restart();
            turnState = TurnState::AITakingTurn;
        }
        else
        {
            UpdateStatusText("Waiting for Player...");
            turnState = TurnState::Waiting;

        }

    }


}

void GameController::UpdateBlackboardState()
{
    Global_Blackboard->EditValue<GameState>("GameState", mainGameState);
}

/*
* isValidMove will return True if the provided row/col move is a valid one given the main game state, otherwsise False
*/
bool GameController::isValidMove(int _row, int _col)
{

    if (mainGameState.gameBoard.GetBoardSquare(_row, _col) == BOARD_SQUARE_STATE::NONE)
    {
        return true;
    }
    else
    {
        std::cout << "ERROR - grid  row " << _row << " col " << _col << " is not empty " << std::endl;
        return false;
    }
}


/*
* MakeMove will update the game board with the provided row/col. The move should be a valid one, checked by isValidMove().
*/
void GameController::MakeMove(int _row, int _col)
{
    mainGameState.SetAndApplyAction(GameAction(_row, _col, markerTurn));

    std::ostringstream  os;
    std::string _p = "Marker ";
    _p.push_back(GetCharForState(markerTurn));

    os << _p << " Move => Row:" << _row << " Col:" << _col << std::endl;

    std::string _s = os.str();
    UpdateStatusText(_s);

}

// - Refernece and Lookups


/*

GetOppositeMarker will return the opposite marker than the one provided. E.g. if X is provided, O will be returned.
*/
BOARD_SQUARE_STATE GameController::GetOppositeMarker(BOARD_SQUARE_STATE _marker)
{
    if (_marker == BOARD_SQUARE_STATE::CIRCLE)
        return BOARD_SQUARE_STATE::CROSS;
    else if (_marker == BOARD_SQUARE_STATE::CROSS)
        return BOARD_SQUARE_STATE::CIRCLE;

    return BOARD_SQUARE_STATE::NONE;

}