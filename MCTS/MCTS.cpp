// MCTS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <SFML/Graphics.hpp>
#include "GameController.h"
#include <iostream>
#include <fstream>

/*
HandleEvents will capture SFML keyboard and mouse input and route to Game Controller to handle
*/
void HandleEvents(sf::RenderWindow& window, GameController* gameController)
{

    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                window.close();
            else
            {
                gameController->HandleKeyPress(keyPressed);
            }
        }
        else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            gameController->HandleMousePress(mouseButtonPressed);
        }
    }
}




int main()
{


    // define window
    sf::RenderWindow window(sf::VideoMode({ 1700, 1000 }), "Tic Tac Toe Game");
    window.setVerticalSyncEnabled(true);



    //define the views
    sf::View gameView(sf::FloatRect({ 0, 0 }, { 1700, 1000.f }));

    gameView.setViewport(sf::FloatRect({ 0.f, 0.f }, {1.f, 1.f }));
 
    gameView.setCenter({ 850.f, 500.f });

    sf::Clock clock;

    // define the game controller, init and start the game 
    GameController gameController;
    gameController.Init();
    gameController.SetupGame();



    //display and game loop
    while (window.isOpen())
    {


        // handle all events
        HandleEvents(window, &gameController);


        // display the game section of the window
        window.clear();
        window.setView(gameView);
        gameController.Render(window);

        gameController.DisplayUI(window);
  

        //call the function to display
        window.display();


        // tick the game controller 
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();
        gameController.Update(dt);



    }
}

